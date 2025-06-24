#pragma once
#include "HomeSpan.h"


struct DEV_invertedLED : Blinkable // 创建一个派生自 Blinkable 的子类,反转状态LED灯的状态
{

    int pin;

    DEV_invertedLED(int pin) : pin{pin}
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }

    void on() override { digitalWrite(pin, LOW); }
    void off() override { digitalWrite(pin, HIGH); }
    int getPin() override { return (pin); }
};
