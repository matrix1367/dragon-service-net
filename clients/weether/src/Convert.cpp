#include "Convert.h"
#include "CDLog.h"
#include <json/json.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

CConvert::CConvert()
{
    //ctor
}

CConvert::~CConvert()
{
    //dtor
}


bool CConvert::ParserJSONToDataWeeter(std::string jsonDataWeether, DataWeether& data)
{
    Json::Value root;
    Json::Reader reader;
    bool parsedSuccess = reader.parse(jsonDataWeether,
                                   root,
                                   false);

    if (parsedSuccess) {
        time(&data.dateTimeUpdate);

        const Json::Value jsonName = root["name"];
        if (jsonName.isNull()) {  return false;  }
        strcpy(data.nameCity, jsonName.asCString());

        const Json::Value jsonMain = root["main"];
        if (jsonMain.isNull()) { return false;   }
        const Json::Value jsontemp = jsonMain["temp"];
        const Json::Value jsontemp_min = jsonMain["temp_min"];
        const Json::Value jsontemp_max = jsonMain["temp_max"];
        const Json::Value jsonPressure = jsonMain["pressure"];
        const Json::Value jsonsea_level = jsonMain["sea_level"];
        const Json::Value jsongrnd_level = jsonMain["grnd_level"];
        const Json::Value jsonhumidity = jsonMain["humidity"];

        if (jsonPressure.isNull() || !jsonPressure.isDouble()) { return false; };
        data.main.pressure =  jsonPressure.asDouble();

        if (jsontemp.isDouble()) data.main.temp = jsontemp.asDouble();
        if (jsontemp_min.isDouble()) data.main.temp_min = jsontemp_min.asDouble();
        if (jsontemp_max.isDouble()) data.main.temp_max = jsontemp_max.asDouble();
        if (jsonsea_level.isDouble()) data.main.see_level = jsonsea_level.asDouble();
        if (jsongrnd_level.isDouble()) data.main.grnd_level = jsongrnd_level.asDouble();
        if (jsonhumidity.isInt()) data.main.humidity = jsonhumidity.asInt();

        const Json::Value jsonWind = root["wind"];
        if (jsonWind.isNull()) { return false;   }
        const Json::Value jsonSpeed = jsonWind["speed"];
        const Json::Value jsontDeg = jsonWind["deg"];

        if (jsonSpeed.isDouble()) data.wind.speed = jsonSpeed.asDouble();
        if (jsontDeg.isDouble()) data.wind.deg = jsontDeg.asDouble();

        const Json::Value jsonWeather = root["weather"];
        if (jsonWeather.isArray()) {

            Json::Value::Members member;
            for ( Json::ValueIterator  it =  jsonWeather.begin();  it != jsonWeather.end(); it++  )
            {
                if ( (*it)["description"].isString()  )
                {
                    data.description = (*it)["description"].asString();
                }
                if ( (*it)["icon"].isString()  )
                {
                    data.icon = (*it)["icon"].asString();
                }
            }
        }

        return true;
    }
    return false;
}

