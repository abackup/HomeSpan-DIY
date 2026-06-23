#include "HomeSpan.h"         // Always start by including the HomeSpan library

void setup() {

  Serial.begin(115200);

  homeSpan.begin(Category::Lighting,"HomeSpan LightBulb");   // initializes a HomeSpan device named "HomeSpan Lightbulb" with Category set to Lighting

  new SpanAccessory();                              // Begin by creating a new Accessory using SpanAccessory(), no arguments needed
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service, with the required Identify Characteristic
      new Characteristic::Identify();                 // Create the required Identify  

    new Service::LightBulb();                       // Create the Light Bulb Service
      new Characteristic::On();                       // This Service requires the "On" Characterstic to turn the light on and off
} 

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();         // run HomeSpan!
  
} 
