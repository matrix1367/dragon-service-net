#include "job\JobTime.h"
#include <time.h>

std::string CJobTime::name = "Aktualny czas";

CJobTime::CJobTime() : IJob(ID_JOB_TIME)
{
    //ctor
}

CJobTime::~CJobTime()
{
    //dtor
}

CMessage CJobTime::Run()
{
    time_t czas;
    struct tm * data;
    char godzina[ 80 ];

    time( & czas );
    data = localtime( & czas );

    strftime( godzina, 80, "[%d-%m-%Y %X] ", data );
    CMessage message("Aktualna godzina:",  std::string(godzina));
    message.SetTypeTask(TYPE_MESSAGE_SEND);
    return message;
}
