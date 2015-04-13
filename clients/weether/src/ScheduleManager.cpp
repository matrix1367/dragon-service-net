#include "ScheduleManager.h"
#include "MessageManager.h"
#include "CDSerialize.h"
#include <cstdio>
#include "CDLog.h"

CScheduleManager::CScheduleManager()
{
    isStopSchedule = false;
    time_t now;
    time( & now );

    AddTask("taskkk",now + 3 , 0, 2);
   // AddTask("tassooo", now+21, now+28, 0);
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

void CTask::GetNextTask(CTask* task) {
    if (m_interval > 0) {
            if (m_dateEnd == 0) {
                CDLog::Write( __FUNCTION__ , __LINE__, Info, "m_dateEnd == 0 "  );
                task =  new CTask(m_name, m_dateStart + m_interval, 0, m_interval );
               // return task;
            } else {
                CDLog::Write( __FUNCTION__ , __LINE__, Info, "m_dateEnd != 0 full task" );
                task =  new CTask(m_name+"_:_", m_dateStart + m_interval, m_dateEnd + m_interval, m_interval );
              //  return task;
            }
    } else {
        CDLog::Write( __FUNCTION__ , __LINE__, Info, "m_interval < 0 -> null task"  );
        task =  new CTask("", 0 , 0 ,0);
    }
}

void CScheduleManager::RemoveTask(unsigned index)
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "_begin" );


    if (schedules.size() > index) {
        CDLog::Write( __FUNCTION__ , __LINE__, Info, "Usuniety schedul o id: " + CDLog::ToString(index) );
        std::list<CTask>::iterator it = schedules.begin();
        advance(it, index);
        schedules.erase(it);
    } else {
        CDLog::Write( __FUNCTION__ , __LINE__, Info, "Nieprawidlowe id " + CDLog::ToString(index) );
    }

    CDLog::Write( __FUNCTION__ , __LINE__, Info, "_end" );
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
    std::list<CTask> newTasks;
  //  struct tm * data;

    //data = localtime( & now );
    int i;
    while(!isStopSchedule)
    {
       // CDLog::Write( __FUNCTION__ , __LINE__, Debug, "Task tick: " );
        time( & now );
        i = 0;
        newTasks.clear();

        for (std::list<CTask>::iterator itVCTask = schedules.begin(); itVCTask != schedules.end(); itVCTask++ , i++)
        {
           // CDLog::Write( __FUNCTION__ , __LINE__, Info, "for Task tick: " );
           if(itVCTask->GetDateStart() <= now) {
                if (itVCTask->GetDateEnd() == 0) {
                 // CDLog::Write( __FUNCTION__ , __LINE__, Info, "Task: " + itVCTask->GetName() + " zostal wykonany." );
                  itVCTask->Run();
                  if (itVCTask->GetInterval() > 0) {
                            //task posiada interwal;
                            //CDLog::Write( __FUNCTION__ , __LINE__, Info, "Task: " + itVCTask->GetName() + "  posiada interwal." );
                            itVCTask->SetDateStart( itVCTask->GetDateStart() + itVCTask->GetInterval());
                        }
                }
           }

            //printf("Name task: %s", itVCTask->GetName().c_str());
        }
        Sleep(1000);
    }
    return 0;
}

int testI = 0;
void CTask::Run()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "Task: " + m_name+ " RUN!" );
    CMessageManager::GetInstance().AddMessage(m_name, CDLog::ToString(testI));
    testI++;
}

void CTask::SetName(std::string name)
{
    m_name = name;
}

void CTask::SetDateStart(time_t dateStart)
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

time_t CTask::GetDateStart()
{
    return m_dateStart;
}

time_t CTask::GetDateEnd()
{
    return m_dateEnd;
}

int CTask::GetInterval()
{
    return m_interval;
}


void CScheduleManager::Print()
{
    for (std::list<CTask>::iterator itVCTask = schedules.begin(); itVCTask != schedules.end(); itVCTask++)
    {
        printf("Name task: %s", itVCTask->GetName().c_str());
    }
}
