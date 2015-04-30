#ifndef CSCHEDULEMANAGER_H
#define CSCHEDULEMANAGER_H
#include <list>
#include <time.h>
#include <string>
#include <windows.h>



class CTask {
private:
    char m_name[30];
    time_t m_dateStart;
    time_t m_dateEnd;
    int m_interval;
    int m_id_job;
    char m_setting[30];

public:
    CTask() {};
    CTask(std::string _name, time_t _dateStart, time_t _dateEnd, int _interval) : m_dateStart(_dateStart), m_dateEnd(_dateEnd), m_interval(_interval)  {
        strcpy(m_name, _name.c_str());
        m_id_job = 0;
    }
    void Run();

    std::string GetStrName();
    std::string GetStrDateStart();
    std::string GetStrDateEnd();
    std::string GetStrInterval();

    void GetNextTask(CTask* task);

    void SetName(std::string name);
    void SetDateStart(time_t dataStart);
    void SetDataEnd(time_t dataEnd);
    void SetInterval(int interval);
    void SetIdJob(int id_job);
    void SetSetting(const std::string& setting);

    std::string GetName();
    time_t GetDateStart();
    time_t GetDateEnd();
    int GetInterval();
    int GetIdJob();

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
        void AddTask(std::string name, time_t st, time_t  en, int interval, int IDJob,  std::string setting);
        void AddTask(const CTask& task);
        void RemoveTask(unsigned int index);
        void Start();
        void Print();
        DWORD ThreadStart();
        static DWORD WINAPI StaticThreadStart(void* Param) {
            CScheduleManager * This = (CScheduleManager*) Param;
            return This->ThreadStart();
        }
        std::list<CTask> GetSchedule();
        CTask* GetTask(int index);
    protected:
    private:
        CScheduleManager();
        virtual ~CScheduleManager();
        void operator= (CScheduleManager const&);
        DWORD m_threadID;
        bool isStopSchedule;
        std::list<CTask> schedules;

};

#endif // CSCHEDULEMANAGER_H
