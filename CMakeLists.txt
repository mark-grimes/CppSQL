#
# CMake file to build CppSQL.
# Mark Grimes (mark.grimes@rymapt.com)
# 06/Mar/2018
# copyright 2018 Rymapt Ltd
# Licence to be decided
#

project( CppSQL )

cmake_minimum_required(VERSION 2.8)

if( NOT MSVC ) # Microsoft Visual Studio is C++11 by default and doesn't recognise this flag
	add_definitions( "-std=c++11" )
endif()


# Create the file that has the version information and git hash
execute_process( COMMAND git describe --dirty --always --tags WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE CppSQL_GIT_DESCRIBE OUTPUT_STRIP_TRAILING_WHITESPACE )
execute_process( COMMAND git log -1 --pretty=format:%H WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE CppSQL_GIT_HASH OUTPUT_STRIP_TRAILING_WHITESPACE )
add_custom_target( CreateVersionFile ALL ${CMAKE_COMMAND} -E touch "${CMAKE_SOURCE_DIR}/src/version.cpp.in" ) # Make sure the git hash is always checked (not just at configure time)
configure_file( "${CMAKE_SOURCE_DIR}/src/version.cpp.in" "${CMAKE_BINARY_DIR}/src/version.cpp" )


include_directories( "${CMAKE_SOURCE_DIR}/include" )
aux_source_directory( "${CMAKE_SOURCE_DIR}/src" library_sources )
list( REMOVE_ITEM library_sources "${CMAKE_SOURCE_DIR}/src/main.cpp" ) # main.cpp belongs only in the executable

# Add dependencies
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
find_package( MYSQL REQUIRED )
include_directories( ${MYSQL_INCLUDE_DIRS} )

add_library( ${PROJECT_NAME}_LIB STATIC ${library_sources} "${CMAKE_BINARY_DIR}/src/version.cpp" )
target_link_libraries( ${PROJECT_NAME}_LIB "sqlite3" ${MYSQL_LIBRARIES} )
add_executable( ${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/src/main.cpp" )
target_link_libraries( ${PROJECT_NAME} ${PROJECT_NAME}_LIB )
## Uncomment this if you get "undefined reference to `pthread_create'" when using std::thread
#if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
#	target_link_libraries( ${PROJECT_NAME}_LIB "pthread" )
#endif()

install( TARGETS ${PROJECT_NAME}_LIB DESTINATION lib )
install( DIRECTORY "${CMAKE_SOURCE_DIR}/include/" DESTINATION include )

option( BUILD_UNITTESTS "Build unit tests" ON )
message( STATUS "BUILD_UNITTESTS: ${BUILD_UNITTESTS}" )
if( BUILD_UNITTESTS )
	# Fix the test configuration file to have the correct paths
	configure_file( "${PROJECT_SOURCE_DIR}/tests/src/testinputs.cpp.in" "${PROJECT_BINARY_DIR}/tests/src/testinputs.cpp" @ONLY )
	include_directories( "${CMAKE_SOURCE_DIR}/tests/include" )
	aux_source_directory( "${CMAKE_SOURCE_DIR}/tests/src" unittests_sources )
	add_executable( ${PROJECT_NAME}Tests ${unittests_sources} "${PROJECT_BINARY_DIR}/tests/src/testinputs.cpp" )
	target_link_libraries( ${PROJECT_NAME}Tests ${PROJECT_NAME}_LIB )
endif()

option( BUILD_PYTHON "Build python module" ON )
message( STATUS "BUILD_PYTHON: ${BUILD_PYTHON}" )
if( BUILD_PYTHON )
	# Fix the test configuration file to have the correct paths
	configure_file( "${PROJECT_SOURCE_DIR}/src/python/setup.py.in" "${PROJECT_BINARY_DIR}/python/src/setup.py" @ONLY )
	add_custom_target( CreatePythonModule ALL "python" "${PROJECT_BINARY_DIR}/python/src/setup.py" "build" DEPENDS ${PROJECT_NAME}_LIB )
	if(APPLE)
		# Mac OS X will give "unsafe use of relative rpath..." errors, so need to change the
		# relative path to an absolute path for the MySQL library.
		get_filename_component( LIB_DIRECTORY ${MYSQL_LIBRARIES} DIRECTORY )
		get_filename_component( LIB_NAME ${MYSQL_LIBRARIES} NAME )
		# 'readlink' on OS X doesn't have the '-f' option, so need to call it recursively
		set( POTENTIAL_LIB_NAME "${LIB_NAME}" )
		while( POTENTIAL_LIB_NAME ) # Loop until POTENTIAL_LIB_NAME is an empty string
			execute_process( COMMAND "readlink" "${LIB_DIRECTORY}/${LIB_NAME}" OUTPUT_VARIABLE POTENTIAL_LIB_NAME OUTPUT_STRIP_TRAILING_WHITESPACE )
			if( POTENTIAL_LIB_NAME )
				set( LIB_NAME ${POTENTIAL_LIB_NAME} )
			endif()
		endwhile()

		# This is the important bit, everything else was to get the variables for this call
		add_custom_target( FixPythonModuleRPath ALL
			"install_name_tool" "-change" "${LIB_NAME}" "${LIB_DIRECTORY}/${LIB_NAME}" "${PROJECT_BINARY_DIR}/build/lib*/cppsql.so"
			DEPENDS CreatePythonModule )
	endif()
endif()
