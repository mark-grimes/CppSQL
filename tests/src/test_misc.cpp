#include "cppsql/misc.h"
#include <string>
#include "catch.hpp"

SCENARIO( "Test that the cppsql::parseSQLAlchemyURL function works properly", "[misc]" )
{
	GIVEN("Some blank strings")
	{
		bool result;
		std::string mysqlHost, mysqlUsername, mysqlPassword, mysqlDatabase, sqliteFilename;
		WHEN( "Testing parsing an empty string" )
		{
			CHECK_NOTHROW( result=cppsql::parseSQLAlchemyURL("", mysqlHost, mysqlUsername, mysqlPassword, mysqlDatabase, sqliteFilename) );
			CHECK( result==false );
		}
		WHEN( "Testing SQLite memory database parsing" )
		{
			CHECK_NOTHROW( result=cppsql::parseSQLAlchemyURL("sqlite://", mysqlHost, mysqlUsername, mysqlPassword, mysqlDatabase, sqliteFilename) );
			CHECK( result==true );
			CHECK( mysqlHost.empty() );
			CHECK( sqliteFilename==":memory:" );
		}
		WHEN( "Testing SQLite parsing" )
		{
			CHECK_NOTHROW( result=cppsql::parseSQLAlchemyURL("sqlite:///file.db", mysqlHost, mysqlUsername, mysqlPassword, mysqlDatabase, sqliteFilename) );
			CHECK( result==true );
			CHECK( mysqlHost.empty() );
			CHECK( sqliteFilename=="file.db" );
		}
		WHEN( "Testing SQLite parsing" )
		{
			CHECK_NOTHROW( result=cppsql::parseSQLAlchemyURL("sqlite://file.db", mysqlHost, mysqlUsername, mysqlPassword, mysqlDatabase, sqliteFilename) );
			CHECK( result==false );
			CHECK( sqliteFilename.empty() );
		}
		WHEN( "Testing SQLite parsing" )
		{
			CHECK_NOTHROW( result=cppsql::parseSQLAlchemyURL("sqlite:///", mysqlHost, mysqlUsername, mysqlPassword, mysqlDatabase, sqliteFilename) );
			CHECK( result==false );
			CHECK( sqliteFilename.empty() );
		}
		WHEN( "Testing SQLite parsing" )
		{
			CHECK_NOTHROW( result=cppsql::parseSQLAlchemyURL("sqlite:///someRelative/directory/file.db", mysqlHost, mysqlUsername, mysqlPassword, mysqlDatabase, sqliteFilename) );
			CHECK( result==true );
			CHECK( mysqlHost.empty() );
			CHECK( sqliteFilename=="someRelative/directory/file.db" );
		}
		WHEN( "Testing SQLite parsing" )
		{
			CHECK_NOTHROW( result=cppsql::parseSQLAlchemyURL("sqlite:////someAbsolute/directory/file.db", mysqlHost, mysqlUsername, mysqlPassword, mysqlDatabase, sqliteFilename) );
			CHECK( result==true );
			CHECK( mysqlHost.empty() );
			CHECK( sqliteFilename=="/someAbsolute/directory/file.db" );
		}
		WHEN( "Testing MySQL parsing" )
		{
			CHECK_NOTHROW( result=cppsql::parseSQLAlchemyURL("mysql://username:password@host/database", mysqlHost, mysqlUsername, mysqlPassword, mysqlDatabase, sqliteFilename) );
			CHECK( result==true );
			CHECK( mysqlUsername=="username" );
			CHECK( mysqlPassword=="password" );
			CHECK( mysqlHost=="host" );
			CHECK( mysqlDatabase=="database" );
			CHECK( sqliteFilename.empty() );
		}
		WHEN( "Testing MySQL parsing" )
		{
			CHECK_NOTHROW( result=cppsql::parseSQLAlchemyURL("mysql://username@host/database", mysqlHost, mysqlUsername, mysqlPassword, mysqlDatabase, sqliteFilename) );
			CHECK( result==true );
			CHECK( mysqlUsername=="username" );
			CHECK( mysqlPassword.empty() );
			CHECK( mysqlHost=="host" );
			CHECK( mysqlDatabase=="database" );
			CHECK( sqliteFilename.empty() );
		}
		WHEN( "Testing MySQL parsing" )
		{
			CHECK_NOTHROW( result=cppsql::parseSQLAlchemyURL("mysql://host/database", mysqlHost, mysqlUsername, mysqlPassword, mysqlDatabase, sqliteFilename) );
			CHECK( result==true );
			CHECK( mysqlUsername.empty() );
			CHECK( mysqlPassword.empty() );
			CHECK( mysqlHost=="host" );
			CHECK( mysqlDatabase=="database" );
			CHECK( sqliteFilename.empty() );
		}
	}
} // end of 'Test ... cppsql::parseSQLAlchemyURL'
