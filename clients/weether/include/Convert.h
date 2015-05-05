#ifndef CCONVERT_H
#define CCONVERT_H
#include <string>
#include <cstdio>
#include <vector>

typedef struct TDataWeetherWind {
    double speed;
    double deg;

    std::string getStr_speed();
    std::string getStr_deg();
} DataWeetherWind;

typedef struct TDataWeetherMain {
    double pressure;
    double temp;
    double temp_max;
    double temp_min;
    double see_level;
    double grnd_level;
    int humidity;

    std::string getStr_pressure();
    std::string getStr_temp();
    std::string getStr_temp_max();
    std::string getStr_temp_min();
    std::string getStr_see_level();
    std::string getStr_grnd_level();
    std::string getStr_humidity();
} DataWeetherMain;

typedef struct TDataWeether {
    time_t dateTimeUpdate;
    char nameCity[30];
    std::string description;
    std::string icon;
    DataWeetherMain main;
    DataWeetherWind wind;
    std::string getPathIcon();
public:
    void Print() {
        printf("nameCity %s, wind.speed %f, description %s\n", nameCity, wind.speed, description.c_str());
    }
} DataWeether;

typedef struct TDataWeetherNextHoursItem {
    DataWeetherMain main;
    DataWeetherWind wind;
    time_t dt;
    std::string dt_text;
    std::string icon;
    std::string description;
} DataWeetherNextHoursItem;

typedef struct TDataWeetherNextHours {
    time_t dateTimeUpdate;
    char nameCity[30];
    std::vector<DataWeetherNextHoursItem> items;
    double* GetAllTemp();
    std::vector<int> GetAllTempINT();

} DataWeetherNextHours;

class CConvert
{
    public:
        CConvert();
        virtual ~CConvert();
        static bool ParserJSONToDataWeeter(std::string jsonDataWeether, DataWeether& data);
        static bool ParserJSONToDataWeeterList(std::string jsonDataWeether, DataWeetherNextHours& data);
    protected:
    private:
};

#endif // CCONVERT_H
