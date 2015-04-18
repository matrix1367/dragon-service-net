#ifndef CJOBTIME_H
#define CJOBTIME_H
#include "IJob.h"
#include "Message.h"

class CJobTime : public IJob
{
    public:
        CJobTime();
        virtual ~CJobTime();
        CMessage Run();
    protected:
    private:
};

#endif // CJOBTIME_H
