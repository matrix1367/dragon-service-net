#ifndef CSERVER_H
#define CSERVER_H
#include <string>
#include <windows.h>
#include <list>

typedef struct TClient {
    std::string GetSrtIP() {
        return ip;
    }
    std::string GetStrSocketHandle() {
        char buffor[10];
        itoa(*socketHandle, buffor, 10);
        return std::string(buffor);
    }
    std::string GetStrName() {
        return name;
    }
    std::string GetStrStatus() {
        return "Connect";
    }
    int* socketHandle;
    std::string ip;
    std::string name;
} SClient;

class CServer
{
    public:
        CServer();
        virtual ~CServer();
        bool Init(void);
        DWORD WINAPI ThreadStart();
        void Start();
        static DWORD WINAPI StaticThreadStart(void* Param) {
            CServer * This = (CServer*) Param;
            return This->ThreadStart();
        }

        DWORD WINAPI ThreadConnectClient(void* lp);
        static DWORD WINAPI StaticThreadConnectClient(void* Param) {
            CServer * This = (CServer*) Param;
            return This->ThreadConnectClient(This->GetTmpParam());
        }
        void SetTmpParam(void* val) {
            m_TmpParam = val;
        }
        void* GetTmpParam() {
            return m_TmpParam;
        }

        std::list<SClient> GetClients();
    protected:
    private:
        void* m_TmpParam;

        std::string m_adresIP;
        int m_port;
        int m_hsock;
        DWORD m_threadID;
        std::list<SClient> m_clients;
};

#endif // CSERVER_H
