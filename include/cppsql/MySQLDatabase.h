#ifndef INCLUDEGUARD_cppsql_MySQLDatabase_h
#define INCLUDEGUARD_cppsql_MySQLDatabase_h

#include <string>
#include <mysql.h>
#include "cppsql/MySQLResult.h"
#include "cppsql/MySQLStatement.h"


namespace cppsql
{
	class MySQLDatabase
	{
	public:
		MySQLDatabase( const char* host, const char* user, const char* password, const char* database );
		~MySQLDatabase();

		void execute( const char* pCommand );
		cppsql::MySQLResult execute_new( const char* pCommand );
		void execute( const char* pCommand, std::function<bool(int,const char* const[],const char* const[])> resultsCallback );
		cppsql::MySQLStatement prepareStatement( const char* pStatement );
		MYSQL* rawHandle() { return pDatabase_; }
	private:
		MYSQL* pDatabase_;
	};

} // end of namespace cppsql

#endif
