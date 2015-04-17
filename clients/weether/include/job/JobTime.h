#ifndef CJOBTIME_H
#define CJOBTIME_H
#include "IJob.h"

class CJobTime : public IJob
{
    public:
        CJobTime();
        virtual ~CJobTime();
        int Run();
    protected:
    private:
};

#endif // CJOBTIME_H
