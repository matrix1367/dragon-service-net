#ifndef CSERVER_H
#define CSERVER_H
#include <string>
#include <windows.h>

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
    protected:
    private:
        std::string m_adresIP;
        int m_port;
        int m_hsock;
        DWORD m_threadID;
};

#endif // CSERVER_H
