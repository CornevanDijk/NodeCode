#ifndef I2CCONTROLLER_H
#define I2CCONTROLLER_H

#include <Arduino.h>
#include <Wire.h>


#define MAX_NR_BYTES_RETURNED 4

#define MAX_SENSORS 10


struct Instruction {

Instruction();

Instruction(uint8_t instructionnumber, uint8_t reqbyte, uint8_t nrbytesReturned)
{
    instructionNumber = instructionnumber;
    reqByte = reqbyte;
    nrBytesReturned = nrbytesReturned;
}



uint8_t instructionNumber = 0;

uint8_t reqByte = 0;

uint8_t nrBytesReturned = 0;

uint8_t values[];

  
}  ;



struct Sensor{

Sensor();

Sensor(uint8_t sensornumber, uint8_t _address )
{
    sensorNumber =sensornumber;
    address = _address;
}

uint8_t sensorNumber = 0;

uint8_t address;

uint8_t nrInstructions = 0; 

Instruction sensorInstructions[];
  
} ;



class Controller 
{
    public:

    void begin();

    Controller(){};

    uint8_t addSensor(uint8_t sensornumber, uint8_t address);
    
    uint8_t addInstruction(uint8_t sensornumber, uint8_t instructionnumber ,uint8_t reqbyte, uint8_t nrbytesReturned);
    
    uint8_t writeInReg(uint8_t address, uint8_t value);

    uint8_t readFromReg (uint8_t address, uint8_t nrbytesreturned, uint8_t values[]);

    void printSensors();

    void refresh();
    
    Sensor sensors[];
    
    private:

    uint8_t numOfSens = 0;

    

    uint8_t readSensor(Sensor sensor, Instruction instruction);
    
   

    
};





//return (int16_t)(th << 8 | tl); bij 2 bytes returned
//return (int32_t)(int8_t)ph << 16 | (uint16_t)pl << 8 | pxl; bij 3 bytes returned

#endif