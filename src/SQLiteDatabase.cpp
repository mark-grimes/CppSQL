#include "cppsql/SQLiteDatabase.h"

cppsql::SQLiteDatabase::SQLiteDatabase( const char* pFilename )
	: pDatabase_(nullptr)
{
	int result=sqlite3_open( pFilename, &pDatabase_ );
	if( result!=SQLITE_OK )
	{
		sqlite3_close( pDatabase_ );
		throw std::runtime_error( std::string("Can't open database: ")+sqlite3_errmsg(pDatabase_) );
	}
}

cppsql::SQLiteDatabase::~SQLiteDatabase()
{
	sqlite3_close( pDatabase_ );
}

void cppsql::SQLiteDatabase::execute( const char* pCommand )
{
	execute_( pCommand, nullptr );
}

void cppsql::SQLiteDatabase::execute( const char* pCommand, std::function<bool(int,int,char*,char*)> resultsCallback )
{
	execute_( pCommand, &resultsCallback );
}

void cppsql::SQLiteDatabase::execute_( const char* pCommand, void* userCallback )
{
	//std::cout << "Executing command " << pCommand << std::endl;
	char* pErrorMsg=0;
	int result=sqlite3_exec( pDatabase_, pCommand, (userCallback ? callback_ : nullptr), userCallback, &pErrorMsg );
	if( result!=SQLITE_OK )
	{
		std::string errorMessage(pErrorMsg);
		sqlite3_free( pErrorMsg );
		throw std::runtime_error(std::string("Error executing command '")+pCommand+"': "+errorMessage);
	}
}

cppsql::SQLiteStatement cppsql::SQLiteDatabase::prepareStatement( const char* pCommand )
{
	sqlite3_stmt* pStatement=nullptr;
	int result;
	if( (result=sqlite3_prepare_v2( pDatabase_, pCommand, std::char_traits<char>::length(pCommand), &pStatement, NULL ))!=SQLITE_OK )
	{
		throw std::runtime_error( std::string("Can't prepare SQLiteStatement '")+pCommand+"': "+liteResultCodeToString(result) );
	}
	return SQLiteStatement( pStatement, pDatabase_ );
}

int cppsql::SQLiteDatabase::callback_( void* pMyFunction, int argc, char** argv, char** azColName )
{
	if( pMyFunction==nullptr ) return 0;
	std::function<bool(int,int,char*,char*)>& callback=*reinterpret_cast<std::function<bool(int,int,char*,char*)>*>(pMyFunction);

	bool keepGoing=true;
	for( int index=0; index<argc && keepGoing; ++index )
	{
		keepGoing=callback( index, argc, argv[index], azColName[index] );
	}

	return (keepGoing ? 0 : 1);
}
