#ifndef CDLOG_H
#define CDLOG_H
#include <string>

enum LogLevel { Error , Warning , Info};

class CDLog
{
public:

    static void Write(const char *fun ,  int line ,LogLevel lvl,std::string msg);
    static std::string LogLvelToString(LogLevel lvl);

    static LogLevel activeLogLvel;

protected:
private:
    static std::string m_nameFile;

};

#endif // CDLOG_H
