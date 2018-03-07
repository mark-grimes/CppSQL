#ifndef INCLUDEGUARD_cppsql_SQLiteDatabase_h
#define INCLUDEGUARD_cppsql_SQLiteDatabase_h

#include <string>
#include <functional>
#include <sqlite3.h>
#include "cppsql/SQLiteStatement.h"

namespace cppsql
{
	class SQLiteDatabase
	{
	public:
		SQLiteDatabase( const char* pFilename );
		SQLiteDatabase( const std::string& filename ) : SQLiteDatabase(filename.c_str()) {}
		~SQLiteDatabase();

		void execute( const char* pCommand );
		void execute( const char* pCommand, std::function<bool(int,const char* const[],const char* const[])> resultsCallback );
		SQLiteStatement prepareStatement( const char* pStatement );
	private:
		void execute_( const char* pCommand, void* userCallback );
		static int callback_( void *pMyFunction, int argc, char **argv, char **azColName );
		sqlite3* pDatabase_;
	};

} // end of namespace cppsql

#endif
