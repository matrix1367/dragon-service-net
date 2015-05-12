#ifndef CCLIENT_H
#define CCLIENT_H
#include <windows.h>
#include <string>


enum CMD { CMD_UNKNOWN, CMD_GET_NAME_CLIENT, CMD_SET_NAME_CLIENT, CMD_MESSAGE};

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
        std::string CommandCreate(CMD cmdID, std::string parm1);
        bool Send(std::string msg);
        bool IsConnect() { return m_isConnect;}
        bool SetIsConnect(bool val) { m_isConnect = val; return val;}
    protected:
    private:
        DWORD WINAPI ThreadStart();
        void CommandParser(std::string command);
        bool m_isConnect;
        char* m_ipAddres;
        int m_port;
        int m_hsock;
        DWORD m_threadID;
};

#endif // CCLIENT_H
