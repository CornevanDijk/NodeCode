#include "I2Ccontroller.h"


void Controller::begin()
{
    Wire.begin();
}

uint8_t Controller::addSensor(uint8_t sensornumber, uint8_t address)
{   
    if (sensornumber != 0 || address != 0)
    {

        if (numOfSens < MAX_SENSORS){
           
            
            sensors[numOfSens] = Sensor(sensornumber, address);
            //Serial.println(sensors[numOfSens].sensorName);
            numOfSens =+ 1;
            return 1;
        }
    }
  return 0;
}

//TO-DO instructions toevoegen

uint8_t Controller::addInstruction(uint8_t sensornumber, uint8_t instructionnumber ,uint8_t reqbyte, 
                                     uint8_t nrbytesReturned)
{

     if(sensornumber != 0 || instructionnumber != 0 ||  reqbyte != 0
        || nrbytesReturned != 0)
    {
        for (int i = 0; i < numOfSens; i++)
        {
            if(sensors[i].sensorNumber == sensornumber)
            {
                sensors[i].nrInstructions += 1;
                sensors[i].sensorInstructions[sensors[i].nrInstructions] = Instruction(instructionnumber, reqbyte, nrbytesReturned);
                
                return 1;
            }
        }
    }
    return 0;
}

void Controller::printSensors()
{
    for(int i =0; i < numOfSens; i++){
        if(sensors[i].sensorNumber != 0){
            Serial.println(sensors[i].sensorNumber);
            Serial.println(sensors[i].address);
            Serial.println(sensors[i].nrInstructions);
            for (int j = 0; j < sensors[i].nrInstructions; j++)
            {
                Serial.println(sensors[i].sensorInstructions[j].instructionNumber);
                Serial.println(sensors[i].sensorInstructions[j].nrBytesReturned);
                for(int l = 0; l < sensors[i].sensorInstructions[j].nrBytesReturned; l ++)
                {
                    Serial.println(sensors[i].sensorInstructions[j].values[l]);
                }
            }
            
            
        }
    }
}

//TO-DO uitlezen toeveogen

uint8_t Controller::readFromReg( uint8_t address, uint8_t nrbytesreturned, uint8_t values[] )
{
    Wire.requestFrom(address, (byte)nrbytesreturned);

    while(Wire.available() < nrbytesreturned);


    for(int i = 0; i < nrbytesreturned; i++)
    {
        values[i] = Wire.read();
    }
    return 1;
}

uint8_t Controller::writeInReg(uint8_t address, uint8_t value)
{
    Wire.beginTransmission(address);
    Wire.write(value);
    Wire.endTransmission();
    
    return 1;
}

void Controller::refresh()
{
    for (int i = 0; i < numOfSens; i++)
    {
        for(int j = 0; j < sensors[i].nrInstructions; j++)
        {
            if(writeInReg(sensors[i].address, sensors[i].sensorInstructions[j].reqByte)){
                readFromReg(sensors[i].address, sensors[i].sensorInstructions[j].nrBytesReturned, sensors[i].sensorInstructions[j].values);
            }
        }
    }
}