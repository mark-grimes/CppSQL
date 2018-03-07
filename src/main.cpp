#include <iostream>
#include "cppsql/version.h"
#include "cppsql/MySQLDatabase.h"
#include "cppsql/SQLiteDatabase.h"
#include "cppsql/tools.h"
#include <mysql.h>


int main( int argc, const char* argv[] )
{
	std::cout.sync_with_stdio(false);
	try
	{
		const char* defaultArgs[]={ "localhost", "ethoscope", "ethoscope", "ethoscope_db", "testdb.sqlite" };
		const char** args[]={ &defaultArgs[0], &defaultArgs[1], &defaultArgs[2], &defaultArgs[3], &defaultArgs[4] };
		for( int index=1; index<argc; ++index ) args[index-1]=&argv[index];
		std::cout << "Database host     = " << *args[0] << "\n"
			<< "Database username = " << *args[1] << "\n"
			<< "Database password = " << *args[2] << "\n"
			<< "Database database = " << *args[3] << "\n"
			<< "Output filename   = " << *args[4] << "\n";
		cppsql::mySQLToSQLiteBackup( *args[0], *args[1], *args[2], *args[3], *args[4] );
	}
	catch( const std::exception& error )
	{
		std::cerr << "Exception: " << error.what() << std::endl;
		return -1;
	}

	return 0;
}
