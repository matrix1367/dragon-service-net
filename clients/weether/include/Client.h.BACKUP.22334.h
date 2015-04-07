#ifndef CCLIENT_H
#define CCLIENT_H
#include <windows.h>
<<<<<<< HEAD
#include <string>
=======
>>>>>>> 63cdba9a2a1d0064bb5e4f6ece93818a8ff79ba2

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
<<<<<<< HEAD
        bool Send(std::string msg);
=======
>>>>>>> 63cdba9a2a1d0064bb5e4f6ece93818a8ff79ba2
    protected:
    private:
        DWORD WINAPI ThreadStart();

        char* m_ipAddres;
        int m_port;
        int m_hsock;
        DWORD m_threadID;
};

#endif // CCLIENT_H
