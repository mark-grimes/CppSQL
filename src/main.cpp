#include <iostream>
#include "cppsql/version.h"
#include "cppsql/MySQLDatabase.h"
#include "cppsql/SQLiteDatabase.h"
#include "cppsql/tools.h"
#include <mysql.h>

void mySQLBackup( const char* host, const char* user, const char* password, const char* database, const char* outputFile );

int main( int argc, const char* argv[] )
{
	std::cout.sync_with_stdio(false);
	try
	{
		const char* defaultArgs[]={ "localhost", "ethoscope", "ethoscope", "ethoscope_db", "testdb.sqlite" };
		const char** args[]={ &defaultArgs[0], &defaultArgs[1], &defaultArgs[2], &defaultArgs[3], &defaultArgs[4] };
		for( int index=1; index<argc; ++index ) args[index-1]=&argv[index];
		std::cout << "Database host     = " << *args[0] << "\n"
			<< "Database username = " << *args[1] << "\n"
			<< "Database password = " << *args[2] << "\n"
			<< "Database database = " << *args[3] << "\n"
			<< "Output filename   = " << *args[4] << "\n";
		mySQLBackup( *args[0], *args[1], *args[2], *args[3], *args[4] );
	}
	catch( const std::exception& error )
	{
		std::cerr << "Exception: " << error.what() << std::endl;
		return -1;
	}

	return 0;
}

void mySQLBackup( const char* host, const char* user, const char* password, const char* database, const char* outputFile )
{
	cppsql::MySQLDatabase inputDatabase( host, user, password, database );
	cppsql::SQLiteDatabase outputDatabase( outputFile );
	outputDatabase.execute("PRAGMA synchronous=OFF");
	outputDatabase.execute("PRAGMA count_changes=OFF");
	outputDatabase.execute("PRAGMA journal_mode=MEMORY");
	outputDatabase.execute("PRAGMA temp_store=MEMORY");

	cppsql::MySQLResult tableNamesResult( mysql_list_tables( inputDatabase.rawHandle(), nullptr ) );
	MYSQL_ROW tableNames;
	while( (tableNames=mysql_fetch_row(tableNamesResult)) )
	{
		for( int fieldIndex=0; fieldIndex<tableNamesResult.numberOfFields(); ++fieldIndex )
		{
			std::string tableName(tableNames[fieldIndex]);
			std::cout << "Table " << tableName << std::endl;

			auto selectStatement=inputDatabase.prepareStatement( (std::string("SELECT * FROM ")+tableName).c_str() );
			if( mysql_stmt_execute(selectStatement)!=0 ) throw std::runtime_error("Failed executing select statement");
			const my_bool trueBool=1;
			if( mysql_stmt_attr_set(selectStatement,STMT_ATTR_UPDATE_MAX_LENGTH,&trueBool)!=0 ) throw std::runtime_error("Failed setting statement attribute");
			if( mysql_stmt_store_result(selectStatement)!=0 ) throw std::runtime_error("Failed storing result");
			cppsql::MySQLResult metadata( mysql_stmt_result_metadata(selectStatement) );
			const int numberOfFields=metadata.numberOfFields();
			MYSQL_FIELD* fields=mysql_fetch_fields(metadata);
			/*for( int columnNameIndex=0; columnNameIndex<numberOfFields; ++columnNameIndex )
			{
				std::cout << "   "
					<< (fields[columnNameIndex].name ? fields[columnNameIndex].name : "null")
					<< ", " << cppsql::typeAsString(fields[columnNameIndex].type)
					<< ", " << fields[columnNameIndex].length
					<< ", " << fields[columnNameIndex].max_length
					<< "\n";
			}*/
			std::vector<MYSQL_BIND> results(numberOfFields);
			std::vector<std::vector<char>> buffers(numberOfFields);
			std::vector<long unsigned int> lengths(numberOfFields);
			std::string createStatementSQL("CREATE TABLE '"+tableName+"'(");
			for( int columnNameIndex=0; columnNameIndex<numberOfFields; ++columnNameIndex )
			{
				buffers[columnNameIndex].resize(fields[columnNameIndex].max_length+1);
				results[columnNameIndex].length=&lengths[columnNameIndex];
				results[columnNameIndex].buffer=buffers[columnNameIndex].data();
				results[columnNameIndex].buffer_type=fields[columnNameIndex].type;
				results[columnNameIndex].buffer_length=(buffers[columnNameIndex].empty() ? 0 : buffers[columnNameIndex].size()-1);
				if( columnNameIndex!=0 ) createStatementSQL+=", '";
				else createStatementSQL+="'";
				createStatementSQL+=fields[columnNameIndex].name;
				createStatementSQL+="' ";
				createStatementSQL+=cppsql::sqlTypeString(fields[columnNameIndex]);
			}
			createStatementSQL+=")";
			outputDatabase.execute("BEGIN TRANSACTION");
			outputDatabase.execute(createStatementSQL.c_str());
			// Prepare the INSERT statement for the output database
			std::string insertStatementSQL("INSERT INTO "+tableName+" VALUES(");
			for( int index=0; index<numberOfFields; ++index )
			{
				if( index!=0 ) insertStatementSQL+=", ";
				insertStatementSQL+="?"+std::to_string(index+1);
			}
			insertStatementSQL+=")";
			auto insertStatement=outputDatabase.prepareStatement(insertStatementSQL.c_str());
			//if( mysql_stmt_bind_param(selectStatement,params.data())!=0 ) throw std::runtime_error("Failed binding param");
			if( mysql_stmt_bind_result(selectStatement,results.data())!=0 ) throw std::runtime_error("Failed binding result");
			int fetchResult;
			while( (fetchResult=mysql_stmt_fetch(selectStatement))!=MYSQL_NO_DATA )
			{
				if( fetchResult!=0 ) std::cout << "Didn't work " << cppsql::stmt_fetch_errorCode(fetchResult) << std::endl;//throw std::runtime_error("Failed fetching result: '"+std::string(mysql_stmt_error(selectStatement))+"'-'"+std::string(mysql_error(inputDatabase.rawHandle()))+"'");
				for( int columnNameIndex=0; columnNameIndex<numberOfFields; ++columnNameIndex )
				{
					//std::cout << (columnNameIndex==0 ? "  " : ", ") << fields[columnNameIndex].name << "=" << BufferDump(results[columnNameIndex]);
					cppsql::bindToSQLite( results[columnNameIndex], insertStatement, columnNameIndex );
				}
				//std::cout << "\n";
				insertStatement.step();
				insertStatement.reset();
			}
			outputDatabase.execute("END TRANSACTION");
		} // end of loop over fields in the table names call (pretty sure there is only one)
	} // end of loop over table names
} // end of function mySQLBackup
