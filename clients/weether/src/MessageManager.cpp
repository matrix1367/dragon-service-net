#include "MessageManager.h"
#include "Models.h"
#include "CDLog.h"

#define TIME_WAIT_FOR_NEXT_SEND 3000


CMessageManager::CMessageManager()
{
    isStopSendMessage = true;
    this->SendMessages();
}

CMessageManager::~CMessageManager()
{
    //dtor
}

void CMessageManager::AddMessage(CMessage message)
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
        if (CModels::getInstance().IsConnect())
        {
            if (m_messages.size() > 0 ) {
                CDLog::Write( __FUNCTION__ , __LINE__, Info, "Kolejka wiedomosci zawiera " + CDLog::ToString(m_messages.size()) + " niewyslanych wiadomosci, wysy�anie..." );
                for (unsigned int i=0; i< m_messages.size(); i++) {
                    if(CModels::getInstance().GetClient().Send(CModels::getInstance().GetClient().CommandCreate(CMD_MESSAGE, m_messages.front().ConvertObjToStr()))) {
                        m_messages.pop();
                        Sleep(100);
                    } else {
                        break;
                    }

                }
            }
        }
        else
        {
            CModels::getInstance().WaitingForServer();
        }
        Sleep(TIME_WAIT_FOR_NEXT_SEND);
    }
    return 0;
}
