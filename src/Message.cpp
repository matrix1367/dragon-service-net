#include "Message.h"
#include "Tools.h"
#include "CDLog.h"

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
    if (msg.size() == 2) {
        CMessage message(msg[0], msg[1],idClient);
        return message;
    } else {
         CDLog::Write( __FUNCTION__ , __LINE__, Info, "liczba parametrów jest niepoprawna");
         CMessage message("","", idClient);
         return message;
    }
}

