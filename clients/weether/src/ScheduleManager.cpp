#include "ScheduleManager.h"
#include "MessageManager.h"
#include "CDSerialize.h"
#include <cstdio>
#include "CDLog.h"
//#include "IJob.h"
#include "TasksManager.h"
#include "EventManager.h"

CScheduleManager::CScheduleManager()
{
    isStopSchedule = false;
 //   time_t now;
  //  time( & now );

    //AddTask("taskkk",now + 39 , 0, 12);
     //AddTask("tasasdadasdkkk",now + 38 , 0, 12);
      //AddTask("tas23423423kkk",now + 37 , 0, 12);

   // AddTask("tassooo", now+21, now+28, 0);
}

CScheduleManager::~CScheduleManager()
{
    //dtor
}

void CScheduleManager::AddTask(std::string name, time_t st, time_t  en, int interval, int IDJob)
{
    CTask task(name, st, en, interval);
    task.SetIdJob(IDJob);
    schedules.push_back(task);
}



void CScheduleManager::Save()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "" );
    printf("SAVE Schedule\n");
    ofstream out_file("task.txt", ios::out | ios::binary | ios::trunc);
    if (!out_file) return;
    for (std::list<CTask>::iterator itVCTask = schedules.begin(); itVCTask != schedules.end(); ++itVCTask++ )
    {
        out_file.write((char*) &(*itVCTask), sizeof(CTask));
    }
    out_file.close();

}

void CScheduleManager::Load()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "" );
    printf("LOAD Schedule\n");
    ifstream in_file("task.txt", ios::in | ios::binary);
    if (!in_file) return ;

    in_file.seekg (0, in_file.end);
    int sizeFile = in_file.tellg();
    in_file.seekg (0, in_file.beg);

    in_file.seekg(0,  std::ios::beg);
     while(sizeFile >0) {
        CTask data ;
        in_file.read((char *) &data, sizeof(CTask));
        AddTask(data);
        sizeFile -= sizeof(CTask);
     }
    in_file.close();
}

void CScheduleManager::AddTask(const CTask& task)
{
    schedules.push_back(task);
}


void CTask::GetNextTask(CTask* task) {

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

    unsigned int i;
    while(!isStopSchedule)
    {
       // CDLog::Write( __FUNCTION__ , __LINE__, Debug, "Task tick: " );
        time( & now );
        i = 0;


        for (std::list<CTask>::iterator itVCTask = schedules.begin(); itVCTask != schedules.end(); itVCTask++ , i++)
        {
           // CDLog::Write( __FUNCTION__ , __LINE__, Info, "for Task tick: " );
           if (i >= schedules.size()) {
            break;
           }
           if(itVCTask->GetDateStart() <= now) {
                if (itVCTask->GetDateEnd() == 0) {
                  CDLog::Write( __FUNCTION__ , __LINE__, Info, "Task: " + itVCTask->GetName() + " zostal wykonany." );
                  itVCTask->Run();

                  if (itVCTask->GetInterval() > 0) {
                            //task posiada interwal;
                            CDLog::Write( __FUNCTION__ , __LINE__, Info, "Task: " + itVCTask->GetName() + "  posiada interwal : " + itVCTask->GetStrInterval() );
                            time_t newTime;
                            newTime =  itVCTask->GetDateStart() + itVCTask->GetInterval();
                            while (newTime <= now) {
                                 newTime += itVCTask->GetInterval();
                            }

                            itVCTask->SetDateStart( newTime );
                            CEventManager::getInstance().Send(EVENT_ADD_SCHEDULE);
                            continue;
                        } else {
                           CDLog::Write( __FUNCTION__ , __LINE__, Info, "Task: " + itVCTask->GetName() + "  nie posiada interwal zostanie usuniety." );
                           schedules.erase(itVCTask);
                           CEventManager::getInstance().Send(EVENT_DELETE_SCHEDULE);
                           continue;
                        }
                } else {
                    CDLog::Write( __FUNCTION__ , __LINE__, Info, "Task: " + itVCTask->GetName() + " zostal wykonany." );
                    itVCTask->Run();
                        if (itVCTask->GetInterval() > 0) {
                            //task posiada interwal;
                            CDLog::Write( __FUNCTION__ , __LINE__, Info, "Task: " + itVCTask->GetName() + "  posiada interwal : " + itVCTask->GetStrInterval() );
                            time_t newTime;
                            newTime =  itVCTask->GetDateStart() + itVCTask->GetInterval();
                            if (newTime > itVCTask->GetDateEnd()) {
                                CDLog::Write( __FUNCTION__ , __LINE__, Info, "Task: " + itVCTask->GetName() + "  ddata start jest wieksza od end zostanie usuniety." );
                                schedules.erase(itVCTask);
                                CEventManager::getInstance().Send(EVENT_DELETE_SCHEDULE);
                                continue;
                            }
                            while (newTime <= now ) {
                                 newTime += itVCTask->GetInterval();
                            }

                            itVCTask->SetDateStart( newTime );
                            CEventManager::getInstance().Send(EVENT_ADD_SCHEDULE);
                            continue;
                        } else {
                           CDLog::Write( __FUNCTION__ , __LINE__, Info, "Task: " + itVCTask->GetName() + "  nie posiada interwal zostanie usuniety." );
                           schedules.erase(itVCTask);
                           CEventManager::getInstance().Send(EVENT_DELETE_SCHEDULE);
                           continue;
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

 static DWORD WINAPI ThreadRun(void* Param) {
    CTask * task = (CTask*) Param;
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "Task: " + task->GetStrName() + " RUN!" );

    IJob* job = CJobsManager::getInstance().GetJobs(task->GetIdJob() );

    CMessageManager::GetInstance().AddMessage(job->Run());

    delete job;
    return 0;
 }


void CTask::Run()
{
    //DWORD m_threadID;
    //CreateThread(0,0,ThreadRun, (void*)this , 0 , &m_threadID);

    IJob* job = CJobsManager::getInstance().GetJobs(this->GetIdJob() );
    CMessageManager::GetInstance().AddMessage(job->Run());

    delete job;
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
    strftime( godzina, 80, "%d-%m-%Y %X", data );
    return std::string(godzina);
}

std::string CTask::GetStrDateEnd()
{
    struct tm * data;
    char godzina[ 80 ];
    data = localtime( & m_dateEnd );
    strftime( godzina, 80, "%d-%m-%Y %X", data );
    return std::string(godzina);
}

std::string CTask::GetStrInterval()
{
    return CDLog::ToString(m_interval);
}


void CTask::SetName(std::string name)
{
    strcpy(m_name, name.c_str());
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

void CTask::SetIdJob(int id_job)
{
        m_id_job = id_job;
}

int CTask::GetIdJob()
{
    return m_id_job;
}


void CScheduleManager::Print()
{
    for (std::list<CTask>::iterator itVCTask = schedules.begin(); itVCTask != schedules.end(); itVCTask++)
    {
        printf("Name task: %s", itVCTask->GetName().c_str());
    }
}
