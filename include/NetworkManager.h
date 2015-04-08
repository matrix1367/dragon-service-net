#ifndef CNETWORKMANAGER_H
#define CNETWORKMANAGER_H
#include "Server.h"

class CNetworkManager
{
    public:
        static CNetworkManager& getInstance() {
            static CNetworkManager instance;
            return instance;
        }
        bool Init();
        std::list<SClient> GetClients();
    protected:
    private:
        CNetworkManager();
        virtual ~CNetworkManager();
        CServer server;
};

#endif // CNETWORKMANAGER_H
