#ifndef CDSETTINGAPP_H
#define CDSETTINGAPP_H

#include "CDSettingData.h"

class CDSettingApp
{
    public:
        static CDSettingApp& getInstance()
        {
            static CDSettingApp instance;
            return instance;
        }

        int Save();
        int Load();
        inline CDSettingData GetSettingData() { return settingData; }
    protected:
    private:
        CDSettingApp();
        CDSettingApp(CDSettingApp const &);
        void operator=(CDSettingApp const &);
        virtual ~CDSettingApp();

        CDSettingData settingData;
};

#endif // CDSETTINGAPP_H
