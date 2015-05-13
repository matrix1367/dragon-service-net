#ifndef CCLIENTMANAGERSCHEDULE_H
#define CCLIENTMANAGERSCHEDULE_H
#include <windows.h>
#include <time.h>
#include <list>

class CTerm
{
    long long m_ID;
    char m_name[30];
    time_t m_dateStart;
    time_t m_dateEnd;
};

class CClientManagerSchedule
{
public:
    static CClientManagerSchedule& getInstance()
    {
        static CClientManagerSchedule insctance;
        return insctance;
    }
    void Save();
    void Load();
    void Start();
    void AddTerm(const CTerm& term);
    void RemoveTerm(unsigned int term);
    static DWORD WINAPI StaticThreadStart(void* Param)
    {
        CClientManagerSchedule * This = (CClientManagerSchedule*) Param;
        return This->ThreadStart();
    }
protected:
private:
    DWORD ThreadStart();
    CClientManagerSchedule();
    virtual ~CClientManagerSchedule();

    DWORD m_threadID;
    bool isStopSchedule;

    std::list<CTerm> schedules;
};

#endif // CCLIENTMANAGERSCHEDULE_H
