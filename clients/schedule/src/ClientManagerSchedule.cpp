#include "ClientManagerSchedule.h"
#include "CDLog.h"
#include <fstream>

CClientManagerSchedule::CClientManagerSchedule()
{
    //ctor
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
    std::ofstream out_file("Schedule.dat", std::ios::out | std::ios::binary | std::ios::trunc);
    if (!out_file) return;
    for (std::list<CTerm>::iterator itVCTerm= schedules.begin(); itVCTerm != schedules.end(); ++itVCTerm )
    {
        out_file.write((char*) &(*itVCTerm), sizeof(CTerm));
    }
    out_file.close();

}

void CClientManagerSchedule::Load()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "LOAD Client Schedule" );
    std::ifstream in_file("Schedule.dat", std::ios::in | std::ios::binary);
    if (!in_file) return ;

    in_file.seekg (0, in_file.end);
    int sizeFile = in_file.tellg();
    in_file.seekg (0, in_file.beg);

    in_file.seekg(0,  std::ios::beg);
     while(sizeFile >0) {
        CTerm data ;
        in_file.read((char *) &data, sizeof(CTerm));
        AddTerm(data);
        sizeFile -= sizeof(CTerm);
     }
    in_file.close();
}

DWORD CClientManagerSchedule::ThreadStart()
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "" );
    time_t now;


    while(!isStopSchedule)
    {
        time( & now );

        Sleep(1000);
    }
    return 0;
}

void CClientManagerSchedule::AddTerm(const CTerm& term)
{
    schedules.push_back(term);
}

void CClientManagerSchedule::AddTerm(const std::string& name, const time_t& dateStart, const time_t& dateEnd, const int& interval)
{
    CTerm term(name, dateStart, dateEnd, interval);
    schedules.push_back(term);
}


void CClientManagerSchedule::RemoveTerm(unsigned int term)
{

}

std::list<CTerm> CClientManagerSchedule::GetSchedule()
{
    return schedules;
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

