#include "cppsql/SomeClass.h"
#include "catch.hpp"

SCENARIO( "Test that cppsql/SomeClass gives the correct values", "[SomeClass]" )
{
	GIVEN( "An instance of SomeClass" )
	{
		cppsql::SomeClass anInstance;

		WHEN( "Testing the result of someResult" )
		{
			CHECK( anInstance.someValue() == "It Works!" );
		}
	} // end of 'GIVEN "An instance of SomeClass"'
} // end of 'SCENARIO ... SomeClass'
