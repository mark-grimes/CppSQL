#ifndef INCLUDEGUARD_cppsql_version_h
#define INCLUDEGUARD_cppsql_version_h

#include <iosfwd>

namespace cppsql
{
	namespace version
	{
		extern const char* GitDescribe;
		extern const char GitHash[41];

	} // end of namespace version
} // end of namespace cppsql

#endif
