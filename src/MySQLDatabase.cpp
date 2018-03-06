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

void cppsql::MySQLDatabase::execute( const char* pCommand, std::function<bool(int,int,char*,char*)> resultsCallback )
{
	execute( pCommand );

	MySQLResult result( mysql_store_result(pDatabase_) );
	if( result==nullptr )
	{
		throw std::runtime_error(std::string("Error getting result: ")+mysql_error(pDatabase_) );
	}

	int num_fields=mysql_num_fields(result);
	MYSQL_FIELD* fields=mysql_fetch_fields(result);

	MYSQL_ROW row;

	bool keepGoing=true;
	while( (row=mysql_fetch_row(result)) && keepGoing )
	{
		for( int fieldIndex=0; fieldIndex<num_fields && keepGoing; ++fieldIndex )
		{
			keepGoing=resultsCallback( fieldIndex, num_fields, row[fieldIndex], fields[fieldIndex].name );
		}
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
