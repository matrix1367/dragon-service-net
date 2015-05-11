#ifndef API_ENGINEDATABASE_H_INCLUDED
#define API_ENGINEDATABASE_H_INCLUDED
#include "DataBase.h"

enum TypeDB { TYPEDB_SQLITE3 , TYPEDB_MYSQL, TYPEDB_POSTGRES };

enum DatatypesDB { DATATYPES_INTEGER, DATATYPES_REAL, DATATYPES_TEXT, DATATYPES_BLOB};


IDataBase* CreateEngine(TypeDB typeDB, const char* fileName, const char* host, const char* user, const char* password);


#endif // API_ENGINEDATABASE_H_INCLUDED
