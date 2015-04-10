#include "MessageManager.h"

CMessageManager::CMessageManager()
{
    //ctor
}

CMessageManager::~CMessageManager()
{
    //dtor
}

void CMessageManager::AddMessage(const std::string name, const std::string description)
{

}

void CMessageManager::AddMessage(const CMessage& message)
{
    m_messages.push_back(message);
}


std::list<CMessage> CMessageManager::GetMessages()
{
    return m_messages;
}
