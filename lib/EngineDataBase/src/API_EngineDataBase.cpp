#include "API_EngineDataBase.h"
#include "DataBaseSQLITE.h"

IDataBase* CreateEngine(TypeDB typeDB, const char* fileName, const char* host, const char* user, const char* password)
{
    switch (typeDB)
    {
        case TYPEDB_SQLITE3:
        {
                CDataBaseSQLITE *dbs = new CDataBaseSQLITE(fileName);
                return dbs;
        }
        default:
        {
            return NULL;

        }
    }
}