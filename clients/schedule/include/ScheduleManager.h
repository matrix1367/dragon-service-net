#ifndef CSCHEDULEMANAGER_H
#define CSCHEDULEMANAGER_H
#include <list>
#include <time.h>
#include <string>
#include <windows.h>



class CTask {
private:
    long long m_ID;
    char m_name[30];
    time_t m_dateStart;
    time_t m_dateEnd;
    int m_interval;
    int m_id_job;
    char m_setting[30];

public:
    CTask() {
         m_ID = GenerateID();
    };
    CTask(std::string _name, time_t _dateStart, time_t _dateEnd, int _interval) : m_dateStart(_dateStart), m_dateEnd(_dateEnd), m_interval(_interval)  {
        strcpy(m_name, _name.c_str());
        m_id_job = 0;
        m_ID = GenerateID();
    }
    long long GenerateID() {
        time_t id;
        time(& id);
        return id;
    }
    void Run();

    std::string GetStrName();
    std::string GetStrDateStart();
    std::string GetStrDateEnd();
    std::string GetStrInterval();
    std::string GetStrString();
    std::string GetStrID();

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
    SYSTEMTIME GetDataStart();
    SYSTEMTIME GetDataEnd();
    int GetInterval();
    int GetIdJob();
    long long GetID() { return m_ID; }
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
        bool EditTask(long long ID, std::string name, time_t st, time_t  en, int interval, int IDJob,  std::string setting);
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