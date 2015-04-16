#include "CDSetting.h"
#include "CDSerialize.h"
#include "CDLog.h"
#include <windows.h>
#include "Shlwapi.h"

CDSettingData::CDSettingData()
{
   // nameApplication = "Weather";
    strcpy(nameApplication, "Weather");
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
        //printf("pathe: %s\n" , ownPth);
    } else {
         printf("pathe exe is  errror!\n" );
    }

}

void CDSetting::Save()
{
    //CDSettingData settingData(automaticCallMainDlL, automaticShowMainForm);
    CDSerialize<CDSettingData> serialzer(pathExe + "dragon_setting.dat");
    serialzer.Write(settingData);
}

void CDSetting::Load()
{
    CDSettingData settingDataLoad;
    CDSerialize<CDSettingData> serialzer(pathExe + "dragon_setting.dat");
    if(serialzer.Read(settingDataLoad)) {
            settingData = settingDataLoad;
    } else {
         CDLog::Write( __FUNCTION__ , __LINE__, Warning, "Wczytanie ustawieñ nie powiod³o siê." );
    }
}


