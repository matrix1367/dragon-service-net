#include "Message.h"
#include "Tools.h"
#include "CDLog.h"
#include <cstdlib>

CMessage::CMessage()
{
    //ctor
}

CMessage::~CMessage()
{
    //dtor
}

CMessage CMessage::ConvertStrToObj(std::string parm,  unsigned int idClient)
{
    std::vector<std::string> msg = split(const_cast<char*>(parm.c_str()), ";");
    if (msg.size() == 3) {
        CMessage message(msg[0], msg[1],idClient);
        message.SetTypeTask((TYPE_MESSAGE) atoi(msg[2].c_str()));
        return message;
    } else {
         CDLog::Write( __FUNCTION__ , __LINE__, Info, "Liczba parametrów jest niepoprawna");
         CMessage message("","", idClient);
         return message;
    }
}

void CMessage::SetTypeTask(TYPE_MESSAGE typeTask)
{
    m_typeTask = typeTask;
}

TYPE_MESSAGE CMessage::GetTypeTask() const
{
    return m_typeTask;
}

