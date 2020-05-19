#include <Arduino.h>
#include "I2Ccontroller.h"

Controller controller;

void addTestSensors()
{
  controller.addSensor(1, 0X68);
  controller.addInstruction(1, 1, 0x3B, 14);

  
}

void setup() 
{
  controller.begin();
  Serial.begin(9600);
}

void loop() 
{
  addTestSensors();
  Serial.println("SensorNumber");
  Serial.println(controller.sensors[0].sensorNumber);
  Serial.println("SensorAddress");
  Serial.println(controller.sensors[0].address); 
  delay(500);
  
  //controller.printSensors();
  controller.refresh();
  //controller.printSensors();

  for( int i = 0; i < 14; i++)
  {
    Serial.println("Byte");
    Serial.println(i);
    Serial.println(controller.sensors[0].sensorInstructions[0].values[i]);
  }
  while (1)
  {
    /* code */
  }
  
}