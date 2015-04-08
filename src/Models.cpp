#include "Models.h"
#include "NetworkManager.h"

void CModels::Init(void) {
    //init all Manager
    CNetworkManager::getInstance().Init();
}

std::list<SClient> CModels::GetClients()
{
    return CNetworkManager::getInstance().GetClients();
}
