#ifndef I2CCONTROLLER_H
#define I2CCONTROLLER_H

#include <Arduino.h>
#include <Wire.h>
#include <ArduinoJson.h>

#include "I2Cdev.h"

#define MAX_SENSORS 10
#define MAX_INSTRUCTIONS 4
#define MAX_NR_BYTES_RETURNED 20
#define SENSOR_NAME_LENGTH 4

/* SENSORS */

struct Instruction {

    Instruction(){};

    Instruction(uint8_t instructionnumber, uint8_t registeraddress, uint8_t bitnum, bool bitvalue, uint8_t nrbytesReturned)
    {
        instructionNumber = instructionnumber;
        registerAddress = registeraddress;
        bitNum = bitnum;
        bitValue = bitvalue;
        nrBytesReturned = nrbytesReturned;
    }

    uint8_t instructionNumber = 0;

    uint8_t registerAddress = 0;

    uint8_t bitNum = 0;

    bool bitValue = 0;
    
    uint8_t nrBytesReturned = 0;
    
    uint8_t values[MAX_NR_BYTES_RETURNED];

  
}  ;



struct Sensor{

    Sensor(){};

    Sensor(uint8_t sensornumber, uint8_t sensoraddress )
    {
        sensorNumber = sensornumber;
        sensorAddress = sensoraddress;
    }

    ~Sensor()
    {

    }

    uint8_t sensorNumber = 0;

    uint8_t sensorAddress = 0;

    uint8_t nrInstructions = 0; 

    Instruction sensorInstructions[MAX_INSTRUCTIONS];
    
} ;

/* COMMUNICATION */



class Controller 
{
    public:

    /* SENSORS */

    void begin();

    Controller(){};

    ~Controller(){};

    void printSensors();

    void refresh();

    void refresh2();
    
    Sensor sensors[MAX_SENSORS];

    /* COMMUNICATION */

    void checkMessages();

    uint8_t parseData(DynamicJsonDocument doc);

    void simpleJson();
    
    private:

    /* SENSORS */

    //I2Cdev connection;

    uint8_t numOfSens = 0;

    uint8_t writeInReg(uint8_t address, uint8_t value);

    uint8_t writeSingleInReg(uint8_t sensoraddress, uint8_t regaddress, uint8_t bitNum, bool value);

    uint8_t readFromReg (uint8_t a0ddress, uint8_t nrbytesreturned, uint8_t values[]);

    uint8_t readSensor(Sensor sensor, Instruction instruction);

    uint8_t addSensor( uint8_t sensornumber, uint8_t address);
    
    uint8_t addInstruction(uint8_t sensornumber, uint8_t instructionnumber ,uint8_t registeraddress, uint8_t bitnum, bool bitvalue, uint8_t nrbytesreturned);

    uint8_t addInstruction(uint8_t sensornumber, uint8_t instructionnumber, uint8_t registeraddress, uint8_t nrbytesreturned);

    uint8_t execInstruction(uint8_t sensoraddress, uint8_t reqbyte, uint8_t nrbytesreturned);

    /* COMMUNICATION */

    uint8_t sendData(uint8_t sensornumber, uint8_t instructionnumber, uint8_t values[], uint8_t nrbytesreturned);

    uint8_t receiveData();

    void returnFault(Sensor sensor, Instruction insruction);

};
#endif