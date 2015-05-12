#ifndef CMODELS_H
#define CMODELS_H
#include "Client.h"
#include "Convert.h"



class CModels
{
    public:
        static CModels& getInstance() {
            static CModels instance;
            return instance;
        }
         static DWORD WINAPI StaticThreadWaitingForServer(void* Param) {
            CModels * This = (CModels*) Param;
            return This->ThreadWaitingForServer();
        }
        static DWORD WINAPI StaticThreadUpdateWeether(void* Param) {
            CModels * This = (CModels*) Param;
            return This->ThreadUpdateWeether();
        }
        void Init(void);
        CClient& GetClient();
        void WaitingForServer();
        void UpdateWeether();
        bool IsConnect() { return client.IsConnect(); }
        bool GetWeether(const std::string& nameCity, DataWeether & data);
        bool GetWeether(const std::string& nameCity, DataWeetherNextHours& data);
        std::string GetTopicalWeather(const std::string& nameCity);
    protected:
    private:
        CModels();
        virtual ~CModels() {}
        void operator= (CModels const&);
        DWORD m_WaitingForServerThreadID;
        HANDLE  m_handlThread;
        DWORD  m_ThreadUpdateWeether;
        DWORD WINAPI ThreadWaitingForServer();
        DWORD WINAPI ThreadUpdateWeether();
        CClient client;
        bool m_isWaitingForServer;
        bool m_isUpdateWeather;

        DataWeether m_data;
        DataWeetherNextHours m_dataList;
};

#endif // CMODELS_H