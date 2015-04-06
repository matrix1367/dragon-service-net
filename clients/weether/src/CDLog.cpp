#include "CDLog.h"
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;


std::string CDLog::m_nameFile = "log.txt";
LogLevel CDLog::activeLogLvel = Info;


void CDLog::Write(const char *fun ,  int line ,LogLevel lvl, std::string msg)
{
    if (lvl > CDLog::activeLogLvel) return;
    ofstream myfile;
    myfile.open (CDLog::m_nameFile.c_str(), ios::app);

    time_t czas;
    struct tm * data;
    char godzina[ 80 ];

    time( & czas );
    data = localtime( & czas );

    strftime( godzina, 80, "[%c] ", data );
    cout << CDLog::LogLvelToString(lvl).c_str() << '[' << fun << ']' << '[' <<line << ']' <<  godzina  <<  msg  << "\n";
    myfile  << CDLog::LogLvelToString(lvl).c_str() << '[' << fun << ']' << '[' <<line << ']' <<  godzina  <<  msg  << "\n";
    myfile.close();
}

std::string CDLog::LogLvelToString(LogLevel lvl)
{
    switch (lvl)
    {
    case Error:
        return "[Error]";
    case Warning :
        return "[Warning]";
    case Info :
        return "[Info]";
    default :
        return "[Info]";
    }
}
