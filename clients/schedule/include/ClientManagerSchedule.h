#ifndef CCLIENTMANAGERSCHEDULE_H
#define CCLIENTMANAGERSCHEDULE_H
#include <windows.h>
#include <time.h>
#include <list>
#include <string>

class CTerm
{


    long long m_ID;
    char m_name[30];
    time_t m_dateStart;
    time_t m_dateEnd;
    int m_interval;

public:
    CTerm(const std::string& name, const time_t& dateStart, const time_t& dateEnd, const int& interval) : m_dateStart(dateStart), m_dateEnd(dateEnd), m_interval(interval)
    {
        m_ID = GenerateID();
        strcpy(m_name, name.c_str());
    }

    CTerm()
    {

    }

    long long GenerateID()
    {
        time_t id;
        time(& id);
        return id;
    }

    std::string GetStrDateEnd();
    std::string GetStrDateStart();
    std::string GetStrName();
    std::string GetStrInterval();

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
    void AddTerm(const std::string& name, const time_t& dateStart, const time_t& dateEnd, const int& interval);
    void RemoveTerm(unsigned int term);
     std::list<CTerm> GetSchedule();


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
