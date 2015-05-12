#include "MessageManager.h"
#include "ManagerDataBase.h"
#include <cstdlib>

CMessageManager::CMessageManager()
{
    std::vector<std::vector<std::string> > rows = CManagerDataBase::getInstance().GetAllMessage();
    for(std::vector<std::vector<std::string> > ::size_type j = 0 ; j != rows.size(); j++)
    {
        AddMessage(rows[j][2],rows[j][3] , atoi( (rows[j][1]).c_str() ) );
    }
}

CMessageManager::~CMessageManager()
{
    //dtor
}

void CMessageManager::AddMessage( std::string name,  std::string description,  int id)
{
    CMessage msgTmp(name, description, id);
    m_messages.push_back(msgTmp);
}

void CMessageManager::AddMessage(const CMessage& message)
{
    if (message.GetTypeTask() != TYPE_MESSAGE_SEND)
    {
        CManagerDataBase::getInstance().AddMessage(message);
    }

    m_messages.push_back(message);
}


std::list<CMessage> CMessageManager::GetMessages()
{
    return m_messages;
}

CMessage CMessageManager::GetItemMessage(unsigned int index)
{
    CMessage msg;
    for (std::list<CMessage>::iterator it = m_messages.begin(); it != m_messages.end(); it++) {

        msg =  *it;
    }
    return msg;
}


void CMessageManager::RemoveMessage(unsigned int index)
{
    unsigned int i = 0;
    for (std::list<CMessage>::iterator itVCMessage = m_messages.begin(); itVCMessage != m_messages.end(); ++itVCMessage , i++)
    {

        if(i == index) {
            m_messages.erase(itVCMessage);
            break;
        }
    }
}
