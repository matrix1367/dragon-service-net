#include "Message.h"
#include <cstdlib>

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
    return m_Name + ";" + m_Description + ";" + GetStrTypeTask()  + ";" ;
}

void CMessage::SetTypeTask(TYPE_MESSAGE typeTask)
{
    m_typeTask = typeTask;
}

TYPE_MESSAGE CMessage::GetTypeTask()
{
    return m_typeTask;
}

std::string CMessage::GetStrTypeTask()
{
    char buffor[5];
    itoa(m_typeTask,buffor,10);
    return std::string(buffor);
}
