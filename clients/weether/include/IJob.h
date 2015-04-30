#ifndef IJOB_H_INCLUDED
#define IJOB_H_INCLUDED
#include "Message.h"

enum ID_JOB { ID_JOB_TIME = 111 , ID_JOB_ALERT_WATHER = 222 };

union ParmJob {
    char str[30];
};

class IJob
{
public:
    IJob(int id) : m_ID(id) {}
    virtual ~IJob(){}
    int GetID();
    void SetParmJob(ParmJob parm1)
    {
        m_parm1 = parm1;
    }
    virtual CMessage Run() = 0;

protected:
    int m_ID;
    ParmJob m_parm1;

};

#endif // IJOB_H_INCLUDED
