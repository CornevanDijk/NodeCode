#include "I2Ccontroller.h"


void Controller::begin()
{
    Wire.begin();
    Serial.begin(9600);
}

 /* SENSORS */

uint8_t Controller::addSensor(uint8_t sensornumber, uint8_t address)
{   
    uint8_t succes = 0;
    if (sensornumber != 0 || address != 0)
    {

        if (numOfSens < MAX_SENSORS){
           
            
            sensors[numOfSens] = Sensor(sensornumber, address);
            numOfSens =+ 1;
            succes = 1;
            Serial.println("added sensor");
        }
    }
  return succes;
}

uint8_t Controller::addInstruction(uint8_t sensornumber, uint8_t instructionnumber ,uint8_t reqbyte, 
                                     uint8_t nrbytesreturned)
{
    uint8_t succes = 0;

    if(sensornumber != 0 || instructionnumber != 0 ||  reqbyte != 0
        || nrbytesreturned != 0)
    {
            Serial.println(sensors[0].sensorNumber);
        for (int i = 0; i < numOfSens; i++)
        {
            if(sensors[i].sensorNumber == sensornumber)
            {
               
                sensors[i].sensorInstructions[sensors[i].nrInstructions] = Instruction(instructionnumber, reqbyte, nrbytesreturned);
                sensors[i].nrInstructions += 1;

                succes = 1;
                Serial.println("added instruction");
            }
        }
    }
    return succes;
}


//Uitvoeren eenmalige instructies
uint8_t Controller::execInstruction(uint8_t sensoraddress, uint8_t reqbyte, uint8_t nrbytesreturned)
{
    uint8_t succes = 0;

    uint8_t values[nrbytesreturned];

    writeInReg(sensoraddress, reqbyte);

    delay(10);

    if(readFromReg(sensoraddress, nrbytesreturned, values))
    {
        succes = 1;
    }

    return succes;
}


// Voor testen
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

void Controller::refresh()
{
    Serial.println("checking sensors");
    for (int i = 0; i < numOfSens; i++)
    {
        for(int j = 0; j < sensors[i].nrInstructions; j++)
        {
            if(writeInReg(sensors[i].address, sensors[i].sensorInstructions[j].reqByte)){
                readFromReg(sensors[i].address, sensors[i].sensorInstructions[j].nrBytesReturned, sensors[i].sensorInstructions[j].values);
                //sendData(sensors[i], sensors[i].sensorInstructions[j]);
                  Serial.println("Done");

            }
        }
    }
}


uint8_t Controller::readFromReg( uint8_t address, uint8_t nrbytesreturned, uint8_t values[] )
{
    uint8_t succes = 1;

    Wire.requestFrom(address, (uint8_t)nrbytesreturned);

    while(Wire.available() < nrbytesreturned);


    for(int i = 0; i < nrbytesreturned; i++)
    {
        values[i] = Wire.read();
    }

    Wire.endTransmission();
    
    return succes;
}

uint8_t Controller::writeInReg(uint8_t address, uint8_t value)
{
    Wire.beginTransmission(address);
    Wire.write(value);
    Wire.endTransmission();
    
    return 1;
}

uint8_t Controller::writeSingleInReg(uint8_t sensoraddress, uint8_t regaddress, uint8_t bitNum, bool value)
{
    uint8_t succes = 0;
    uint8_t tempB;

    Wire.beginTransmission(sensoraddress);
    Wire.requestFrom(regaddress, 1);
    Wire.readBytes(&tempB, 1);

    tempB = (value != 0) ? (tempB | (1 << bitNum)) : (tempB & ~(1 << bitNum));

    if(Wire.write(tempB))
    {
        succes = 1;
    }
    Wire.endTransmission();

    return succes;
}


/* COMMUNICATION */

uint8_t Controller::sendData(Sensor sensor, Instruction instruction)
{
    uint8_t succes = 0;

    DynamicJsonDocument doc(1024);

    Serial.println(sensor.sensorNumber);
    Serial.println(sensor.address);

    delay(100);
    doc["SensNr"] = sensor.sensorNumber;
    doc["InsNr"] = instruction.instructionNumber;
    doc["BRet"] = instruction.nrBytesReturned;
    delay(100);
    serializeJson(doc, Serial);
    succes = 1;
    delay(100);
    doc.clear();

    Serial.println("KOMPO");

    return succes;
}

uint8_t Controller::receiveData()
{
    uint8_t succes = 0;

    DynamicJsonDocument doc(1024);

    DeserializationError err = deserializeJson(doc, Serial);

    if(err == DeserializationError::Ok){
        if(parseData(doc))
        {
            succes = 1;
        }
    }

    return succes;

}

uint8_t Controller::parseData(DynamicJsonDocument doc)
{
    uint8_t succes = 0;

    int instructionType = doc["type"].as<int>();
    uint8_t sensorNr = doc["sensNr"].as<uint8_t>();
    uint8_t sensorAdd = doc["sensAdd"].as<uint8_t>();
    uint8_t instructionNr = doc["instNr"].as<uint8_t>();
    uint8_t reqByte = doc["reqB"]. as<uint8_t>();
    uint8_t nrBReturned = doc["BRet"].as<uint8_t>();
    uint8_t bitNr = doc["bNr"].as<uint8_t>();
    bool bitValue = doc["bVal"].as<bool>();

    switch (instructionType)
    {
    case 1:
         Serial.println("adding sensor");
        
        if(addSensor(sensorNr, sensorAdd))
        {
            succes = 1;
           
            Serial.println(sensorNr);
            Serial.println(sensorAdd);
            break;
        }
        return 0;
        break;
    
    case 2:
        Serial.println("adding instruction");
        if(addInstruction(sensorNr, instructionNr,reqByte, nrBReturned ))
        {
            succes = 1;
            
            Serial.println(sensorNr);
            Serial.println(instructionNr);
            Serial.println(reqByte);
            Serial.println(nrBReturned);
            break;
        }
        break;
    
    case 3:

        if(execInstruction(sensorAdd, reqByte, nrBReturned))
        {
            succes = 1;
            break;
        }
        break;

    case 4:

        if(writeSingleInReg(sensorAdd, reqByte, bitNr, bitValue))
        {
            succes = 1;
            break;
        }
        break;
    
    default:
        break;
    }
    
    return succes;
}

void Controller::checkMessages()
{
    Serial.println("checking messages");
    if(Serial.available())
    {
        if(receiveData() == 0)
        {
            Serial.println("receive error");
        }
    }
}