#pragma once
#include "HomeSpan.h"
#include "driver/temperature_sensor.h"

#define TEMP_LOOP_TIME 1200000 // 20分钟更新一次（单位：毫秒）

// ─────────────────────────────────────────────────────────────
// ESP32-C3 内核温度传感器封装类

class CoreTemp
{
private:
    temperature_sensor_handle_t handle = NULL;

public:
    bool begin()
    {
        temperature_sensor_config_t config = {
            .range_min = -10,
            .range_max = 80,
        };

        esp_err_t err = temperature_sensor_install(&config, &handle);
        if (err != ESP_OK)
        {

            Serial.printf("CoreTemp: Sensor install failed: %s\n", esp_err_to_name(err));
            // ESP_LOGE("CoreTemp", "Sensor install failed: %s\n", esp_err_to_name(err));
            return false;
        }

        err = temperature_sensor_enable(handle);
        if (err != ESP_OK)
        {
            Serial.printf("CoreTemp Sensor enable failed: %s\n", esp_err_to_name(err));
            // WEBLOG("CoreTemp Sensor enable failed: %s\n", esp_err_to_name(err));
            return false;
        }
        Serial.printf("CoreTemp Temperature sensor initialized successfully.\n");
        // WEBLOG("CoreTemp Temperature sensor initialized successfully.\n");
        return true;
    }

    bool read(float &outTemp)
    {
        if (!handle)
        {
            Serial.printf("CoreTemp Sensor handle not initialized.\n");
            // WEBLOG("CoreTemp Sensor handle not initialized.\n");
            return false;
        }

        esp_err_t err = temperature_sensor_get_celsius(handle, &outTemp);
        if (err != ESP_OK)
        {
            Serial.printf("CoreTemp Temperature read failed: %s\n", esp_err_to_name(err));
            // WEBLOG("CoreTemp Temperature read failed: %s\n", esp_err_to_name(err));
            return false;
        }

        return true;
    }

};

CoreTemp coreTemp; // 创建全局对象

// ─────────────────────────────────────────────────────────────
// HomeKit 温度传感器服务（内核温度）
struct DEV_CoreTemp : Service::TemperatureSensor
{
    SpanCharacteristic *temp;
    SpanCharacteristic *statusActive;

    DEV_CoreTemp() : Service::TemperatureSensor()
    {
        temp = new Characteristic::CurrentTemperature(0.0, true);
        statusActive = new Characteristic::StatusActive(1, true); // 初始为“在线”
        temp->setRange(-10.0, 80.0);

        if (!coreTemp.begin())
        {
            statusActive->setVal(false);
        }
    }

    void loop() override
    {
        if (temp->timeVal() > TEMP_LOOP_TIME)
        {
            float value = 0.0;
            if (coreTemp.read(value))
            {
                temp->setVal(value);
                statusActive->setVal(true);
                Serial.printf("CoreTemp Updated temp: %.2f°C\n", value);
                // WEBLOG("CoreTemp Updated temp: %.2f°C\n", value);
            }
            else
            {
                temp->setVal(0.0);
                statusActive->setVal(false);
                Serial.printf("CoreTemp Failed to read temperature sensor.\n");
                // WEBLOG("CoreTemp Failed to read temperature sensor.\n");
            }
        }
    }
};
