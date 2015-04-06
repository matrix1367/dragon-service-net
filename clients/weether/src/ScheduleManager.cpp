#include "ScheduleManager.h"

CScheduleManager::CScheduleManager()
{
    isStopSchedule = false;
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

}

void CScheduleManager::Load()
{

}

void CScheduleManager::RemoveTask(int index)
{

}

void CScheduleManager::Start()
{
    CreateThread(0,0,StaticThreadStart, (void*)this , 0,&m_threadID);
}

DWORD CScheduleManager::ThreadStart()
{
    while(isStopSchedule)
    {
        for (std::vector<CTask>::iterator itVCTask = schedules.begin(); itVCTask != schedules.end(); itVCTask++)
        {

        }
        Sleep(1000);
    }
    return 0;
}

