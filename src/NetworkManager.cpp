#include "NetworkManager.h"

CNetworkManager::CNetworkManager()
{
    //ctor
}

CNetworkManager::~CNetworkManager()
{
    //dtor
}

bool CNetworkManager::Init()
{
    if(server.Init()) {
        server.Start();
        return true;
    }
    return false;
}

std::list<SClient> CNetworkManager::GetClients()
{
    return server.GetClients();
}
