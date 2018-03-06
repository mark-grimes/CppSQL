#include <iostream>
#include "cppsql/SomeClass.h"
#include "cppsql/version.h"

int main( int argc, char* argv[] )
{
	std::cout << "SkeletonCpp program with version " << cppsql::version::GitDescribe << "\n";
	cppsql::SomeClass instance;
	std::cout << "SomeClass::someValue returns '" << instance.someValue() << "'\n";
	return 0;
}
