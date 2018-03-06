#ifndef INCLUDEGUARD_cppsql_SomeClass_h
#define INCLUDEGUARD_cppsql_SomeClass_h

#include <string>

namespace cppsql
{
	/** @brief A pointless class to make sure the bare skeleton project will compile
	 *
	 * make will fail if there is not at least one source file to compile. You should
	 * delete this file and replace with your own code.
	 */
	class SomeClass
	{
	public:
		std::string someValue() const;
	};

} // end of namespace cppsql

#endif
