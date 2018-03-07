#include "cppsql/MySQLResult.h"
#include <vector>

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

void cppsql::MySQLResult::showResults( std::function<bool(int,const char* const[],const char* const[])> resultsCallback )
{
	int num_fields=numberOfFields();
	MYSQL_FIELD* fields=mysql_fetch_fields(pResult_);

	std::vector<const char*> fieldNames(num_fields);
	for( size_t index=0; index<num_fields; ++index ) fieldNames[index]=fields[index].name;

	MYSQL_ROW row;
	bool keepGoing=true;
	while( (row=mysql_fetch_row(pResult_)) && keepGoing )
	{
		keepGoing=resultsCallback( num_fields, row, fieldNames.data() );
	}
}
