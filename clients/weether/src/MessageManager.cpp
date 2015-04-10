#include "MessageManager.h"
#include "Models.h"
#include "CDLog.h"


CMessageManager::CMessageManager()
{
    isStopSendMessage = true;
    this->SendMessages();
}

CMessageManager::~CMessageManager()
{
    //dtor
}

void CMessageManager::AddMessage(const CMessage& message)
{
    m_messages.push(message);
}

void CMessageManager::AddMessage(const std::string name, const std::string description)
{
    CMessage message(name, description);
    m_messages.push(message);
}

void CMessageManager::SendMessages()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "" );
    CreateThread(0,0,StaticThreadSendMessages, (void*)this , 0,&m_threadID);
}

DWORD CMessageManager::ThreadSendMessages()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "" );
    while(isStopSendMessage) {
        if (m_messages.size() > 0) {
            CDLog::Write( __FUNCTION__ , __LINE__, Info, "Kolejka wiedomosci zawiera " + CDLog::ToString(m_messages.size()) + " niewyslanych wiadomosci." );
            CModels::getInstance().GetClient().Send(CModels::getInstance().GetClient().CommandCreate(CMD_MESSAGE,m_messages.front().ConvertObjToStr()));
            m_messages.pop();
        }
        Sleep(3000);
    }
    return 0;
}
