#ifndef INCLUDEGUARD_cppsql_SQLiteStatement_h
#define INCLUDEGUARD_cppsql_SQLiteStatement_h

#include <string>
#include <sqlite3.h>

namespace cppsql
{
	class SQLiteStatement
	{
	public:
		~SQLiteStatement();
		template<typename T> void bind( const T value, int position );
		template<typename T> void bind( const T value, int position, int size );
		void reset();
		void step();
	private:
		friend class SQLiteDatabase;
		SQLiteStatement( sqlite3_stmt* pStatement, sqlite3* pDatabase );
		sqlite3_stmt* pStatement_;
		sqlite3* pDatabase_; // required to get error messages
	};

} // end of namespace cppsql

template<> void cppsql::SQLiteStatement::bind( const double value, int position );
template<> void cppsql::SQLiteStatement::bind( const float value, int position );
template<> void cppsql::SQLiteStatement::bind( const int value, int position );
template<> void cppsql::SQLiteStatement::bind( const short value, int position );
template<> void cppsql::SQLiteStatement::bind( const char* value, int position );
/// Note that this function is for MYSQL_TYPE_TINY, i.e. a single byte, not a pointer to an array of bytes
template<> void cppsql::SQLiteStatement::bind( const signed char value, int position );
template<> void cppsql::SQLiteStatement::bind( const void* value, int position, int size );

#endif
