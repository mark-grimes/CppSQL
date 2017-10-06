#! /bin/bash
#
# Creates a C++ cmake project with test directory etcetera
# Mark Grimes 06/Oct/2017
# copyright Mark Grimes 2017
# Released under the MIT licence (see https://opensource.org/licenses/MIT)
#

if [ $# -ne 1 ]; then
	echo "You should specify the project name as the only argument. Don't use spaces or it'll probably end in tears." >&2
	exit
fi

REPLACEME_PROJECT_NAME="$1"
REPLACEME_PROJECT_NAMESPACE=${REPLACEME_PROJECT_NAME,,} # Convert project name to lower case
REPLACEME_PROJECT_TESTS_NAMESPACE="${REPLACEME_PROJECT_NAMESPACE}tests"

mv "include/REPLACEME_PROJECT_NAMESPACE" "include/${REPLACEME_PROJECT_NAMESPACE}"
mv "tests/include/REPLACEME_PROJECT_TESTS_NAMESPACE" "tests/include/${REPLACEME_PROJECT_TESTS_NAMESPACE}"
sed -i -e s:"REPLACEME_PROJECT_NAMESPACE":"${REPLACEME_PROJECT_NAMESPACE}":g \
       -e s:"REPLACEME_PROJECT_NAME":"${REPLACEME_PROJECT_NAME}":g \
       -e s:"REPLACEME_PROJECT_TESTS_NAMESPACE":"${REPLACEME_PROJECT_TESTS_NAMESPACE}":g \
           "tests/include/${REPLACEME_PROJECT_TESTS_NAMESPACE}/testinputs.h" \
           "tests/src/testinputs.cpp.in" \
           "include/${REPLACEME_PROJECT_NAMESPACE}/version.h" \
           "src/version.cpp.in" \
           "tests/src/test_version.cpp" \
           "include/${REPLACEME_PROJECT_NAMESPACE}/SomeClass.h" \
           "src/SomeClass.cpp" \
           "tests/src/test_SomeClass.cpp" \
           "CMakeLists.txt"
