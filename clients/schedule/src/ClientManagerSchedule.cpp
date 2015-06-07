#include "ClientManagerSchedule.h"
#include "EventManager.h"
#include "GUIManager.h"
#include "CDLog.h"
#include <fstream>

CClientManagerSchedule::CClientManagerSchedule()
{
    isLockScheduleList = false;
}

CClientManagerSchedule::~CClientManagerSchedule()
{
    //dtor
}

void CClientManagerSchedule::Start()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "" );
    CreateThread(0,0,StaticThreadStart, (void*)this , 0,&m_threadID);
}

void CClientManagerSchedule::Save()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "SAVE Client Schedule" );
    std::ofstream out_file("ClientSchedule.dat", std::ios::out | std::ios::binary | std::ios::trunc);
    if (!out_file) return;
    //waitUnlock();
    for (std::list<CTerm>::iterator itVCTerm= schedules.begin(); itVCTerm != schedules.end(); ++itVCTerm )
    {
        out_file.write((char*) &(*itVCTerm), sizeof(CTerm));
    }
    out_file.close();
    //unlock();
}

void CClientManagerSchedule::Load()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "LOAD Client Schedule" );
    std::ifstream in_file("ClientSchedule.dat", std::ios::in | std::ios::binary);
    if (!in_file) return ;

    in_file.seekg (0, in_file.end);
    int sizeFile = in_file.tellg();
    in_file.seekg (0, in_file.beg);

    in_file.seekg(0,  std::ios::beg);
    while(sizeFile >0)
    {
        CTerm data ;
        in_file.read((char *) &data, sizeof(CTerm));
        AddTerm(data);
        sizeFile -= sizeof(CTerm);
    }
    in_file.close();
}

//#include <iostream>
DWORD CClientManagerSchedule::ThreadStart()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "" );
    time_t now;
    std::list<CTerm>::iterator itTerm ;
    time_t newTime;
    time_t splitTime;
    while(!isStopSchedule)
    {
        time( & now );
        for (itTerm = schedules.begin(); itTerm != schedules.end() ; itTerm++ )
        {
            //std::cout << "st:" <<  (itTerm->GetDateStart() <= now)  << " end:" <<  (itTerm->GetDateEnd() >= now) << " now:" << now << " show:" <<  itTerm->GetShowMessage() <<std::endl;
            //printf("TIME now :%ll, st :%ll end: %ll , showMessage %d\n" , (long long)now, (long long)itTerm->GetDateStart(), (long long)itTerm->GetDateEnd(), (int)itTerm->GetShowMessage());
            //printf("%d %d %d\n" , (int)(itTerm->GetDateStart() <= now), (int)(itTerm->GetDateEnd() >= now), (int)itTerm->GetShowMessage());
            if (itTerm->GetShowMessage() && itTerm->GetDateStart() <= now && itTerm->GetDateEnd() >= now)
            {
                printf("Zadanie %s rozpocze³o siê\n" , itTerm->GetStrName().c_str());
                //term sie rozpoczol i nie skonczyl
                itTerm->SetShowMessage(false);
                CGUIManager::getInstance().ShowMessage("Rozpoczelo sie zadanie",itTerm->GetStrName());
            }

            if (itTerm->GetDateEnd() < now )
            {
                if (itTerm->GetInterval() >0)
                {
                    //wyliczanie nowego zadania zgodnie z intervalem

                    splitTime = itTerm->GetDateEnd() - itTerm->GetDateStart();
                    newTime =  itTerm->GetDateStart() + itTerm->GetInterval();
                    while (newTime <= now) {
                        newTime += itTerm->GetInterval();
                    }

                    itTerm->SetDateStart( newTime );
                    itTerm->SetDateEnd( newTime+ splitTime);
                    itTerm->SetShowMessage(true);
                    CEventManager::getInstance().Send(EVENT_ADD_TERM);
                    continue;
                }
                else
                {
                    //usuwanie zadania ktore nie posiada interwalu i jest zakonczone
                    //printf("Usuniecie zadania z listy %d, %s\n", i , itTerm->GetStrName().c_str());
                    //RemoveTerm(i);
                    waitUnlock();
                    schedules.erase(itTerm);
                    unlock();
                    CEventManager::getInstance().Send(EVENT_DELETE_TERM);
                    continue;
                }
            }
        }
        //unlock();
        Sleep(1000);
    }
    return 0;
}

void CClientManagerSchedule::AddTerm(const CTerm& term)
{
    waitUnlock();
    schedules.push_back(term);
    unlock();
}

void CClientManagerSchedule::AddTerm(const std::string& name, const time_t& dateStart, const time_t& dateEnd, const int& interval)
{
    waitUnlock();
    CTerm term(name, dateStart, dateEnd, interval);
    schedules.push_back(term);
    unlock();
}


void CClientManagerSchedule::RemoveTerm(unsigned int index)
{
    waitUnlock();
    unsigned int i = 0;
    for (std::list<CTerm>::iterator itVCTask = schedules.begin(); itVCTask != schedules.end(); ++itVCTask , i++)
    {

        if(i == index) {
            schedules.erase(itVCTask);
            break;
        }
    }
    unlock();
}

void CClientManagerSchedule::GetSchedule(std::list<CTerm> & terms )
{
    waitUnlock();
    terms = schedules;
    unlock();
}

std::string CTerm::GetStrInterval()
{
    return CDLog::ToString(m_interval);
}

std::string CTerm::GetStrName()
{
    return m_name;
}

std::string CTerm::GetStrDateStart()
{
    struct tm * data;
    char godzina[ 80 ];
    data = localtime( & m_dateStart );
    strftime( godzina, 80, "%d-%m-%Y %X", data );
    return std::string(godzina);
}

std::string CTerm::GetStrDateEnd()
{
    if (m_dateEnd == 0 ) return "brak";
    struct tm * data;
    char godzina[ 80 ];
    data = localtime( & m_dateEnd );
    strftime( godzina, 80, "%d-%m-%Y %X", data );
    return std::string(godzina);
}

time_t CTerm::GetDateStart()
{
    return m_dateStart;
}

time_t CTerm::GetDateEnd()
{
    return m_dateEnd;
}
