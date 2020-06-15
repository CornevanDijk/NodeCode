#include <Arduino.h>
#include "I2Ccontroller.h"

Controller controller;

void testJsonAdd()
{
  DynamicJsonDocument doc(1024);
  DynamicJsonDocument doc2(1024);
  DynamicJsonDocument doc3(1024);
  DynamicJsonDocument doc4(1024);

  char json[] = "{\"type\":1,\"sensNr\":1,\"sensAdd\":104}";
  char json2[] = "{\"type\":2,\"inType\":0,\"sensNr\":1,\"instNr\":1,\"reqB\":59,\"bNr\":0,\"bVal\":0,\"BRet\":14}";
  char json3[] = "{\"type\":3,\"sensAdd\":104,\"reqB\":107,\"bNr\":2,\"bVal\":0}";
  char json4[] = "{\"type\":3,\"sensAdd\":104,\"reqB\":107,\"bNr\":6,\"bVal\":0}";
  

  // deserializeJson(doc, json);
  // controller.parseData(doc);

  // delay(300);
  
  // deserializeJson(doc3, json3);
  // controller.parseData(doc3);
  
  // delay(300);

  // deserializeJson(doc4, json4);
  // controller.parseData(doc4);
  
  // delay(300);
  
  // deserializeJson(doc2, json2);
  // controller.parseData(doc2);

  // delay(100);
}

void setup() 
{
  controller.begin();
  testJsonAdd();
}

void loop() 
{
  static unsigned long lastMillis = 0;

  controller.checkMessages();

  if((millis() - lastMillis) > 1000)
  {
    //controller.printSensors();
    controller.refresh();
    
    lastMillis = millis();
  }

  delay(100);

}