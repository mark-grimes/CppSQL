#include <Python.h>
#include "cppsql/tools.h"
#include "cppsql/IDatabase.h"

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

static PyObject* cppsql_executeOnDatabase( PyObject *self, PyObject *args, PyObject *keywords )
{
	const char* location;
	const char* sqlQuery;
	PyObject* pCallback;
	static const char* keywordList[]={"location","sqlQuery","callback",nullptr};
	if( !PyArg_ParseTupleAndKeywords( args, keywords, "ssO", const_cast<char**>(keywordList),
		&location, &sqlQuery, &pCallback ) ) return nullptr;
	if( !PyCallable_Check(pCallback) )
	{
		PyErr_SetString(PyExc_TypeError, "callback parameter must be callable");
		return nullptr;
	}
	// Not going to increment reference count on pCallback because I finish with
	// it before returning from this function.
	auto pDatabase=cppsql::openDatabase( location );
	if( pDatabase==nullptr )
	{
		PyErr_SetString(PyExc_ValueError, "database location is not valid");
		return nullptr;
	}
	PyObject* pArguments=PyTuple_New(3);
	PyObject* pNumFields=nullptr;
	PyObject* pReturnValue=nullptr;
	const char* const* oldValues=nullptr;
	const char* const* oldFieldNames=nullptr;
	pDatabase->execute( sqlQuery, [pCallback,pArguments,&pNumFields,&oldValues,&oldFieldNames,&pReturnValue](int numFields,const char* const values[],const char* const fieldNames[]){
		if( pNumFields==nullptr )
		{
			pNumFields = PyLong_FromLong(numFields);
			PyTuple_SetItem(pArguments, 0, pNumFields);
		}
		if( oldValues!=values )
		{
			PyObject* pValue = PyLong_FromSize_t((size_t) values);
			PyTuple_SetItem(pArguments, 1, pValue);
			oldValues=values;
		}
		if( oldFieldNames!=fieldNames )
		{
			PyObject* pValue = PyLong_FromSize_t((size_t) fieldNames);
			PyTuple_SetItem(pArguments, 2, pValue);
			oldFieldNames=fieldNames;
		}
		pReturnValue = PyObject_CallObject(pCallback, pArguments);
		if( pReturnValue==nullptr ) return false;

		bool returnValue=PyBool_Check(pReturnValue);
		Py_DECREF(pReturnValue);
		return returnValue;
	} );
	Py_DECREF(pArguments);
	// Pretty sure pNumFields is DECREF'd because it's owned by pArguments.
	// pReturnValue will have been DECREF'd in the lambda.
	if( pReturnValue==nullptr && pNumFields!=nullptr ) return nullptr; // if pNumFields is null no records were returned

	Py_RETURN_NONE;
}

static PyMethodDef SqlitedumpMethods[] = {
	{ "mySQLToSQLiteBackup", (PyCFunction)cppsql_mySQLToSQLiteBackup, METH_VARARGS | METH_KEYWORDS, "Backup a MySQL database to a SQLite file" },
	{ "executeOnDatabase", (PyCFunction)cppsql_executeOnDatabase, METH_VARARGS | METH_KEYWORDS, "Execute an SQL query string on a database" },
	{ NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initcppsql(void)
{
	(void) Py_InitModule( "cppsql", SqlitedumpMethods );
}
