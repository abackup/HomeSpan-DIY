#pragma once
#include "HomeSpan.h"
#include "driver/temperature_sensor.h"

#define TEMP_LOOP_TIME 1200000 // ESP32C3内核温度传感器20分钟（1200000）更新一次状态

temperature_sensor_handle_t temp_sensor_handle = NULL;
float tsens_value = 0;

class CoreTemp
{
public:
    void begin()
    {
        temperature_sensor_config_t temp_sensor_config = {
            .range_min = -10,
            .range_max = 80,
        };
        esp_err_t err = temperature_sensor_install(&temp_sensor_config, &temp_sensor_handle);
        err = temperature_sensor_enable(temp_sensor_handle);
    }
};

CoreTemp coreTemp;

struct DEV_CoreTemp : Service::TemperatureSensor
{
    SpanCharacteristic *temp;
    SpanCharacteristic *statusActive;

    DEV_CoreTemp() : Service::TemperatureSensor()
    {
        temperature_sensor_get_celsius(temp_sensor_handle, &tsens_value);
        temp = new Characteristic::CurrentTemperature(tsens_value, true);
        statusActive = new Characteristic::StatusActive(1, true); // 1表示在运行，0表示未在运行
        temp->setRange(-10, 80);
    }

    void loop() override
    {

        if (temp->timeVal() > TEMP_LOOP_TIME)
        {
            // esp_err_t err = temperature_sensor_get_celsius(temp_sensor_handle, &tsens_value);
            temperature_sensor_get_celsius(temp_sensor_handle, &tsens_value);
            temp->setVal(tsens_value);
        }
    }
};
