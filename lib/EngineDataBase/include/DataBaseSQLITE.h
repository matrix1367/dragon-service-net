#ifndef CDATABASESQLITE_H
#define CDATABASESQLITE_H
#include "DataBase.h"
#include "sqlite3.h"
#include <vector>

class CDataBaseSQLITE : public IDataBase
{
    public:
        CDataBaseSQLITE(std::string namefile);
        virtual ~CDataBaseSQLITE();
         bool Open();
         bool Close();
         bool Query(std::vector<std::vector<std::string> >& results, const std::string& message);
    protected:
    private:
        std::string m_namefile;
        sqlite3 *db;
};

#endif // CDATABASESQLITE_H
