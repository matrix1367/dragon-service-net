#ifndef CDATABASESQLITE_H
#define CDATABASESQLITE_H
#include "DataBase.h"
#include "sqlite3.h"

class CDataBaseSQLITE : public CDataBase
{
    public:
        CDataBaseSQLITE(std::string namefile);
        virtual ~CDataBaseSQLITE();
        virtual bool Open();
        virtual bool Close();
        virtual void Exec(std::string sql);
    protected:
    private:
        std::string m_namefile;
        sqlite3 *db;
};

#endif // CDATABASESQLITE_H
