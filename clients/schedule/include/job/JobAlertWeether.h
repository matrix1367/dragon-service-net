#ifndef CJOBALERTWEETHER_H
#define CJOBALERTWEETHER_H
#include "IJob.h"
#include "Message.h"

class CJobAlertWeether : public IJob
{
    public:
        static std::string name;
        CJobAlertWeether();
        virtual ~CJobAlertWeether();
        CMessage Run();
        std::string  Parser(std::string page);
        TYPE_MESSAGE GetTypeMessage(std::string description);
    protected:
    private:
};

#endif // CJOBALERTWEETHER_H
