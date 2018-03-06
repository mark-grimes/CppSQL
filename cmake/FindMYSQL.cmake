find_path( MYSQL_INCLUDE_DIR
	NAMES "mysql.h"
	PATHS "${MYSQL_ROOT}/include/mysql"
		  "${MYSQL_ROOT}/include/mariadb"
		  "/usr/include/mysql"
		  "/usr/local/include/mysql"
		  "/usr/mysql/include/mysql" )

find_library( MYSQL_LIBRARY
	NAMES "mysql" "mariadb"
	PATHS "${MYSQL_ROOT}/lib/mysql"
		  "${MYSQL_ROOT}/lib/mariadb"
		  "/lib/mysql"
		  "/lib64/mysql"
		  "/usr/lib/mysql"
		  "/usr/lib64/mysql"
		  "/usr/local/lib/mysql"
		  "/usr/local/lib64/mysql"
		  "/usr/mysql/lib/mysql"
		  "/usr/mysql/lib64/mysql" )

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( MYSQL DEFAULT_MSG MYSQL_LIBRARY MYSQL_INCLUDE_DIR )

set( MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR} )
set( MYSQL_LIBRARIES ${MYSQL_LIBRARY} )

mark_as_advanced( MYSQL_INCLUDE_DIR MYSQL_LIBRARY )
