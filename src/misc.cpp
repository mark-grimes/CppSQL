#include "cppsql/misc.h"
#include <stdexcept>

const char* cppsql::mySqlTypeString( const MYSQL_FIELD& field )
{
	switch( field.type )
	{
		case MYSQL_TYPE_TINY: return "TINY INT";
		case MYSQL_TYPE_SHORT: return "SHORT";
		case MYSQL_TYPE_INT24: return "LONG INT";
		case MYSQL_TYPE_LONG: return "LONG";
		case MYSQL_TYPE_LONGLONG: return "LONGLONG";
		case MYSQL_TYPE_FLOAT: return "FLOAT";
		case MYSQL_TYPE_DOUBLE: return "DOUBLE";
		case MYSQL_TYPE_NEWDECIMAL: return "NEWDECIMAL";
		case MYSQL_TYPE_TIME: return "TIME";
		case MYSQL_TYPE_DATE: return "DATE";
		case MYSQL_TYPE_DATETIME: return "DATETIME";
		case MYSQL_TYPE_TIMESTAMP: return "TIMESTAMP";
		case MYSQL_TYPE_STRING: return "STRING";
		case MYSQL_TYPE_VAR_STRING: return "VAR_STRING";
		case MYSQL_TYPE_TINY_BLOB: return "TINY_BLOB";
		case MYSQL_TYPE_BLOB: return "BLOB";
		case MYSQL_TYPE_MEDIUM_BLOB: return "MEDIUM_BLOB";
		case MYSQL_TYPE_LONG_BLOB: return "LONG_BLOB";
		case MYSQL_TYPE_BIT: return "BIT";
		default: throw std::runtime_error("Unknown type (sqlTypeString)");
	}
}

void cppsql::bindMySQLToSQLite( const MYSQL_BIND& bind, SQLiteStatement& statement, const int fieldNumber )
{
	switch( bind.buffer_type )
	{
		case MYSQL_TYPE_TINY: statement.bind( *(signed char*)bind.buffer, fieldNumber+1 ); break;
		case MYSQL_TYPE_SHORT: statement.bind( *(short int*)bind.buffer, fieldNumber+1 ); break;
		case MYSQL_TYPE_INT24: statement.bind( *(int*)bind.buffer, fieldNumber+1 ); break;
		case MYSQL_TYPE_LONG: statement.bind( *(int*)bind.buffer, fieldNumber+1 ); break;
		//case MYSQL_TYPE_LONGLONG: statement.bind( *(long long int*)bind.buffer, fieldNumber ); break;
		case MYSQL_TYPE_FLOAT: statement.bind( *(float*)bind.buffer, fieldNumber+1 ); break;
		case MYSQL_TYPE_DOUBLE: statement.bind( *(double*)bind.buffer, fieldNumber+1 ); break;
		//case MYSQL_TYPE_NEWDECIMAL: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		//case MYSQL_TYPE_TIME: stream << (MYSQL_TIME*)buffer.bind_.buffer; break;
		//case MYSQL_TYPE_DATE: stream << (MYSQL_TIME*)buffer.bind_.buffer; break;
		//case MYSQL_TYPE_DATETIME: stream << (MYSQL_TIME*)buffer.bind_.buffer; break;
		//case MYSQL_TYPE_TIMESTAMP: stream << (MYSQL_TIME*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_STRING: statement.bind( (const char*)bind.buffer, fieldNumber+1 ); break;
		case MYSQL_TYPE_VAR_STRING: statement.bind( (const char*)bind.buffer, fieldNumber+1 ); break;
		//case MYSQL_TYPE_TINY_BLOB: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_BLOB: statement.bind( (const void*)bind.buffer, fieldNumber+1, bind.buffer_length ); break;
		//case MYSQL_TYPE_MEDIUM_BLOB: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		//case MYSQL_TYPE_LONG_BLOB: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		//case MYSQL_TYPE_BIT: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		default: throw std::runtime_error("Unable to bind SQLite parameter to MySQL result "+std::string(cppsql::mySQLTypeAsString(bind.buffer_type)));
	}
}

const char* cppsql::mySQLTypeAsString( int type )
{
	switch( type )
	{
		case MYSQL_TYPE_TINY: return "MYSQL_TYPE_TINY";
		case MYSQL_TYPE_SHORT: return "MYSQL_TYPE_SHORT";
		case MYSQL_TYPE_INT24: return "MYSQL_TYPE_INT24";
		case MYSQL_TYPE_LONG: return "MYSQL_TYPE_LONG";
		case MYSQL_TYPE_LONGLONG: return "MYSQL_TYPE_LONGLONG";
		case MYSQL_TYPE_FLOAT: return "MYSQL_TYPE_FLOAT";
		case MYSQL_TYPE_DOUBLE: return "MYSQL_TYPE_DOUBLE";
		case MYSQL_TYPE_NEWDECIMAL: return "MYSQL_TYPE_NEWDECIMAL";
		case MYSQL_TYPE_TIME: return "MYSQL_TYPE_TIME";
		case MYSQL_TYPE_DATE: return "MYSQL_TYPE_DATE";
		case MYSQL_TYPE_DATETIME: return "MYSQL_TYPE_DATETIME";
		case MYSQL_TYPE_TIMESTAMP: return "MYSQL_TYPE_TIMESTAMP";
		case MYSQL_TYPE_STRING: return "MYSQL_TYPE_STRING";
		case MYSQL_TYPE_VAR_STRING: return "MYSQL_TYPE_VAR_STRING";
		case MYSQL_TYPE_TINY_BLOB: return "MYSQL_TYPE_TINY_BLOB";
		case MYSQL_TYPE_BLOB: return "MYSQL_TYPE_BLOB";
		case MYSQL_TYPE_MEDIUM_BLOB: return "MYSQL_TYPE_MEDIUM_BLOB";
		case MYSQL_TYPE_LONG_BLOB: return "MYSQL_TYPE_LONG_BLOB";
		case MYSQL_TYPE_BIT: return "MYSQL_TYPE_BIT";
		default: return "<unknown>";
	}
}

bool cppsql::parseSQLAlchemyURL( const char* url, std::string& mysqlHost, std::string& mysqlUsername, std::string& mysqlPassword, std::string& mysqlDatabase, std::string& sqliteFilename )
{
	return cppsql::parseSQLAlchemyURL( std::string(url), mysqlHost, mysqlUsername, mysqlPassword, mysqlDatabase, sqliteFilename );
}

bool cppsql::parseSQLAlchemyURL( const std::string& url, std::string& mysqlHost, std::string& mysqlUsername, std::string& mysqlPassword, std::string& mysqlDatabase, std::string& sqliteFilename )
{
	if( url.find("mysql://")==0 )
	{
		return false;
	}
	else if( url.find("sqlite://")==0 )
	{
		if( url.size()==9 ) // url is exactly "sqlite://"
		{
			sqliteFilename=":memory:";
			return true;
		}

		if( url.size()<11 ) return false; // need to have at least '/'+one character

		if( url[9]!='/' ) return false;

		sqliteFilename=url.substr(10);
		return true;
	}
	else return false;
}
