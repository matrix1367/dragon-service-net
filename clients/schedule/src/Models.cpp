#include "Models.h"
#include "CDLog.h"
#include "WebTool.h"
#include "Convert.h"
#include "EventManager.h"
#include "CDSetting.h"
#include <cstdio>

CModels::CModels()
{
    m_isWaitingForServer = false;
    m_isUpdateWeather = true;
    strcpy(m_data.nameCity, "brak danych");
    m_data.description = "brak danych";
    m_data.main.pressure = 0;
    m_data.wind.speed = 0;
    m_data.wind.deg = 0;
}

void CModels::Init(void)
{
    WaitingForServer();
    UpdateWeether();
}

CClient& CModels::GetClient()
{
    return client;
}

DWORD WINAPI CModels::ThreadUpdateWeether()
{
    CWebTool webTool;
    while (m_isUpdateWeather)
    {
        std::string page = webTool.getPage("http://api.openweathermap.org/data/2.5/weather?q=Poznan,pl&APPID=d5141e09f65620429bf2ac06cd2a5a2c");
        if (CConvert::ParserJSONToDataWeeter(page, m_data))
        {
            //printf("Page: %s\n" , page.c_str());
            //m_data.Print();
            CEventManager::getInstance().Send(EVENT_UPDATE_WEETHER);
        } else {
            //printf("Page not CONVERT : %s\n" , page.c_str());
            CDLog::Write( __FUNCTION__ , __LINE__, Error, "Page [http://api.openweathermap.org/data/2.5/weather?q=Poznan,pl] not CONVERT" );
        }

         std::string pageNextHours = webTool.getPage("http://api.openweathermap.org/data/2.5/forecast?q=Poznan,pl");
         //printf("Page: %s\n" , pageNextHours.c_str());
         if (CConvert::ParserJSONToDataWeeterList(pageNextHours, m_dataList))
         {
            CEventManager::getInstance().Send(EVENT_UPDATE_WEATHER_NEXT_HOURS);
         } else {
            //printf("Page not CONVERT : %s\n" , pageNextHours.c_str());
            CDLog::Write( __FUNCTION__ , __LINE__, Error, "Page [http://api.openweathermap.org/data/2.5/forecast?q=Poznan,pl] not CONVERT" );
        }

        Sleep(CDSetting::getInstance().getSetting().updateWeather);
    }
    return 0;
}

void CModels::UpdateWeether()
{
    CreateThread(0,0,StaticThreadUpdateWeether, (void*)this , 0,&m_ThreadUpdateWeether);
}

void CModels::WaitingForServer()
{
    if(!m_isWaitingForServer)
    {
        CDLog::Write( __FUNCTION__ , __LINE__, Info, "Thread: Oczekiwanie na serwer...." );
        m_handlThread =  CreateThread(0,0,StaticThreadWaitingForServer, (void*)this , 0,&m_WaitingForServerThreadID);
    }
}

DWORD WINAPI CModels::ThreadWaitingForServer()
{
    m_isWaitingForServer = true;
    while (!client.SetIsConnect(client.Init()))
    {
        CDLog::Write( __FUNCTION__ , __LINE__, Info, ">>>[ping serwera]<<<" );
        Sleep(3000);
    }
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "Klient nawiazan polaczenie z serwerem." );
    client.Connect();
    m_isWaitingForServer = false;
    return 0;
}

bool CModels::GetWeether(const std::string& nameCity, DataWeether & data)
{
    data = m_data;
    return true;
}


bool CModels::GetWeether(const std::string& nameCity, DataWeetherNextHours& data)
{
    data = m_dataList;
    return true;
}

std::string CModels::GetTopicalWeather(const std::string& nameCity)
{
    return m_data.main.getStr_temp() + " C, " + m_data.main.getStr_pressure() +" hPa, " + m_data.description;
}