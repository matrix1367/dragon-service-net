#include "CDSetting.h"
#include "CDSerialize.h"

#include <windows.h>

CDSettingData::CDSettingData()
{
    nameApplication = "Weather";
}

CDSetting::CDSetting()
{


}

CDSetting::~CDSetting()
{
    //dtor
}


void CDSetting::Save()
{
    //CDSettingData settingData(automaticCallMainDlL, automaticShowMainForm);
    CDSerialize<CDSettingData> serialzer("dragon_setting.dat");
    serialzer.Write(settingData);
}

void CDSetting::Load()
{
    CDSerialize<CDSettingData> serialzer("dragon_setting.dat");
    serialzer.Read(settingData);

}


