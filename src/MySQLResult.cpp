#include "cppsql/MySQLResult.h"

cppsql::MySQLResult::MySQLResult( MYSQL_RES* pResult )
	: pResult_(pResult)
{
	if(pResult_==nullptr) throw std::runtime_error("Got null result pointer");
}

cppsql::MySQLResult::~MySQLResult()
{
	mysql_free_result(pResult_);
}

int cppsql::MySQLResult::numberOfFields()
{
	return mysql_num_fields(pResult_);
}

void cppsql::MySQLResult::showResults( std::function<bool(int,int,char*,char*)> resultsCallback )
{
	int num_fields=numberOfFields();
	MYSQL_FIELD* fields=mysql_fetch_fields(pResult_);

	MYSQL_ROW row;
	bool keepGoing=true;
	while( (row=mysql_fetch_row(pResult_)) && keepGoing )
	{
		for( int fieldIndex=0; fieldIndex<num_fields && keepGoing; ++fieldIndex )
		{
			keepGoing=resultsCallback( fieldIndex, num_fields, row[fieldIndex], fields[fieldIndex].name );
		}
	}
}
