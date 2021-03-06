#include "Models.h"
#include "CDLog.h"
#include <cstdio>

void CModels::Init(void)
{
    WaitingForServer();
}

CClient& CModels::GetClient()
{
    return client;
}

void CModels::WaitingForServer()
{
    if(!m_isWaitingForServer) {
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
