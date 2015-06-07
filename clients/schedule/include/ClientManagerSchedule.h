#ifndef CCLIENTMANAGERSCHEDULE_H
#define CCLIENTMANAGERSCHEDULE_H
#include <windows.h>
#include <time.h>
#include <list>
#include <string>
#include <cstdio>

class CTerm
{

    bool m_showMessage;
    bool m_sendMessage;
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
        m_showMessage = true;
        m_sendMessage = false;
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

    time_t GetDateStart();
    time_t GetDateEnd();
    bool GetShowMessage() { return m_showMessage;}
    void SetShowMessage(bool message) { m_showMessage = message; }

    bool GetSendMessage() { return m_sendMessage;}
    void SetSendMessage(bool message) { m_sendMessage = message; }

    int GetInterval()
    {
        return m_interval;
    }

    std::string GetStrDateEnd();
    std::string GetStrDateStart();
    std::string GetStrName();
    std::string GetStrInterval();

    void SetDateStart(time_t time) { m_dateStart = time;}
    void SetDateEnd(time_t time) { m_dateEnd = time; }

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
    void GetSchedule(std::list<CTerm> & terms);


    static DWORD WINAPI StaticThreadStart(void* Param)
    {
        CClientManagerSchedule * This = (CClientManagerSchedule*) Param;
        return This->ThreadStart();
    }
   void lock() { printf("LOCK\n"); isLockScheduleList = true; }
   void unlock() { printf("UNLOCK\n"); isLockScheduleList = false; }
   bool tryLock() { return isLockScheduleList;}
   void waitUnlock() {
       printf("WAIT START\n");
        while (isLockScheduleList)
        {

            Sleep(100);
        }
        printf("WAIT STOP\n");
        lock();
   }
protected:
private:
    DWORD ThreadStart();
    CClientManagerSchedule();
    virtual ~CClientManagerSchedule();

    DWORD m_threadID;
    bool isStopSchedule;

    std::list<CTerm> schedules;

    //mutex
     bool isLockScheduleList;

};

#endif // CCLIENTMANAGERSCHEDULE_H
