#pragma once
#include "HomeSpan.h"
#include "ArduinoQWeather.h"

#define WEATHER_LOOP_TIME 3600000 // 天气信息60分钟（3600000）更新一次状态
#define AIR_LOOP_TIME 3600000     // 空气质量60分钟（3600000）更新一次状态

String API_KEY = "***************************";
String Location_ID = "1011******";
String API_Host = "*******gg.re.qweatherapi.com";
String Unit = "m"; // 公制-m/英制-i
String Lang = "en"; // 语言 英文-en/中文-zh
String Latitude = "**.**";
String Longitude = "***.**";

WeatherNow weatherNow;
AirQuality airQuality;

float iHum = 0;  // 外部记录湿度
float iTemp = 0; // 外部记录温度
int Aqi = 1;

class QWeather{
public:
    void begin()
    {
        weatherNow.config(API_Host, API_KEY, Location_ID, Unit, Lang);
        airQuality.config(API_Host, API_KEY, Latitude, Longitude, Lang);
    }
};

QWeather qWeather;

struct DEV_TempSensor : Service::TemperatureSensor
{
    SpanCharacteristic *temp;
    SpanCharacteristic *statusActive;

    DEV_TempSensor() : Service::TemperatureSensor()
    {
        if (weatherNow.get())
        {
            iTemp = weatherNow.getTemp();
            iHum = weatherNow.getHumidity();
        }
        temp = new Characteristic::CurrentTemperature(iTemp, true);
        statusActive = new Characteristic::StatusActive(1, true); // 1表示在运行，0表示未在运行
        temp->setRange(-20, 50);
    }
    void loop() override
    {

        if (temp->timeVal() > WEATHER_LOOP_TIME)
        {
            if (weatherNow.get())
            {
                iTemp = weatherNow.getTemp();
                iHum = weatherNow.getHumidity();
                temp->setVal(iTemp);
            }
        }
    }
};

struct DEV_HumSensor : Service::HumiditySensor
{
    SpanCharacteristic *hum;
    SpanCharacteristic *statusActive;

    DEV_HumSensor() : Service::HumiditySensor()
    {

        hum = new Characteristic::CurrentRelativeHumidity(iHum, true);
        statusActive = new Characteristic::StatusActive(1, true); // 1表示在运行，0表示未在运行
        hum->setRange(0, 100);
    }

    void loop() override
    {

        if (hum->timeVal() > (WEATHER_LOOP_TIME + 1000))
        {
            hum->setVal(iHum);
        }
    }
};

struct DEV_AirQuality : Service::AirQualitySensor
{
    SpanCharacteristic *api;                    // 空气质量
    SpanCharacteristic *ozoneDensity;           // 臭氧密度
    SpanCharacteristic *nitrogenDioxideDensity; // 二氧化氮密度
    SpanCharacteristic *sulphurDioxideDensity;  // 二氧化硫密度
    SpanCharacteristic *pm25Density;            // PM25浓度
    SpanCharacteristic *pm10Density;            // PM10浓度
    SpanCharacteristic *statusActive;

    DEV_AirQuality() : Service::AirQualitySensor()
    {

        int Pm10 = 0;
        int Pm2p5 = 0;
        int No2 = 0;
        int So2 = 0;
        int O3 = 0;

        if (airQuality.get())
        {
            int beforeAqi = airQuality.getAqi();
            Pm10 = airQuality.getPm10();
            Pm2p5 = airQuality.getPm2p5();
            No2 = airQuality.getNo2();
            So2 = airQuality.getSo2();
            O3 = airQuality.getO3();
            if (beforeAqi > 200)
            {
                Aqi = 5;
            }
            else if (beforeAqi > 150)
            {
                Aqi = 4;
            }
            else if (beforeAqi > 100)
            {
                Aqi = 3;
            }
            else if (beforeAqi > 50)
            {
                Aqi = 2;
            }
            else if (beforeAqi > 0)
            {
                Aqi = 1;
            }
        }
        api = new Characteristic::AirQuality(Aqi, true);
        ozoneDensity = new Characteristic::OzoneDensity(O3, true);
        nitrogenDioxideDensity = new Characteristic::NitrogenDioxideDensity(No2, true);
        sulphurDioxideDensity = new Characteristic::SulphurDioxideDensity(So2, true);
        pm25Density = new Characteristic::PM25Density(Pm2p5, true);
        pm10Density = new Characteristic::PM10Density(Pm10, true);
        statusActive = new Characteristic::StatusActive(1, true); // 1表示在运行，0表示未在运行
    }

    void loop() override
    {

        if (statusActive->timeVal() > AIR_LOOP_TIME)
        {
            if (airQuality.get())
            {
                int beforeAqi = airQuality.getAqi();
                int Pm10 = airQuality.getPm10();
                int Pm2p5 = airQuality.getPm2p5();
                int No2 = airQuality.getNo2();
                int So2 = airQuality.getSo2();
                int O3 = airQuality.getO3();
                if (beforeAqi > 200)
                {
                    Aqi = 5;
                }
                else if (beforeAqi > 150)
                {
                    Aqi = 4;
                }
                else if (beforeAqi > 100)
                {
                    Aqi = 3;
                }
                else if (beforeAqi > 50)
                {
                    Aqi = 2;
                }
                else if (beforeAqi > 0)
                {
                    Aqi = 1;
                }
                else
                {
                    Aqi = 0;
                }
                api->setVal(Aqi);
                nitrogenDioxideDensity->setVal(No2);
                sulphurDioxideDensity->setVal(So2);
                pm25Density->setVal(Pm2p5);
                pm10Density->setVal(Pm10);
                ozoneDensity->setVal(O3);
                statusActive->setVal(1); // 1表示在运行
            }
            else
            {
                statusActive->setVal(0); // 0表示未在运行
            }
        }
    }
};
