#include <Arduino.h>
#include <Wire.h>

#define MAX_SENSORS 10

uint8_t numOfSens = 0;

typedef struct {

String name;

uint8_t address;

uint8_t regByte;

uint8_t nrBytesReturned = ;

uint8_t values[nrBytesReturned];
  
} Instruction ;



typedef struct{

String name;

uint8_t address;

Instruction instructions[];
  
}Sensor;



//return (int16_t)(th << 8 | tl); bij 2 bytes returned
//return (int32_t)(int8_t)ph << 16 | (uint16_t)pl << 8 | pxl; bij 3 bytes returned


Sensor sensors[MAX_SENSORS];



uint8_t addSensor(String name, uint8_t address);

uint8_t addInstruction();

uint8_t readSensor ();