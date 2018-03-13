#include "cppsql/SQLiteDatabase.h"
#include "cppsql/errorcodes.h"

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

std::vector<std::string> cppsql::SQLiteDatabase::tableNames() const
{
	std::vector<std::string> returnValue;

	// Horrible dirty trick so that I can call the non-const execute method
	const_cast<cppsql::SQLiteDatabase*>(this)->execute("SELECT name FROM sqlite_master WHERE type='table'",
		[&returnValue]( int numFields, const char* const value[], const char* const fieldName[] )
		{
			returnValue.emplace_back( value[0] );
			return true;
		});

	return returnValue;
}

void cppsql::SQLiteDatabase::execute( const char* pCommand )
{
	execute_( pCommand, nullptr );
}

void cppsql::SQLiteDatabase::execute( const char* pCommand, std::function<bool(int,const char* const[],const char* const[])> resultsCallback )
{
	execute_( pCommand, &resultsCallback );
}

void cppsql::SQLiteDatabase::execute_( const char* pCommand, void* userCallback )
{
	//std::cout << "Executing command " << pCommand << std::endl;
	char* pErrorMsg=0;
	int result=sqlite3_exec( pDatabase_, pCommand, (userCallback ? callback_ : nullptr), userCallback, &pErrorMsg );
	if( result==SQLITE_ABORT && userCallback!=nullptr ) return; // User returned false from callback to stop processing
	else if( result!=SQLITE_OK )
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
		throw std::runtime_error( std::string("Can't prepare SQLiteStatement '")+pCommand+"': "+cppsql::liteResultCodeToString(result) );
	}
	return SQLiteStatement( pStatement, pDatabase_ );
}

int cppsql::SQLiteDatabase::callback_( void* pMyFunction, int argc, char** argv, char** azColName )
{
	if( pMyFunction==nullptr ) return 0;
	std::function<bool(int,const char* const[],const char* const[])>& callback=*reinterpret_cast<std::function<bool(int,const char* const[],const char* const[])>*>(pMyFunction);

	bool keepGoing=callback( argc, argv, azColName );

	return (keepGoing ? 0 : 1);
}
