#ifndef IJOB_H_INCLUDED
#define IJOB_H_INCLUDED
#include "Message.h"

enum ID_JOB { ID_JOB_TIME = 111 };

class IJob
{
public:
    IJob(int id) : m_ID(id) {}
    virtual ~IJob(){}
    int GetID();
    virtual CMessage Run() = 0;
protected:
    int m_ID;
};

#endif // IJOB_H_INCLUDED
