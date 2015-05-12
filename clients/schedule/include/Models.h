#ifndef CMODELS_H
#define CMODELS_H
#include "Client.h"
#include "Convert.h"



class CModels
{
    public:
        static CModels& getInstance() {
            static CModels instance;
            return instance;
        }
         static DWORD WINAPI StaticThreadWaitingForServer(void* Param) {
            CModels * This = (CModels*) Param;
            return This->ThreadWaitingForServer();
        }

        void Init(void);
        CClient& GetClient();
        void WaitingForServer();

        bool IsConnect() { return client.IsConnect(); }

    protected:
    private:
        CModels();
        virtual ~CModels() {}
        void operator= (CModels const&);
        DWORD m_WaitingForServerThreadID;
        HANDLE  m_handlThread;

        DWORD WINAPI ThreadWaitingForServer();

        CClient client;
        bool m_isWaitingForServer;


};

#endif // CMODELS_H
