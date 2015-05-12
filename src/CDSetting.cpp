#include "CDSetting.h"
#include "CDSerialize.h"
#include "CDLog.h"
#include <windows.h>
#include "Shlwapi.h"

CDSettingData::CDSettingData()
{
    strcpy(ipServer, "127.0.0.1");
    strcpy(portServer, "1101");
}

void CDSetting::SetIp(std::string ip){
    strcpy(settingData.ipServer, ip.c_str());
}

void CDSetting::SetPort(std::string port) {
    strcpy(settingData.portServer, port.c_str());
}

CDSetting::CDSetting()
{
    Init();
}

CDSetting::~CDSetting()
{
    //dtor
}

void CDSetting::Init() {
    HMODULE hModule = GetModuleHandle(NULL);
    if (hModule != NULL)
    {
        char ownPth[MAX_PATH];
        GetModuleFileName(hModule,ownPth, (sizeof(ownPth)));
        PathRemoveFileSpec(ownPth);
        pathExe = std::string(ownPth) + '\\';

    } else {
         printf("pathe exe is  errror!\n" );
    }

}

void CDSetting::Save()
{
    //CDSettingData settingData(automaticCallMainDlL, automaticShowMainForm);
    CDSerialize<CDSettingData> serialzer(pathExe + "dragon_setting-net.dat");
    serialzer.Write(settingData);
}

void CDSetting::Load()
{
    CDSettingData settingDataLoad;
    CDSerialize<CDSettingData> serialzer(pathExe + "dragon_setting-net.dat");
    if(serialzer.Read(settingDataLoad)) {
            settingData = settingDataLoad;
    } else {
         CDLog::Write( __FUNCTION__ , __LINE__, Warning, "Wczytanie ustawien nie powiodlo sie." );
    }
}


