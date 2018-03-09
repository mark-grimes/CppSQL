#include "cppsql/tools.h"
#include "cppsql/IDatabase.h"
#include "catch.hpp"

SCENARIO( "Test that the cppsql::openDatabase function works properly for SQLite databases", "[tools]" )
{
	WHEN( "Opening a memory database" )
	{
		std::unique_ptr<cppsql::IDatabase> pDatabase;
		CHECK_NOTHROW( pDatabase=cppsql::openDatabase("sqlite://") ); // equivalent to ":memory:"
		CHECK( pDatabase!=nullptr );

		// Perform a few checks to make sure it is actually valid
		std::vector<std::string> tableNames;
		CHECK_NOTHROW( tableNames=pDatabase->tableNames() );
		CHECK( tableNames.empty() );
		CHECK_NOTHROW( pDatabase->execute("CREATE TABLE mytest (id INTEGER PRIMARY KEY, foo INTEGER)") );
		CHECK_NOTHROW( tableNames=pDatabase->tableNames() );
		CHECK( tableNames.size()==1 );
		std::string tableName;
		CHECK_NOTHROW( tableName=tableNames.at(0) );
		CHECK( tableName=="mytest" );
	}
} // end of 'Test cppsql::openDatabase'
