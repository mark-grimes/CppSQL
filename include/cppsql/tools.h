#ifndef INCLUDEGUARD_cppsql_tools_h
#define INCLUDEGUARD_cppsql_tools_h

#include <memory>
// Forward declarations
namespace cppsql
{
	class IDatabase;
}

namespace cppsql
{
	/** @brief Copy the complete MySQL database to an SQLite file
	 *
	 * Will probably fail if the SQLite file already exists.
	 * @author Mark Grimes (mark.grimes@rymapt.com)
	 * @date 07/Mar/2018
	 */
	void mySQLToSQLiteBackup( const char* host, const char* user, const char* password, const char* database, const char* outputFile );

	/** @brief Open a database using an SQLAlchemy style url
	 *
	 * See http://docs.sqlalchemy.org/en/latest/core/engines.html for what this function is trying to
	 * emulate, but note that only minimal conformance is expected, i.e. just strings starting with
	 * "mysql://" or "sqlite://", anything else will fail.
	 */
	std::unique_ptr<cppsql::IDatabase> openDatabase( const std::string& url );
	std::unique_ptr<cppsql::IDatabase> openDatabase( const char* url );
} // end of namespace cppsql

#endif
