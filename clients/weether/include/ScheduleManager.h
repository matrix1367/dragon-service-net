#ifndef CSCHEDULEMANAGER_H
#define CSCHEDULEMANAGER_H
#include <vector>
#include <time.h>
#include <string>
#include <windows.h>

class CTask {
    std::string name;
    time_t dateStart;
    time_t dateEnd;
    int interval;
public:
    CTask(std::string _name, time_t _dateStart, time_t _dateEnd, int _interval) : name(_name), dateStart(_dateStart), dateEnd(_dateEnd), interval(_interval)  {}
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
        void RemoveTask(int index);
        void Start();
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
