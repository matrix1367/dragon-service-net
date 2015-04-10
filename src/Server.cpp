#include "Server.h"
#include <cstdio>
#include <winsock2.h>
#include "EventManager.h"
#include "Tools.h"
#include "CDLog.h"
#include <string>

DWORD WINAPI SocketHandler(void*);

CServer::CServer()
{
    m_port = 1101;
    m_adresIP ="127.0.0.1";
}

CServer::~CServer()
{
    //dtor
}

std::list<SClient> CServer::GetClients()
{
    return m_clients;
}

DWORD WINAPI CServer::ThreadStart()
{
    int* csock;
    sockaddr_in sadr;
    int    addr_size = sizeof(SOCKADDR);

    while(true){
        printf("waiting for a connection\n");
        csock = (int*)malloc(sizeof(int));

        if((*csock = accept( m_hsock, (SOCKADDR*)&sadr, &addr_size))!= INVALID_SOCKET ){
            printf("Received connection from %s\n",inet_ntoa(sadr.sin_addr));
            SClient clientData;
            clientData.socketHandle = csock;
            clientData.ip = inet_ntoa(sadr.sin_addr);
            m_clients.push_back(clientData);

            CEventManager::getInstance().Send(EVENT_CLIENT_CONNECTION);

            this->SetTmpParam((void*)csock);
            CreateThread(0,0,&StaticThreadConnectClient, (void*)this , 0,0);
        }
        else{
            fprintf(stderr, "Error accepting %d\n",WSAGetLastError());
        }
    }
    return 0;
}

void CServer::Start(){
    CreateThread(0,0,StaticThreadStart, (void*)this , 0,&m_threadID);
}

bool CServer::Send(int* csock, std::string msg)
{
   int bytecount = 0;
   if((bytecount = send(*csock, msg.c_str(), strlen(msg.c_str()), 0))==SOCKET_ERROR){
        fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
        return false;
    }
    printf("Sent cmd %s bytes %d\n", msg.c_str(), bytecount);
    return true;
}



std::string CServer::CommandCreate(CMD cmd, std::string parm1)
{
    switch (cmd) {
    case CMD_GET_NAME_CLIENT:
        {
            return "00001|" + parm1 + "|";
        }
    case CMD_SET_NAME_CLIENT:
        {
            return "00002|" + parm1 + "|";
        }
        default: {
            return "00000|";
        }
    }
}

void CServer::CommandParser(int* csock, std::string command)
{
    CDLog::Write( __FUNCTION__ , __LINE__, Info, command.c_str() );
    if (command.length() > 4) {
        std::string cmd = command.substr(0,5);
        std::string parmsStr= command.substr(6, command.length() - cmd.length());
        std::vector<std::string> parms = split(const_cast<char *>(parmsStr.c_str()), "|");

        if (cmd == "00002") {
            CDLog::Write( __FUNCTION__ , __LINE__, Info, "cmd: CMD_SET_NAME_CLIENT");
            if (parms.size() == 1) {
                for (std::list<SClient>::iterator itClients = m_clients.begin() ; itClients != m_clients.end(); itClients++ )
                {
                    if (itClients->socketHandle == csock) {
                           printf("Ustawiono nazwe %s klienta %d\n", parms[0].c_str(), *csock);
                           itClients->name = parms[0];
                           //todo: stworzyc oddzielny event
                           CEventManager::getInstance().Send(EVENT_CLIENT_CONNECTION);
                           break;
                    }
                }
            }

          /*  for (unsigned int i = 0 ; i < parms.size() ; i ++) {
                printf("Parm %d: %s" , i, parms[i].c_str());
            }
            CDLog::Write( __FUNCTION__ , __LINE__, Info, "paramters:"+parms );
        */
        } else {
            CDLog::Write( __FUNCTION__ , __LINE__, Info, "komenda nieznana:" + cmd );
        }


    } else {
        CDLog::Write( __FUNCTION__ , __LINE__, Info, "komenda niewlasciwa:" + command );
    }
}


bool CServer::Init(void) {
    //Initialize socket support WINDOWS ONLY!
    WSADATA wsaData;
    int err = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
    if ( err != 0 || ( LOBYTE( wsaData.wVersion ) != 2 ||
            HIBYTE( wsaData.wVersion ) != 2 )) {
        fprintf(stderr, "Could not find useable sock dll %d\n",WSAGetLastError());
        return false;
    }

    //Initialize sockets and set any options
    int * p_int ;
    m_hsock = socket(AF_INET, SOCK_STREAM, 0);
    if(m_hsock == -1){
        printf("Error initializing socket %d\n",WSAGetLastError());
        return false;
    }

    p_int = (int*)malloc(sizeof(int));
    *p_int = 1;
    if( (setsockopt(m_hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
        (setsockopt(m_hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
        printf("Error setting options %d\n", WSAGetLastError());
        free(p_int);
       return false;
    }
    free(p_int);

    //Bind and listen
    struct sockaddr_in my_addr;

    my_addr.sin_family = AF_INET ;
    my_addr.sin_port = htons(m_port);

    memset(&(my_addr.sin_zero), 0, 8);
    my_addr.sin_addr.s_addr = INADDR_ANY ;

    if( bind( m_hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
        fprintf(stderr,"Error binding to socket, make sure nothing else is listening on this port %d\n",WSAGetLastError());
        return false;
    }
    if(listen( m_hsock, 10) == -1 ){
        fprintf(stderr, "Error listening %d\n",WSAGetLastError());
        return false;
    }

    return true;
}

DWORD WINAPI CServer::ThreadConnectClient(void* lp){
    int *csock = (int*)lp;

    this->Send(csock, this->CommandCreate(CMD_GET_NAME_CLIENT,""));


    char buffer[1024];
    int buffer_len = 1024;
    int bytecount;
    while(true) {
        memset(buffer, 0, buffer_len);
        if((bytecount = recv(*csock, buffer, buffer_len, 0))==SOCKET_ERROR){
            fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
            goto FINISH;
        }
        this->CommandParser(csock, std::string(buffer));
        //printf("Received bytes %d\nReceived string \"%s\"\n", bytecount, buffer);
/*        strcat(buffer, " SERVER ECHO");
        if((bytecount = send(*csock, buffer, strlen(buffer), 0))==SOCKET_ERROR){
            fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
            goto FINISH;
        }

        printf("Sent bytes %d\n", bytecount);
*/
    }

FINISH:

    for (std::list<SClient>::iterator itClients = m_clients.begin() ; itClients != m_clients.end(); itClients++ )
    {
        if (itClients->socketHandle == csock) {
               printf("Delete socket csock %d\n", *csock);
               m_clients.erase(itClients);
               break;
        }
    }

    CEventManager::getInstance().Send(EVENT_CLIENT_DISCONNECT);
    free(csock);
    return 0;
}
