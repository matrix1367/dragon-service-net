#ifndef CMANAGERDATABASE_H
#define CMANAGERDATABASE_H
#include <DataBase.h>
#include "Message.h"

class CManagerDataBase
{
public:

    static CManagerDataBase& getInstance()
    {
        static CManagerDataBase instance;
        return instance;
    }
    void Init();
    void AddMessage(const CMessage& msg);
    void DeleteMessage(const CMessage& msg);
    std::vector<std::vector<std::string> > GetAllMessage();
protected:
private:
    CManagerDataBase();
    virtual ~CManagerDataBase();
    void CreateStructurTable();
    IDataBase* db;
};

#endif // CMANAGERDATABASE_H
