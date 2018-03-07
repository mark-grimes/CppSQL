#ifndef INCLUDEGUARD_cppsql_tools_h
#define INCLUDEGUARD_cppsql_tools_h


namespace cppsql
{
	/** @brief Copy the complete MySQL database to an SQLite file
	 *
	 * Will probably fail if the SQLite file already exists.
	 * @author Mark Grimes (mark.grimes@rymapt.com)
	 * @date 07/Mar/2018
	 */
	void mySQLToSQLiteBackup( const char* host, const char* user, const char* password, const char* database, const char* outputFile );
} // end of namespace cppsql

#endif
