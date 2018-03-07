#ifndef INCLUDEGUARD_cppsql_IDatabase_h
#define INCLUDEGUARD_cppsql_IDatabase_h

#include <functional>
#include <vector>
#include <string>

namespace cppsql
{
	/** @brief Pure abstract interface for database classes
	 * @author Mark Grimes (mark.grimes@rymapt.com)
	 * @date 07/Mar/2018
	 */
	class IDatabase
	{
	public:
		~IDatabase() {}
		virtual std::vector<std::string> tableNames() const = 0;
		virtual void execute( const char* pCommand ) = 0;
		virtual void execute( const char* pCommand, std::function<bool(int,const char* const[],const char* const[])> resultsCallback ) = 0;
	};

} // end of namespace cppsql

#endif
