#ifndef CDSETTING_H
#define CDSETTING_H
#include <string>

struct CDSettingData {
    public:
        CDSettingData();
        char nameApplication[30];
        char ipServer[11];
        char portServer[5];
        char cityName[30];
        unsigned int updateWeather;
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
    void SetCityName(std::string name) ;

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