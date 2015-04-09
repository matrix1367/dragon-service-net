#ifndef CDLOG_H
#define CDLOG_H
#include <string>
#include <sstream>

enum LogLevel { Error , Warning , Info};

class CDLog
{
public:

    static void Write(const char *fun ,  int line ,LogLevel lvl,std::string msg);
    static std::string LogLvelToString(LogLevel lvl);

    static LogLevel activeLogLvel;

    static std::string ToString(unsigned int i) {
        std::ostringstream ss;
        ss << i;
        return ss.str();
    }

protected:
private:
    static std::string m_nameFile;

};

#endif // CDLOG_H
