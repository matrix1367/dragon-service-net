#ifndef CDSETTING_H
#define CDSETTING_H
#include <string>

struct CDSettingData {
    public:
        CDSettingData();
        char nameApplication[30];
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

    void Init();

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
    std::string pathExe;
};

#endif // CDSETTING_H
