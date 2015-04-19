#ifndef CDSETTING_H
#define CDSETTING_H
#include <string>

struct CDSettingData {
    public:
        CDSettingData();
        char ipServer[11];
        char portServer[5];
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

    void SetIp(std::string ip);
    void SetPort(std::string port);

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