bool CConvert::ParserJSONToDataWeeterList(std::string jsonDataWeether, DataWeetherNextHours& data)
{
    Json::Value root;
    Json::Reader reader;
    bool parsedSuccess = reader.parse(jsonDataWeether,
                                   root,
                                   false);

    if (parsedSuccess) {
        time(&data.dateTimeUpdate);
        const Json::Value jsonName = root["city"]["name"];
        if (!jsonName.isNull())
        {
            strcpy(data.nameCity, jsonName.asCString());
            printf(" city -> name:%s\n" , data.nameCity);
        }

        const Json::Value jsonList = root["list"];
        if (jsonList.isArray())
        {
            data.items.clear();
            for ( Json::ValueIterator  it =  jsonList.begin();  it != jsonList.end(); it++  )
            {
                 const Json::Value jsonDt_text =       (*it)["dt_txt"];
                 DataWeetherNextHoursItem item;
                 if (jsonDt_text.isString())
                 {
                    item.dt_text = jsonDt_text.asString();
                   // printf(">>>>>> dt_text: %s\n" , jsonDt_text.asCString());
                 }

                 const Json::Value jsonWeatherMain = (*it)["main"];
                 if (!jsonWeatherMain.isNull())
                 {
                    const Json::Value jsonWeatherMainTmp = jsonWeatherMain["temp"];
                    if (!jsonWeatherMainTmp.isNull())
                    {
                         item.main.temp =  (round((jsonWeatherMainTmp.asDouble() - 273.15) * 10))/10.0 ;
                    }
                    const Json::Value jsonWeatherMainPressure = jsonWeatherMain["pressure"];
                    if (!jsonWeatherMainPressure.isNull())
                    {
                         item.main.pressure = jsonWeatherMainPressure.asDouble();
                    }
                    const Json::Value jsonWeatherMain_sea_level = jsonWeatherMain["sea_level"];
                    if (!jsonWeatherMain_sea_level.isNull())
                    {
                         item.main.see_level = jsonWeatherMain_sea_level.asDouble();
                    }
                    const Json::Value jsonWeatherMain_grnd_level = jsonWeatherMain["grnd_level"];
                    if (!jsonWeatherMain_grnd_level.isNull())
                    {
                         item.main.grnd_level = jsonWeatherMain_grnd_level.asDouble();
                    }
                    const Json::Value jsonWeatherMain_humidity = jsonWeatherMain["humidity"];
                    if (!jsonWeatherMain_humidity.isNull())
                    {
                         item.main.humidity = jsonWeatherMain_humidity.asDouble();
                    }
                 } else printf(">>>>>>> jsonWeatherMain is NULL\n");

                 const Json::Value jsonWeatherWind = (*it)["wind"];
                 if (!jsonWeatherWind.isNull())
                 {
                    const Json::Value jsonWeatherWind_speed = jsonWeatherWind["speed"];
                    if (!jsonWeatherWind_speed.isNull())
                    {
                         item.wind.speed = jsonWeatherWind_speed.asDouble();
                    }
                    const Json::Value jsonWeatherWind_deg = jsonWeatherWind["deg"];
                    if (!jsonWeatherWind_deg.isNull())
                    {
                         item.wind.deg = jsonWeatherWind_deg.asDouble();
                    }
                 }else printf(">>>>>>> jsonWeatherWind is NULL\n");


                    const Json::Value jsonWeather = (*it)["weather"];
                    if (jsonWeather.isArray()) {
                        for ( Json::ValueIterator  it =  jsonWeather.begin();  it != jsonWeather.end(); it++  )
                        {
                            if ( (*it)["description"].isString()  )
                            {
                                item.description = (*it)["description"].asString();
                            }
                            if ( (*it)["icon"].isString()  )
                            {
                                item.icon = (*it)["icon"].asString();
                            }
                        }
                    }
/*
                 const Json::Value jsonWeatherWeather = (*it)["weather"];
                 if (!jsonWeatherWeather.isNull())
                 {
                    const Json::Value jsonWeatherWeather_description = jsonWeatherWeather["description"];
                    if (!jsonWeatherWeather_description.isNull())
                    {
                         item.description = jsonWeatherWeather_description.asString();
                    }
                    const Json::Value jsonWeatherWeather_icon = jsonWeatherWeather["icon"];
                    if (!jsonWeatherWeather_icon.isNull())
                    {
                         item.icon = jsonWeatherWeather_icon.asString();
                    }
                 } else printf(">>>>>>> jsonWeatherWeather is NULL\n");
*/

                data.items.push_back(item);
            }
        }

        return true;
    }
    return false;
}


std::string TDataWeetherMain::getStr_pressure()
{
    return CDLog::ToStringDouble( pressure );
}
#include <math.h>
std::string TDataWeetherMain::getStr_temp()
{
    return CDLog::ToStringDouble( (round((temp - 273.15) * 10))/10.0 );
}

std::string TDataWeetherMain::getStr_temp_max()
{
    return CDLog::ToStringDouble((round((temp_max - 273.15) * 10))/10.0);
}

std::string TDataWeetherMain::getStr_temp_min()
{
    return CDLog::ToStringDouble((round((temp_min - 273.15) * 10))/10.0);
}

std::string TDataWeetherMain::getStr_see_level()
{
    return CDLog::ToStringDouble(see_level);
}

std::string TDataWeetherMain::getStr_grnd_level()
{
    return CDLog::ToStringDouble(grnd_level);
}

std::string TDataWeetherMain::getStr_humidity()
{
    return CDLog::ToString(humidity);
}

std::string TDataWeetherWind::getStr_speed()
{
    return CDLog::ToString(speed);
}

std::string TDataWeetherWind::getStr_deg()
{
    return CDLog::ToString(deg);
}

std::string TDataWeether::getPathIcon()
{
    //TODO:
    std::string pathIcon = "F:\\projects\\dragon-service-net-master\\clients\\weether\\img\\" + icon + ".ico";
    return pathIcon;
}
