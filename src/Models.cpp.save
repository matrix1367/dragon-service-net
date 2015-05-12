#include "Models.h"
#include "NetworkManager.h"
#include "ManagerDataBase.h"
#include "MessageManager.h"

void CModels::Init(void) {
    //init all Manager
    CManagerDataBase::getInstance().Init();
    CNetworkManager::getInstance().Init();
}

std::list<SClient> CModels::GetClients()
{
    return CNetworkManager::getInstance().GetClients();
}

void CModels::RmoveMessage(unsigned int index)
{
    CMessage msq = CMessageManager::GetInstance().GetItemMessage(index);
    CManagerDataBase::getInstance().DeleteMessage(msq);
    CMessageManager::GetInstance().RemoveMessage(index);
}

