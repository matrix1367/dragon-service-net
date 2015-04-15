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

    AddTask("taskkk",now + 39 , 0, 12);
     AddTask("tasasdadasdkkk",now + 38 , 0, 12);
      AddTask("tas23423423kkk",now + 37 , 0, 12);
       AddTask("tasasdasdkkk",now + 36 , 0, 12);
        AddTask("taskdasdasdasdasdasdkk",now + 36 , 0, 12);
         AddTask("tgfhfghfghaskkk",now + 37 , 0, 12);
          AddTask("fgjfgjgfjtaskkk",now + 35 , 0, 12);
           AddTask("tagfdhfghgfjfgjskkk",now + 34 , 0, 12);
            AddTask("tfffffaskkk",now + 36 , 0, 12);
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
    printf("SAVE");
    ofstream out_file("task.txt", ios::out | ios::binary);
    if (!out_file) return;
    for (std::list<CTask>::iterator itVCTask = schedules.begin(); itVCTask != schedules.end(); ++itVCTask )
    {
        out_file.write((char*) &(*itVCTask), sizeof(CTask));
    }
    out_file.close();

}

void CScheduleManager::Load()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "" );
    printf("LOAD");
    ifstream in_file("task.txt", ios::in | ios::binary);
    if (!in_file) return ;
            in_file.read((char *) &data, sizeof(T));
            in_file.close();
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
//    CDLog::Write( __FUNCTION__ , __LINE__, Info, "_begin" );
unsigned int i = 0;
for (std::list<CTask>::iterator itVCTask = schedules.begin(); itVCTask != schedules.end(); ++itVCTask , i++)
{

    if(i == index) {
        schedules.erase(itVCTask);
        break;
    }
}
/*
    if (schedules.size() > index) {
        CDLog::Write( __FUNCTION__ , __LINE__, Info, "Usuniety schedul o id: " + CDLog::ToString(index) );
        std::list<CTask>::iterator it = schedules.begin();
        advance(it, index);
        schedules.erase(it);
    } else {
        CDLog::Write( __FUNCTION__ , __LINE__, Info, "Nieprawidlowe id " + CDLog::ToString(index) );
    }
*/
 //   CDLog::Write( __FUNCTION__ , __LINE__, Info, "_end" );
}

CTask* CScheduleManager::GetTask(int index)
{
    int i = 0;
    for (std::list<CTask>::iterator itVCTask = schedules.begin(); itVCTask != schedules.end(); ++itVCTask , i++)
    {
        if(i == index) {
            return &(*itVCTask);
        }
    }
    return NULL;
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
                        } else {
                           // RemoveTask(i);
                           schedules.erase(itVCTask);
                        }
                }
           }

            //printf("Name task: %s", itVCTask->GetName().c_str());
        }
        Sleep(1000);
    }
    return 0;
}

std::list<CTask> CScheduleManager::GetSchedule()
{
    return schedules;
}


int testI = 0;
void CTask::Run()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "Task: " + m_name+ " RUN!" );
    CMessageManager::GetInstance().AddMessage(m_name, CDLog::ToString(testI));
    testI++;
}

std::string CTask::GetStrName()
{
    return m_name;
}

std::string CTask::GetStrDateStart()
{
    struct tm * data;
    char godzina[ 80 ];
    data = localtime( & m_dateStart );
    strftime( godzina, 80, "[%c] ", data );
    return std::string(godzina);
}

std::string CTask::GetStrDateEnd()
{
    struct tm * data;
    char godzina[ 80 ];
    data = localtime( & m_dateEnd );
    strftime( godzina, 80, "[%c] ", data );
    return std::string(godzina);
}

std::string CTask::GetStrInterval()
{
    return CDLog::ToString(m_interval);
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
