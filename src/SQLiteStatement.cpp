#include "cppsql/SQLiteStatement.h"

cppsql::SQLiteStatement::SQLiteStatement( sqlite3_stmt* pStatement, sqlite3* pDatabase )
	: pStatement_(pStatement), pDatabase_(pDatabase)
{
	// No operation besides the initialiser list
}

cppsql::SQLiteStatement::~SQLiteStatement()
{
	sqlite3_finalize(pStatement_);
}

void cppsql::SQLiteStatement::reset()
{
	sqlite3_reset(pStatement_);
}

void cppsql::SQLiteStatement::step()
{
	int result;
	if( (result=sqlite3_step(pStatement_))!=SQLITE_DONE )
	{
		throw std::runtime_error( std::string("Can't step SQLiteStatement: ")+liteResultCodeToString(result) );
	}
}

template<> void cppsql::SQLiteStatement::bind( const double value, int position )
{
	int result;
	if( (result=sqlite3_bind_double( pStatement_, position, value ))!=SQLITE_OK )
	{
		throw std::runtime_error( std::string("Can't bind double to SQLiteStatement: ")+cppsql::liteResultCodeToString(result) );
	}
}

template<> void cppsql::SQLiteStatement::bind( const float value, int position )
{
	int result;
	if( (result=sqlite3_bind_double( pStatement_, position, value ))!=SQLITE_OK )
	{
		throw std::runtime_error( std::string("Can't bind float to SQLiteStatement: ")+cppsql::liteResultCodeToString(result) );
	}
}

template<> void cppsql::SQLiteStatement::bind( const int value, int position )
{
	int result;
	if( (result=sqlite3_bind_int( pStatement_, position, value ))!=SQLITE_OK )
	{
		throw std::runtime_error( std::string("Can't bind int to SQLiteStatement: ")+cppsql::liteResultCodeToString(result) );
	}
}

template<> void cppsql::SQLiteStatement::bind( const short value, int position )
{
	int result;
	if( (result=sqlite3_bind_int( pStatement_, position, value ))!=SQLITE_OK )
	{
		throw std::runtime_error( std::string("Can't bind short to SQLiteStatement: ")+cppsql::liteResultCodeToString(result) );
	}
}

template<> void cppsql::SQLiteStatement::bind( const char* value, int position )
{
	int result;
	if( (result=sqlite3_bind_text( pStatement_, position, value, -1, nullptr ))!=SQLITE_OK )
	{
		throw std::runtime_error( std::string("Can't bind short to SQLiteStatement: ")+cppsql::liteResultCodeToString(result) );
	}
}

/// Note that this function is for MYSQL_TYPE_TINY, i.e. a single byte, not a pointer to an array of bytes
template<> void cppsql::SQLiteStatement::bind( const signed char value, int position )
{
	int result;
	if( (result=sqlite3_bind_int( pStatement_, position, value ))!=SQLITE_OK )
	{
		throw std::runtime_error( std::string("Can't bind signed char to SQLiteStatement: ")+cppsql::liteResultCodeToString(result) );
	}
}

template<> void cppsql::SQLiteStatement::bind( const void* value, int position, int size )
{
	int result;
	if( (result=sqlite3_bind_blob( pStatement_, position, value, size, nullptr ))!=SQLITE_OK )
	{
		throw std::runtime_error( std::string("Can't bind blob to SQLiteStatement: ")+cppsql::liteResultCodeToString(result) );
	}
}
