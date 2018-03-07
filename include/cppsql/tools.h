#ifndef INCLUDEGUARD_cppsql_tools_h
#define INCLUDEGUARD_cppsql_tools_h


namespace cppsql
{
	void mySQLBackup( const char* host, const char* user, const char* password, const char* database, const char* outputFile );
} // end of namespace cppsql

#endif
