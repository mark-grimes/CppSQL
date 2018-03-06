#ifndef INCLUDEGUARD_cppsql_tools_h
#define INCLUDEGUARD_cppsql_tools_h

#include <mysql.h>
#include "cppsql/SQLiteStatement.h"

namespace cppsql
{
	const char* sqlTypeString( const MYSQL_FIELD& field );
	void bindToSQLite( const MYSQL_BIND& bind, cppsql::SQLiteStatement& statement, const int fieldNumber );
	/** @brief MySQL type as a string */
	const char* typeAsString( int type );
} // end of namespace cppsql

#endif
