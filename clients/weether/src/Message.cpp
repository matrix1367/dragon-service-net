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
