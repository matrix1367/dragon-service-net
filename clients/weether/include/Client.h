#ifndef CCLIENT_H
#define CCLIENT_H
#include <windows.h>
#include <string>

class CClient
{
    public:
        CClient();
        virtual ~CClient();
        bool Init(void);

        void Connect();
        static DWORD WINAPI StaticThreadStart(void* Param) {
            CClient * This = (CClient*) Param;
            return This->ThreadStart();
        }
        bool Send(std::string msg);
    protected:
    private:
        DWORD WINAPI ThreadStart();

        char* m_ipAddres;
        int m_port;
        int m_hsock;
        DWORD m_threadID;
};

#endif // CCLIENT_H
