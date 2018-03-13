#ifndef INCLUDEGUARD_cppsql_MySQLDatabase_h
#define INCLUDEGUARD_cppsql_MySQLDatabase_h

#include "cppsql/IDatabase.h"
#include <string>
#include <mysql.h>
#include "cppsql/MySQLResult.h"
#include "cppsql/MySQLStatement.h"


namespace cppsql
{
	class MySQLDatabase : public cppsql::IDatabase
	{
	public:
		MySQLDatabase( const char* host, const char* user, const char* password, const char* database );
		virtual ~MySQLDatabase();

		virtual std::vector<std::string> tableNames() const override;
		virtual void execute( const char* pCommand ) override;
		cppsql::MySQLResult execute_new( const char* pCommand );
		virtual void execute( const char* pCommand, std::function<bool(int,const char* const[],const char* const[])> resultsCallback ) override;
		cppsql::MySQLStatement prepareStatement( const char* pStatement );
		MYSQL* rawHandle() { return pDatabase_; }
	private:
		MYSQL* pDatabase_;
	};

} // end of namespace cppsql

#endif
