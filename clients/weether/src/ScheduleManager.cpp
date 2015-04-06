#include "ScheduleManager.h"
#include "CDSerialize.h"
#include <cstdio>
#include "CDLog.h"

CScheduleManager::CScheduleManager()
{
    isStopSchedule = false;
    AddTask("taskkk",NULL,NULL,0);
    AddTask("tassooo", NULL, NULL, 0);
}

CScheduleManager::~CScheduleManager()
{
    //dtor
}

void CScheduleManager::AddTask(std::string name, time_t st, time_t  en, int interval)
{
    CTask task(name, st, en, interval);
    schedules.push_back(task);
}

void CScheduleManager::Save()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "" );
    //printf("SAVE");
    //CDSettingData settingData(automaticCallMainDlL, automaticShowMainForm);
    //CDSerialize<std::vector<CTask> > serialzer("dragon_setting.dat");
    //serialzer.Write(schedules);
    //Print();
}

void CScheduleManager::Load()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "" );
    //printf("LOAD");
    //schedules.clear();
    //CDSerialize<std::vector<CTask> > serialzer("dragon_setting.dat");
    //serialzer.Read(schedules);
    //Print();
}

void CScheduleManager::RemoveTask(int index)
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "" );
   // schedules.erase(index);
}

void CScheduleManager::Start()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "" );
    CreateThread(0,0,StaticThreadStart, (void*)this , 0,&m_threadID);
}

DWORD CScheduleManager::ThreadStart()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "" );
    time_t now;
    struct tm * data;
    time( & now );
    data = localtime( & now );

    while(!isStopSchedule)
    {
        for (std::vector<CTask>::iterator itVCTask = schedules.begin(); itVCTask != schedules.end(); itVCTask++)
        {
            CDLog::Write( __FUNCTION__ , __LINE__, Info, itVCTask->GetName() );
            //printf("Name task: %s", itVCTask->GetName().c_str());
        }
        Sleep(1000);
    }
    return 0;
}

void CTask::SetName(std::string name)
{
    m_name = name;
}

void CTask::SetDataStart(time_t dateStart)
{
    m_dateStart = dateStart;
}

void CTask::SetDataEnd(time_t dateEnd)
{
    m_dateEnd = dateEnd;
}

void CTask::SetInterval(int interval)
{
    m_interval = interval;
}

std::string CTask::GetName()
{
    return m_name;
}

time_t CTask::GetDataStart()
{
    return m_dateStart;
}

time_t CTask::GetDataEnd()
{
    return m_dateEnd;
}

void CScheduleManager::Print()
{
    for (std::vector<CTask>::iterator itVCTask = schedules.begin(); itVCTask != schedules.end(); itVCTask++)
    {
        printf("Name task: %s", itVCTask->GetName().c_str());
    }
}
