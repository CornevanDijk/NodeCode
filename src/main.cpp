#include <Arduino.h>
#include "I2Ccontroller.h"

Controller controller;

void setup() 
{
  controller.begin();
}

void loop() 
{
  static unsigned long lastMillis = 0;

  controller.checkMessages();

  if((millis() - lastMillis) > 1000)
  {
    controller.refresh();
    
    lastMillis = millis();
  }


  delay(100);

}