#ifndef I2CCONTROLLER_H
#define I2CCONTROLLER_H

#include <Arduino.h>
#include <Wire.h>
#include <ArduinoJson.h>

#define MAX_SENSORS 10
#define MAX_INSTRUCTIONS 4
#define MAX_NR_BYTES_RETURNED 20



/* SENSORS */

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

    uint8_t values[MAX_NR_BYTES_RETURNED];

  
}  ;



struct Sensor{

    Sensor();

    Sensor(uint8_t sensornumber, uint8_t _address )
    {
        sensorNumber = sensornumber;
        address = _address;
    }

    ~Sensor()
    {
    }

    uint8_t sensorNumber = 0;

    uint8_t address = 0;

    uint8_t nrInstructions = 0; 

    Instruction sensorInstructions[];
    
} ;

/* COMMUNICATION */



class Controller 
{
    public:

    /* SENSORS */

    void begin();

    Controller(){};

    ~Controller(){};

    uint8_t addSensor(uint8_t sensornumber, uint8_t address);
    
    uint8_t addInstruction(uint8_t sensornumber, uint8_t instructionnumber ,uint8_t reqbyte, uint8_t nrbytesreturned);

    uint8_t execInstruction(uint8_t sensoraddress, uint8_t reqbyte, uint8_t nrbytesreturned);

    void printSensors();

    void refresh();
    
    Sensor sensors[];

    /* COMMUNICATION */

    void checkMessages();

     uint8_t parseData(DynamicJsonDocument doc);
    
    private:

    /* SENSORS */

    uint8_t numOfSens = 0;

    uint8_t writeInReg(uint8_t address, uint8_t value);

    uint8_t writeSingleInReg(uint8_t sensoraddress, uint8_t regaddress, uint8_t bitNum, bool value);

    uint8_t readFromReg (uint8_t address, uint8_t nrbytesreturned, uint8_t values[]);

    uint8_t readSensor(Sensor sensor, Instruction instruction);



    /* COMMUNICATION */

    uint8_t sendData(Sensor sensor, Instruction Instruction);

    uint8_t receiveData();

};





//return (int16_t)(th << 8 | tl); bij 2 bytes returned
//return (int32_t)(int8_t)ph << 16 | (uint16_t)pl << 8 | pxl; bij 3 bytes returned

#endif