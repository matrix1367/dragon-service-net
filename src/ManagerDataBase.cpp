#include "ManagerDataBase.h"
#include <API_EngineDataBase.h>
#include <string>

CManagerDataBase::CManagerDataBase()
{
    //used sqlite
}

void CManagerDataBase::Init()
{
    db = CreateEngine(TYPEDB_SQLITE3, "serverMesageDB.dat", "", "", "");
    db->Open();
    CreateStructurTable();
}

CManagerDataBase::~CManagerDataBase()
{
    db->Close();
    if (db) delete db;
}

void CManagerDataBase::CreateStructurTable()
{
    if(db)
    {
        std::vector<std::vector<std::string> > result;
        const std::string creatMessageTable = "CREATE TABLE Message (id INTEGER PRIMARY KEY AUTOINCREMENT, idClient TEXT, name TEXT, description TEXT);";
        db->Query(result, creatMessageTable);
    }
}

void CManagerDataBase::AddMessage(const CMessage& msg)
{
    if(db)
    {
        std::vector<std::vector<std::string> > result;
        const std::string insertMessageTable = "INSERT INTO Message ( `idClient`, `name`, `description`) VALUES ( '" + msg.GetStrID() + "', '" + msg.GetName() + "', '" + msg.GetDescription() + "' );";
        db->Query(result,insertMessageTable);
    }
}


std::vector<std::vector<std::string> > CManagerDataBase::GetAllMessage()
{
    std::vector<std::vector<std::string> > result;
    if(db)
    {
        const std::string insertMessageTable = "SELECT * FROM `Message` ;";
        db->Query(result,insertMessageTable);
    }
    return result;
}

void CManagerDataBase::DeleteMessage(const CMessage& msg)
{
    if(db)
    {
        std::vector<std::vector<std::string> > result;
        const std::string deleteMessageTable = "DELETE FROM `Message`  WHERE idClient='" + msg.GetStrID() + "' AND  name='"+ msg.GetName() +"' AND description='"+ msg.GetDescription() +"';";
        db->Query(result,deleteMessageTable);
    }
}

