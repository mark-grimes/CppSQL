#ifndef INCLUDEGUARD_cppsql_errorcodes_h
#define INCLUDEGUARD_cppsql_errorcodes_h

namespace cppsql
{
	const char* liteResultCodeToString( int result );

	/** @brief Error code to string for MySQL mysql_stmt_fetch function */
	const char* stmt_fetch_errorCode( int errorCode );
} // end of namespace cppsql

#endif
