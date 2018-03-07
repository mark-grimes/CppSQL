#include "cppsql/MySQLDatabase.h"

cppsql::MySQLDatabase::MySQLDatabase( const char* host, const char* user, const char* password, const char* database )
	: pDatabase_(nullptr)
{
	pDatabase_=mysql_init(nullptr);

	if( pDatabase_==nullptr )
	{
		throw std::runtime_error( std::string("Error initiating MySQL: ")+mysql_error(pDatabase_) );
	}

	if( mysql_real_connect( pDatabase_, host, user, password, database, 0, nullptr, 0 )==nullptr )
	{
		throw std::runtime_error( std::string("Can't open database: ")+mysql_error(pDatabase_));
	}
}

cppsql::MySQLDatabase::~MySQLDatabase()
{
	mysql_close(pDatabase_);
}

std::vector<std::string> cppsql::MySQLDatabase::tableNames() const
{
	std::vector<std::string> returnValue;
	cppsql::MySQLResult tableNamesResult( mysql_list_tables( pDatabase_, nullptr ) );
	MYSQL_ROW tableNames;
	while( (tableNames=mysql_fetch_row(tableNamesResult)) )
	{
		returnValue.emplace_back( tableNames[0] );
	} // end of loop over table names

	return returnValue;
}

void cppsql::MySQLDatabase::execute( const char* pCommand )
{
	//std::cout << "Executing command " << pCommand << std::endl;
	if( mysql_query( pDatabase_, pCommand ) )
	{
		throw std::runtime_error( std::string("Error performing query: ")+mysql_error(pDatabase_) );
	}
}

cppsql::MySQLResult cppsql::MySQLDatabase::execute_new( const char* pCommand )
{
	execute( pCommand );
	return MySQLResult( mysql_store_result(pDatabase_) );
}

void cppsql::MySQLDatabase::execute( const char* pCommand, std::function<bool(int,const char* const[],const char* const[])> resultsCallback )
{
	execute( pCommand );

	MySQLResult result( mysql_store_result(pDatabase_) );
	if( result==nullptr )
	{
		throw std::runtime_error(std::string("Error getting result: ")+mysql_error(pDatabase_) );
	}

	int num_fields=mysql_num_fields(result);
	MYSQL_FIELD* fields=mysql_fetch_fields(result);

	std::vector<const char*> fieldNames(num_fields);
	for( size_t index=0; index<num_fields; ++index ) fieldNames[index]=fields[index].name;

	MYSQL_ROW row;

	bool keepGoing=true;
	while( (row=mysql_fetch_row(result)) && keepGoing )
	{
		keepGoing=resultsCallback( num_fields, row, fieldNames.data() );
	}
}

cppsql::MySQLStatement cppsql::MySQLDatabase::prepareStatement( const char* pStatement )
{
	MYSQL_STMT* pHandle=mysql_stmt_init(pDatabase_);
	if( pHandle==nullptr ) throw std::runtime_error("Couldn't initialise the statement handler");
	if( mysql_stmt_prepare( pHandle, pStatement, std::char_traits<char>::length(pStatement) )!=0 )
	{
		throw std::runtime_error( std::string("Could not prepare statement: ")+mysql_stmt_error(pHandle) );
	}
	return MySQLStatement(pHandle);
}
