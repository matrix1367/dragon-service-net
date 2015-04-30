#include "Message.h"

CMessage::CMessage()
{
    //ctor
}

CMessage::~CMessage()
{
    //dtor
}

std::string CMessage::ConvertObjToStr()
{
    return m_Name + ";" + m_Description + ";" ;
}

void CMessage::SetTypeTask(TYPE_MESSAGE typeTask)
{
    m_typeTask = typeTask;
}

TYPE_MESSAGE CMessage::GetTypeTask()
{
    return m_typeTask;
}
