#ifndef INCLUDEGUARD_cppsql_misc_h
#define INCLUDEGUARD_cppsql_misc_h

#include <mysql.h>
#include "cppsql/SQLiteStatement.h"

namespace cppsql
{
	const char* mySqlTypeString( const MYSQL_FIELD& field );
	void bindMySQLToSQLite( const MYSQL_BIND& bind, cppsql::SQLiteStatement& statement, const int fieldNumber );
	/** @brief MySQL type as a string */
	const char* mySQLTypeAsString( int type );

	/** @brief Parse an sqlalchemy style database URL.
	 *
	 * See http://docs.sqlalchemy.org/en/latest/core/engines.html for what this function is trying to
	 * emulate, but note that only minimal conformance is expected, i.e. just strings starting with
	 * "mysql://" or "sqlite://", anything else will throw an error.
	 *
	 * If the string starts "mysql://", then the output parameters mysqlHost, mysqlUsername,
	 * mysqlPassword and mysqlDatabase will be filled (assuming no other error). If the string starts
	 * "sqlite://" then only sqliteFilename will be filled.
	 *
	 * The function will return false if there is any parsing error.
	 */
	bool parseSQLAlchemyURL( const std::string& url, std::string& mysqlHost, std::string& mysqlUsername, std::string& mysqlPassword, std::string& mysqlDatabase, std::string& sqliteFilename );
	bool parseSQLAlchemyURL( const char* url, std::string& mysqlHost, std::string& mysqlUsername, std::string& mysqlPassword, std::string& mysqlDatabase, std::string& sqliteFilename );
} // end of namespace cppsql

#endif
