#ifndef INCLUDEGUARD_cppsql_MySQLResult_h
#define INCLUDEGUARD_cppsql_MySQLResult_h

#include <string>
#include <functional>
#include <mysql.h>

namespace cppsql
{
	class MySQLResult
	{
	public:
		MySQLResult( MYSQL_RES* pResult );
		~MySQLResult();
		int numberOfFields();
		void showResults( std::function<bool(int,const char* const[],const char* const[])> resultsCallback );
		operator MYSQL_RES*() { return pResult_; }
	private:
		MYSQL_RES* pResult_;
	};

} // end of namespace cppsql

#endif
