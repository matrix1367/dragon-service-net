#ifndef CDSETTING_H
#define CDSETTING_H
#include <string>

struct CDSettingData {
    public:
        CDSettingData();
        std::string nameApplication;
};

class CDSetting
{
public:

    static CDSetting& getInstance()
    {
        static CDSetting instance;
        return instance;
    }

    void Save();
    void Load();

    CDSettingData getSetting() {
        return settingData;
    }

protected:
private:
    CDSetting();
    CDSetting(CDSetting const &);
    void operator=(CDSetting const &);
    virtual ~CDSetting();

    CDSettingData settingData;
};

#endif // CDSETTING_H
