#include "Server.h"
#include <cstdio>
#include <winsock2.h>

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
            CreateThread(0,0,&SocketHandler, (void*)csock , 0,0);
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

DWORD WINAPI SocketHandler(void* lp){
    int *csock = (int*)lp;

    char buffer[1024];
    int buffer_len = 1024;
    int bytecount;
    while(true) {
        memset(buffer, 0, buffer_len);
        if((bytecount = recv(*csock, buffer, buffer_len, 0))==SOCKET_ERROR){
            fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
            goto FINISH;
        }
        printf("Received bytes %d\nReceived string \"%s\"\n", bytecount, buffer);
        strcat(buffer, " SERVER ECHO");

        if((bytecount = send(*csock, buffer, strlen(buffer), 0))==SOCKET_ERROR){
            fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
            goto FINISH;
        }

        printf("Sent bytes %d\n", bytecount);
    }

FINISH:
    free(csock);
    return 0;
}
