#ifndef INCLUDEGUARD_cppsql_MySQLStatement_h
#define INCLUDEGUARD_cppsql_MySQLStatement_h

#include <string>
#include <vector>
#include <mysql.h>

namespace cppsql
{
	class MySQLStatement
	{
	public:
		~MySQLStatement();
		template<typename T> void bind( const T value, int position );
		void reset();
		void step();
		void bindResults( std::vector<MYSQL_BIND>& results );
		operator MYSQL_STMT*() { return pStatement_; }
	private:
		friend class MySQLDatabase;
		MySQLStatement( MYSQL_STMT* pStatement );
		MYSQL_STMT* pStatement_;
	};
} // end of namespace cppsql

template<typename T> void cppsql::MySQLStatement::bind( const T value, int position )
{

}

#endif
