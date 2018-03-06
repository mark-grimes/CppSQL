#include "cppsql/MySQLStatement.h"

cppsql::MySQLStatement::MySQLStatement( MYSQL_STMT* pStatement )
	: pStatement_(pStatement)
{
	// No operation besides the initialiser list
}

cppsql::MySQLStatement::~MySQLStatement()
{
	mysql_stmt_free_result(pStatement_);
	mysql_stmt_close(pStatement_);
}

void cppsql::MySQLStatement::reset()
{

}

void cppsql::MySQLStatement::step()
{
	if( mysql_stmt_execute(pStatement_)!=0 )
	{
		throw std::runtime_error( std::string("Error performing query: ")+mysql_stmt_error(pStatement_) );
	}
}

void cppsql::MySQLStatement::bindResults( std::vector<MYSQL_BIND>& results )
{
	if( mysql_stmt_bind_result( pStatement_, results.data() )!=0 )
	{
		throw std::runtime_error( "Unable to bind results" );
	}
}
