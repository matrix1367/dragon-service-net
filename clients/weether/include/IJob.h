#ifndef IJOB_H_INCLUDED
#define IJOB_H_INCLUDED

enum ID_JOB { ID_JOB_TIME = 111 };

class IJob
{
public:
    IJob(int id) : m_ID(id) {}
    int GetID();
    virtual int Run() = 0;
protected:
    int m_ID;
};

#endif // IJOB_H_INCLUDED
