#include <Arduino.h>
#include "I2Ccontroller.h"

Controller controller;

void testJsonAdd()
{
  DynamicJsonDocument doc(1024);
  DynamicJsonDocument doc2(1024);
  DynamicJsonDocument doc3(1024);

  char json[] = "{\"type\":1,\"sensNr\":1,\"sensAdd\":104}";
  char json2[] = "{\"type\":2,\"sensNr\":1,\"instNr\":1,\"reqB\":59,\"BRet\":14}";
  char json3[] = "{\"type\":4,\"sensAdd\":104,\"reqB\":107,\"bNr\":6,\"bVal\":1}";

  deserializeJson(doc, json);
  controller.parseData(doc);

  delay(100);
  
  deserializeJson(doc2, json2);
  controller.parseData(doc2);

  delay(100);
  
  deserializeJson(doc3, json3);
  controller.parseData(doc3);

  doc.clear();
  doc2.clear();
  doc3.clear();
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
    controller.refresh();
    controller.printSensors();
    
    lastMillis = millis();
     Serial.println("Here");
  }

  delay(100);

}