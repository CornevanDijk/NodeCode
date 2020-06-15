#ifndef I2CCONTROLLER_H
#define I2CCONTROLLER_H

#include <Arduino.h>
#include <ArduinoJson.h>


#include "I2Cdev.h"

#define MAX_SENSORS 10
#define MAX_INSTRUCTIONS 4
#define MAX_NR_BYTES_RETURNED 20
#define SENSOR_NAME_LENGTH 4

/* SENSORS */

struct Instruction {

    Instruction(){};

    uint8_t requestFirst = 0;

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

class Controller 
{
    public:

    /* SENSORS */

    void begin();

    Controller(){};

    ~Controller(){};

    void printSensors();

    void refresh();
    
    Sensor sensors[MAX_SENSORS];

    /* COMMUNICATION */

    void checkMessages();

    uint8_t parseData(DynamicJsonDocument doc);

    private:

    /* SENSORS */

    I2Cdev connection;

    uint8_t numOfSens = 0;

    uint8_t addSensor( uint8_t *sensornumber, uint8_t *address);
    
    uint8_t addInstruction(uint8_t *sensornumber, uint8_t *instructionnumber ,uint8_t *requestfirst, uint8_t *registeraddress, uint8_t *bitnum, bool *bitvalue, uint8_t *nrbytesreturned);

    /* COMMUNICATION */

    uint8_t sendData(uint8_t *sensornumber, uint8_t *instructionnumber, uint8_t values[], uint8_t *nrbytesreturned);

    uint8_t receiveData();

};
#endif