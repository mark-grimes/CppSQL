#include <Python.h>
#include "cppsql/tools.h"

static PyObject* cppsql_mySQLToSQLiteBackup( PyObject *self, PyObject *args, PyObject *keywords )
{
	// Argument defaults
	const char* host="localhost";
	const char* user="root";
	const char* password="root";
	const char* database="ethoscope_db";
	const char* outputFile="testdb.sqlite";
	static const char* keywordList[]={"host","user","password","database","outputFile",nullptr};
	// Parse the arguments
	if( !PyArg_ParseTupleAndKeywords( args, keywords, "|sssss", const_cast<char**>(keywordList),
		&host, &user, &password, &database, &outputFile ) ) return nullptr;

	// Call the C++ function
	cppsql::mySQLToSQLiteBackup( host, user, password, database, outputFile );
	Py_RETURN_NONE;
}

static PyMethodDef SqlitedumpMethods[] = {
	{ "mySQLToSQLiteBackup", (PyCFunction)cppsql_mySQLToSQLiteBackup, METH_VARARGS | METH_KEYWORDS, "Backup a MySQL database to a SQLite file" },
	{ NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initcppsql(void)
{
	(void) Py_InitModule( "cppsql", SqlitedumpMethods );
}
