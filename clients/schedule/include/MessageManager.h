#ifndef CMESSAGEMANAGER_H
#define CMESSAGEMANAGER_H
#include <Message.h>
#include <queue>
#include <windows.h>

class CMessageManager
{
    public:
        static CMessageManager& GetInstance(){
            static CMessageManager instance;
            return instance;
        }

        void AddMessage(CMessage message);
        void AddMessage(const std::string name, const std::string description);

        void SendMessages();
        DWORD ThreadSendMessages();
        static DWORD WINAPI StaticThreadSendMessages(void* Param) {
            CMessageManager * This = (CMessageManager*) Param;
            return This->ThreadSendMessages();
        }
    protected:
    private:
        CMessageManager();
        virtual ~CMessageManager();
        void operator= (CMessageManager const&);
        DWORD m_threadID;
        bool isStopSendMessage;
        std::queue<CMessage> m_messages;
};

#endif // CMESSAGEMANAGER_H
