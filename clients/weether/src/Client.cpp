#include "Client.h"
#include <winsock2.h>
#include <windows.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <conio.h>
#include "CDSetting.h"
#include "CDLog.h"

CClient::CClient()
{
    m_ipAddres = (char*)"127.0.0.1";
    m_port = 1101;
}

CClient::~CClient()
{
    //dtor
}

bool CClient::Send(std::string msg)
{
    int bytecount;
    if( (bytecount=send(m_hsock, msg.c_str(), strlen(msg.c_str()),0))==SOCKET_ERROR){
        fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
        return false;
    }
    printf("Sent cmd %s bytes %d\n", msg.c_str(), bytecount);
    return true;
}

std::string CClient::CommandCreate(CMD cmd, std::string parm1)
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

void CClient::CommandParser(std::string command) {
    CDLog::Write( __FUNCTION__ , __LINE__, Info, "CClient: " + command );
    if (command.length() > 4) {
        std::string cmd = command.substr(0, 5);

        if (cmd == "00001") {
            printf("[%s][%d] command CMD_GET_NAME_CLIENT:%s\n" , __FUNCTION__, __LINE__, cmd.c_str() );
             Send(CommandCreate(CMD_SET_NAME_CLIENT, CDSetting::getInstance().getSetting().nameApplication));
        } else {
            printf("[%s][%d] command nieznana:%s\n" , __FUNCTION__, __LINE__, cmd.c_str() );

        }
        /*
        CDLog::Write( __FUNCTION__ , __LINE__, Info, "cmd:" + cmd );
        int intCmd = atoi(cmd.c_str());
        switch (intCmd) {
            case CMD_GET_NAME_CLIENT : {
                //CDLog::Write( __FUNCTION__ , __LINE__, Info,  "cmd: CMD_GET_NAME_CLIENT", intCmd );
                printf("[%s][%d] command CMD_GET_NAME_CLIENT:%d" , __FUNCTION__, __LINE__, intCmd );
               // this->Send(this->CommandCreate(CMD_SET_NAME_CLIENT, CDSetting::getInstance().getSetting().nameApplication));
                break;
            }

            default : {
                break;
            }
        } */
    } else {
         printf("[%s][%d] command error: %s\n" , __FUNCTION__, __LINE__, command.c_str() );
    }
}

DWORD WINAPI CClient::ThreadStart()
{
    while(true)
    {
        char buffer[1024];
        int buffer_len = 1024;
        int bytecount;

     //   int c;
        memset(buffer, '\0', buffer_len);

       // strcpy(buffer, CDSetting::getInstance().getSetting().nameApplication.c_str());
/*
        for(char* p=buffer ; (c=getch())!=13 ; p++){
            printf("%c", c);
            *p = c;
        }

        if(strcmp(buffer,"exit") == 0) break;

        if( (bytecount=send(m_hsock, buffer, strlen(buffer),0))==SOCKET_ERROR){
            fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
            return false;
        }
        printf("Sent bytes %d\n", bytecount);
*/
        if((bytecount = recv(m_hsock, buffer, buffer_len, 0))==SOCKET_ERROR){
            fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
            break;
        }
        this->CommandParser(std::string(buffer));
        //printf("Recieved bytes %d\nReceived string \"%s\"\n", bytecount, buffer);
    }

    return closesocket(m_hsock);
}

void CClient::Connect()
{
    CreateThread(0,0,StaticThreadStart, (void*)this , 0,&m_threadID);
}

bool CClient::Init() {
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

    //Connect to the server
    struct sockaddr_in my_addr;

    my_addr.sin_family = AF_INET ;
    my_addr.sin_port = htons(m_port);

    memset(&(my_addr.sin_zero), 0, 8);
    my_addr.sin_addr.s_addr = inet_addr(m_ipAddres);

    if( connect( m_hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == SOCKET_ERROR ){
        fprintf(stderr, "Error connecting socket %d\n", WSAGetLastError());
        return false;
    }

    return true;
}
