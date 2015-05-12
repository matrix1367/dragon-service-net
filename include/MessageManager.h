#ifndef CMESSAGEMANAGER_H
#define CMESSAGEMANAGER_H
#include "Message.h"

#include <string>
#include <list>

class CMessageManager
{
    public:
        static CMessageManager& GetInstance(){
            static CMessageManager instance;
            return instance;
        }

        void AddMessage( std::string name,  std::string description,  int id);
        void AddMessage(const CMessage& message);

        std::list<CMessage> GetMessages();

        CMessage GetItemMessage(unsigned int index);
        void RemoveMessage(unsigned int index);
    protected:
    private:
        CMessageManager();
        virtual ~CMessageManager();
        void operator= (CMessageManager const&);
        std::list<CMessage> m_messages;
};

#endif // CMESSAGEMANAGER_H
