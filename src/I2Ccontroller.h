#include <Arduino.h>
#include <Wire.h>

#define MAX_SENSORS 10
#define MAX_LETTERS 10
#define MAX_INSTRUCTIONS 3

int numOfSens = 0;

typedef struct{

char name[MAX_LETTERS];

uint8_t reg;

int nrBytesReturned;

uint8_t values[nrBytesReturned];
  
}Instruction;

typedef struct{

char name[MAX_LETTERS];

int address;

int numberInstructions= 0;

Instruction instructions[MAX_INSTRUCTIONS];
  
}Sensor;



//return (int16_t)(th << 8 | tl); bij 2 bytes returned
//return (int32_t)(int8_t)ph << 16 | (uint16_t)pl << 8 | pxl; bij 3 bytes returned


Sensor sensors[MAX_SENSORS];

uint8_t addSensor();
uint8_t addInstruction();