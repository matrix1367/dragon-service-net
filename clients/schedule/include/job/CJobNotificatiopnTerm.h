#ifndef CJOBNOTIFICATIOPNTERM_H
#define CJOBNOTIFICATIOPNTERM_H
#include "IJob.h"
#include "Message.h"

class CJobNotificatiopnTerm : public IJob
{
    public:
        static std::string name;
        CJobNotificatiopnTerm();
        virtual ~CJobNotificatiopnTerm();
        CMessage Run();
    protected:
    private:
};

#endif // CJOBNOTIFICATIOPNTERM_H
