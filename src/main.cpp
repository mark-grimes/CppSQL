#include <iostream>
#include "cppsql/version.h"
#include "cppsql/MySQLDatabase.h"
#include "cppsql/SQLiteDatabase.h"
#include "cppsql/tools.h"
#include <mysql.h>

class ResultHandler
{
public:
	virtual ~ResultHandler() {}
	virtual bool handleField( const char* fieldName, signed char value, bool isFinalField ) = 0;
	virtual bool handleField( const char* fieldName, short int value, bool isFinalField ) = 0;
	virtual bool handleField( const char* fieldName, int value, bool isFinalField ) = 0;
	virtual bool handleField( const char* fieldName, long long int value, bool isFinalField ) = 0;
	virtual bool handleField( const char* fieldName, float value, bool isFinalField ) = 0;
	virtual bool handleField( const char* fieldName, double value, bool isFinalField ) = 0;
	virtual bool handleField( const char* fieldName, const MYSQL_TIME& value, bool isFinalField ) = 0;
	virtual bool handleField( const char* fieldName, signed char* value, bool isFinalField ) = 0;
	virtual bool handleField( const char* fieldName, char* value, bool isFinalField ) = 0;
};

class DumpResultHandler : public ResultHandler
{
public:
	DumpResultHandler( std::ostream& outputStream ) : outputStream_(outputStream) {}
	virtual ~DumpResultHandler() {}
	virtual bool handleField( const char* fieldName, signed char value, bool isFinalField ) override { return handleField_(fieldName,(int)value,isFinalField); }
	virtual bool handleField( const char* fieldName, short int value, bool isFinalField ) override { return handleField_(fieldName,value,isFinalField); }
	virtual bool handleField( const char* fieldName, int value, bool isFinalField ) override { return handleField_(fieldName,value,isFinalField); }
	virtual bool handleField( const char* fieldName, long long int value, bool isFinalField ) override { return handleField_(fieldName,value,isFinalField); }
	virtual bool handleField( const char* fieldName, float value, bool isFinalField ) override { return handleField_(fieldName,value,isFinalField); }
	virtual bool handleField( const char* fieldName, double value, bool isFinalField ) override { return handleField_(fieldName,value,isFinalField); }
	virtual bool handleField( const char* fieldName, const MYSQL_TIME& value, bool isFinalField ) override { return handleField_(fieldName,"MYSQL_TIME",isFinalField); }
	virtual bool handleField( const char* fieldName, signed char* value, bool isFinalField ) override { return handleField_(fieldName,value,isFinalField); }
	virtual bool handleField( const char* fieldName, char* value, bool isFinalField ) override { return handleField_(fieldName,value,isFinalField); }
private:
	std::ostream& outputStream_;
	size_t numRecords_ = 0;
	template<typename T>
	bool handleField_( const char* fieldName, T&& value, bool isFinalField )
	{
		outputStream_ << fieldName << "=" << value << (isFinalField ? "\n" : ", ");
		// if( isFinalField ) ++numRecords_;
		return numRecords_<10;
	}
};

#include "cppsql/misc.h"

bool callHandler( const MYSQL_BIND& bind, const char* fieldName, ResultHandler& handler, bool isFinalField )
{
	switch( bind.buffer_type )
	{
		case MYSQL_TYPE_TINY: return handler.handleField( fieldName, *(signed char*)bind.buffer, isFinalField );
		case MYSQL_TYPE_SHORT: return handler.handleField( fieldName, *(short int*)bind.buffer, isFinalField );
		case MYSQL_TYPE_INT24: return handler.handleField( fieldName, *(int*)bind.buffer, isFinalField );
		case MYSQL_TYPE_LONG: return handler.handleField( fieldName, *(int*)bind.buffer, isFinalField );
		case MYSQL_TYPE_LONGLONG: return handler.handleField( fieldName, *(long long int*)bind.buffer, isFinalField );
		case MYSQL_TYPE_FLOAT: return handler.handleField( fieldName, *(float*)bind.buffer, isFinalField );
		case MYSQL_TYPE_DOUBLE: return handler.handleField( fieldName, *(double*)bind.buffer, isFinalField );
		case MYSQL_TYPE_NEWDECIMAL: return handler.handleField( fieldName, (char*)bind.buffer, isFinalField );
		case MYSQL_TYPE_TIME: return handler.handleField( fieldName, *(MYSQL_TIME*)bind.buffer, isFinalField );
		case MYSQL_TYPE_DATE: return handler.handleField( fieldName, *(MYSQL_TIME*)bind.buffer, isFinalField );
		case MYSQL_TYPE_DATETIME: return handler.handleField( fieldName, *(MYSQL_TIME*)bind.buffer, isFinalField );
		case MYSQL_TYPE_TIMESTAMP: return handler.handleField( fieldName, *(MYSQL_TIME*)bind.buffer, isFinalField );
		case MYSQL_TYPE_STRING: return handler.handleField( fieldName, (char*)bind.buffer, isFinalField );
		case MYSQL_TYPE_VAR_STRING: return handler.handleField( fieldName, (char*)bind.buffer, isFinalField );
		case MYSQL_TYPE_TINY_BLOB: return handler.handleField( fieldName, (char*)bind.buffer, isFinalField );
		case MYSQL_TYPE_BLOB: return handler.handleField( fieldName, (char*)bind.buffer, isFinalField );
		case MYSQL_TYPE_MEDIUM_BLOB: return handler.handleField( fieldName, (char*)bind.buffer, isFinalField );
		case MYSQL_TYPE_LONG_BLOB: return handler.handleField( fieldName, (char*)bind.buffer, isFinalField );
		case MYSQL_TYPE_BIT: return handler.handleField( fieldName, (char*)bind.buffer, isFinalField );
		default: throw std::runtime_error("MySQL type"+std::to_string(bind.buffer_type)+" not recognised");
	}
}

