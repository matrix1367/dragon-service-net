#include "CJobNotificatiopnTerm.h"

std::string CJobNotificatiopnTerm::name = "Powiadomienia harmonogramu";

CJobNotificatiopnTerm::CJobNotificatiopnTerm() : IJob(ID_JOB_NOTYFICATION_TERM)
{
    //ctor
}

CJobNotificatiopnTerm::~CJobNotificatiopnTerm()
{
    //dtor
}

CMessage CJobNotificatiopnTerm::Run()
{
    CMessage message("Notyfikacja:",  std::string("test"));
    message.SetTypeTask(TYPE_MESSAGE_SEND);
    return message;
}
