#ifndef CSCHEDULEMANAGER_H
#define CSCHEDULEMANAGER_H
#include <vector>
#include <time.h>
#include <string>
#include <windows.h>

class CTask {
    std::string m_name;
    time_t m_dateStart;
    time_t m_dateEnd;
    int m_interval;
public:
    CTask(std::string _name, time_t _dateStart, time_t _dateEnd, int _interval) : m_name(_name), m_dateStart(_dateStart), m_dateEnd(_dateEnd), m_interval(_interval)  {}

    void GetNextTask(CTask* task);

    void SetName(std::string name);
    void SetDataStart(time_t dataStart);
    void SetDataEnd(time_t dataEnd);
    void SetInterval(int interval);
    std::string GetName();
    time_t GetDateStart();
    time_t GetDateEnd();
    int GetInterval();
};

class CScheduleManager
{
    public:
        static CScheduleManager& getInstance() {
            static CScheduleManager insctance;
            return insctance;
        }
        void Save();
        void Load();
        void AddTask(std::string name, time_t st, time_t  en, int interval);
        void RemoveTask(unsigned int index);
        void Start();
        void Print();
        DWORD ThreadStart();
        static DWORD WINAPI StaticThreadStart(void* Param) {
            CScheduleManager * This = (CScheduleManager*) Param;
            return This->ThreadStart();
        }
    protected:
    private:
        CScheduleManager();
        virtual ~CScheduleManager();
        void operator= (CScheduleManager const&);
        DWORD m_threadID;
        std::vector<CTask> schedules;
        bool isStopSchedule;
};

#endif // CSCHEDULEMANAGER_H