void executeStatement( cppsql::MySQLDatabase& database, const char* statement, ResultHandler& handler )
{
	auto selectStatement=database.prepareStatement( statement );
	if( mysql_stmt_execute(selectStatement)!=0 ) throw std::runtime_error("Failed executing statement");
	const my_bool trueBool=1;
	if( mysql_stmt_attr_set(selectStatement,STMT_ATTR_UPDATE_MAX_LENGTH,&trueBool)!=0 ) throw std::runtime_error("Failed setting statement attribute");
	if( mysql_stmt_store_result(selectStatement)!=0 ) throw std::runtime_error("Failed storing result");
	cppsql::MySQLResult metadata( mysql_stmt_result_metadata(selectStatement) );
	const int numberOfFields=metadata.numberOfFields();
	MYSQL_FIELD* fields=mysql_fetch_fields(metadata);

	std::vector<MYSQL_BIND> results(numberOfFields);
	std::vector<std::vector<char>> buffers(numberOfFields);
	std::vector<long unsigned int> lengths(numberOfFields);
	for( int columnNameIndex=0; columnNameIndex<numberOfFields; ++columnNameIndex )
	{
		buffers[columnNameIndex].resize(fields[columnNameIndex].max_length+1);
		results[columnNameIndex].length=&lengths[columnNameIndex];
		results[columnNameIndex].buffer=buffers[columnNameIndex].data();
		results[columnNameIndex].buffer_type=fields[columnNameIndex].type;
		results[columnNameIndex].buffer_length=(buffers[columnNameIndex].empty() ? 0 : buffers[columnNameIndex].size()-1);
	}

	if( mysql_stmt_bind_result(selectStatement,results.data())!=0 ) throw std::runtime_error("Failed binding result");
	int fetchResult;
	bool keepLooping=true;
	while( (fetchResult=mysql_stmt_fetch(selectStatement))!=MYSQL_NO_DATA && keepLooping )
	{
		if( fetchResult!=0 ) std::cout << "Didn't work " << cppsql::stmt_fetch_errorCode(fetchResult) << std::endl;
		for( int columnNameIndex=0; columnNameIndex<numberOfFields && keepLooping; ++columnNameIndex )
		{
			keepLooping = callHandler( results[columnNameIndex], fields[columnNameIndex].name, handler, columnNameIndex+1==numberOfFields );
		}
	}
}

void mySQLDumpStuffUsingHandler( const char* host, const char* user, const char* password, const char* database )
{
	cppsql::MySQLDatabase inputDatabase( host, user, password, database );

	cppsql::MySQLResult tableNamesResult( mysql_list_tables( inputDatabase.rawHandle(), nullptr ) );
	MYSQL_ROW tableNames;
	while( (tableNames=mysql_fetch_row(tableNamesResult)) )
	{
		DumpResultHandler dumpHandler( std::cout );
		const std::string tableName(tableNames[0]);
		std::cout << "Table " << tableName << "\n";
		executeStatement( inputDatabase, ("SELECT * FROM "+tableName).c_str(), dumpHandler );
	} // end of loop over table names
}

// Callback used to process query results that just prints everything to std::cout
bool printResults( int numFields, const char* const values[], const char* const fieldNames[] )
{
	for( size_t index=0; index<numFields; ++index )
	{
		std::cout << fieldNames[index] << "=" << values[index] << (index+1==numFields ? "\n" : ", ");
	}
	return true;
}

void mySQLDumpStuff( const char* host, const char* user, const char* password, const char* database )
{
	cppsql::MySQLDatabase inputDatabase( host, user, password, database );

	cppsql::MySQLResult tableNamesResult( mysql_list_tables( inputDatabase.rawHandle(), nullptr ) );
	MYSQL_ROW tableNames;
	while( (tableNames=mysql_fetch_row(tableNamesResult)) )
	{
		DumpResultHandler dumpHandler( std::cout );
		const std::string tableName(tableNames[0]);
		std::cout << "Table " << tableName << "\n";
		inputDatabase.execute( ("SELECT * FROM "+tableName).c_str(), printResults );
	} // end of loop over table names
}

void sqliteDumpStuff( const char* sqliteFilename )
{
	cppsql::SQLiteDatabase database( sqliteFilename );
	std::vector<std::string> tableNames;
	database.execute("SELECT name FROM sqlite_master WHERE type='table'",
		[&tableNames]( int numFields, const char* const value[], const char* const fieldName[] )
		{
			tableNames.emplace_back( value[0] );
			return true;
		});
	for( const auto& tableName : tableNames )
	{
		std::cout << "Table " << tableName << "\n";
		database.execute( ("SELECT * FROM "+tableName).c_str(), printResults );
	}
}

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
		//cppsql::mySQLToSQLiteBackup( *args[0], *args[1], *args[2], *args[3], *args[4] );
		mySQLDumpStuff( *args[0], *args[1], *args[2], *args[3] );
		sqliteDumpStuff( *args[4] );
		mySQLDumpStuffUsingHandler( *args[0], *args[1], *args[2], *args[3] );
	}
	catch( const std::exception& error )
	{
		std::cerr << "Exception: " << error.what() << std::endl;
		return -1;
	}

	return 0;
}
