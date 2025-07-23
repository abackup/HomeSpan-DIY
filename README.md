# HomeSpan-DIY


* ESP32-C3 内核温度读取库 DEV_CoreTemp.h 已优化。
  
```C++
#include "DEV_CoreTemp.h"
  ...
  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Identify();

  coreTemp.begin(); // 初始化内核温度传感器
  new DEV_CoreTemp();
  ...
```
  
* 
* 控制反转状态指示灯库 DEV_invertedLED.h 已优化。

