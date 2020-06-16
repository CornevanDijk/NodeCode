#include "I2Ccontroller.h"


void Controller::begin()
{
    Wire.begin();
    Serial.begin(9600);
}


 /* SENSORS */

uint8_t Controller::addSensor(uint8_t *sensornumber, uint8_t *address)
{   
    uint8_t succes = 0;
    if (sensornumber != 0 || address != 0)
    {
        if (numOfSens < MAX_SENSORS){
            sensors[numOfSens].sensorNumber = *sensornumber;
            sensors[numOfSens].sensorAddress = *address;
            numOfSens =+ 1;
            succes = 1;
        }
    }
  return succes;
}

uint8_t Controller:: addInstruction(uint8_t *sensornumber, uint8_t *instructionnumber , uint8_t *requestfirst, uint8_t *registeraddress, uint8_t *bitnum, bool *bitvalue, uint8_t *nrbytesreturned)
{
    uint8_t succes = 0;

    if(*sensornumber != 0 || *instructionnumber != 0 ||  *registeraddress != 0)
    {
        for (int i = 0; i < numOfSens; i++)
        {
            if(sensors[i].sensorNumber == *sensornumber)
            {
               
                sensors[i].sensorInstructions[sensors[i].nrInstructions].instructionNumber = *instructionnumber;
                sensors[i].sensorInstructions[sensors[i].nrInstructions].requestFirst = *requestfirst;
                sensors[i].sensorInstructions[sensors[i].nrInstructions].registerAddress = *registeraddress;
                sensors[i].sensorInstructions[sensors[i].nrInstructions].bitNum = *bitnum;
                sensors[i].sensorInstructions[sensors[i].nrInstructions].bitValue = *bitvalue;
                sensors[i].sensorInstructions[sensors[i].nrInstructions].nrBytesReturned = *nrbytesreturned;
                sensors[i].nrInstructions += 1;

                succes = 1;
            }
        }
    }
    return succes;
}

// Voor testen
void Controller::printSensors()
{
    DynamicJsonDocument doc(2048);

    for (int i = 0; i < MAX_SENSORS; i++)
    {
        if(sensors[i].sensorNumber != 0)
        {
            for (int j = 0; j < MAX_INSTRUCTIONS; j++)
            {
                if(sensors[i].sensorInstructions[j].instructionNumber != 0)
                {
                    doc["sensNr"].set(sensors[i].sensorNumber);
                    doc["instNr"].set(sensors[i].sensorInstructions[j].instructionNumber);
                    doc["BRet"].set(sensors[i].sensorInstructions[j].nrBytesReturned);

                    for(int k = 0; k < sensors[i].sensorInstructions[j].nrBytesReturned; k++)
                    {
                        doc["Data"].add(sensors[i].sensorInstructions[j].values[k]);
                    }

                    serializeJson(doc, Serial);

                    doc.clear();
                }
            }
            
        }
    }
}

void Controller::refresh()
{
    for (int i = 0; i < MAX_SENSORS; i++)
    {
        if(sensors[i].sensorNumber != 0)
        {
            for (int j = 0; j < MAX_INSTRUCTIONS; j++)
            {
                if(sensors[i].sensorInstructions[j].instructionNumber != 0)
                {
                   if(sensors[i].sensorInstructions[j].requestFirst)
                   {
                       if(connection.writeBit(sensors[i].sensorAddress, sensors[i].sensorInstructions[j].registerAddress, sensors[i].sensorInstructions[j].bitNum, sensors[i].sensorInstructions[j].bitValue) == 0)
                       {
                           break;
                       }
                   }

                   if(connection.readBytes(sensors[i].sensorAddress, sensors[i].sensorInstructions[j].registerAddress, sensors[i].sensorInstructions[j].nrBytesReturned, sensors[i].sensorInstructions[j].values, 2000))
                   {
                       if(sendData(&sensors[i].sensorNumber, &sensors[i].sensorInstructions[j].instructionNumber, sensors[i].sensorInstructions[j].values, &sensors[i].sensorInstructions[j].nrBytesReturned) == 0)
                       {
                           break;
                       }
                   }
                }
            }
        }
    }
}

/* COMMUNICATION */

uint8_t Controller::sendData(uint8_t *sensornumber, uint8_t *instructionnumber, uint8_t values[], uint8_t *nrbytesreturned)
{
    uint8_t succes = 0;

    DynamicJsonDocument doc(1024);

    doc["sensNr"].set(*sensornumber);
    doc["instNr"].set(*instructionnumber);
    doc["BRet"].set(*nrbytesreturned);

    for(uint8_t k = 0; k <= *nrbytesreturned; k++)
    {
        doc["Data"].add(values[k]);
    }

    if(serializeJson(doc, Serial))
    {
        succes =1;
    }

    doc.clear();
    return succes;
}

uint8_t Controller::receiveData()
{
    uint8_t succes = 0;

    String buffer = Serial.readString();
    
    DynamicJsonDocument doc(1024);

    DeserializationError err = deserializeJson(doc, buffer);

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
    int commandType = doc["type"].as<int>();
    uint8_t instructionType = doc["inType"].as<uint8_t>();
    uint8_t sensorNr = doc["sensNr"].as<int>();
    uint8_t sensorAdd = doc["sensAdd"].as<uint8_t>();
    uint8_t instructionNr = doc["instNr"].as<uint8_t>();
    uint8_t registerAddress = doc["reqB"]. as<uint8_t>();
    uint8_t nrBReturned = doc["BRet"].as<uint8_t>();
    uint8_t bitNr = doc["bNr"].as<uint8_t>();
    bool bitValue = doc["bVal"].as<bool>();

    switch (commandType)
    {
    case 1: /* Adding sensor */
        if(addSensor(&sensorNr, &sensorAdd))
        {
            succes = 1;
             Serial.println("Added Sensor");
            break;
        }
        return 0;
        break;
    
    case 2: /* Adding instruction */
    
        if(addInstruction(&sensorNr, &instructionNr, &instructionType, &registerAddress, &bitNr, &bitValue, &nrBReturned ))
        {
            succes = 1;
             Serial.println("Added Instruction");
            break;
        }
        break;
    
    case 3: /* Adding simple instruction */
         if(connection.writeBit(sensorAdd, registerAddress, bitNr, bitValue))
        {
            succes = 1;
             Serial.println("Executed custom instruction");
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
    if(Serial.available())
    {
      uint8_t succes = 0;

        String buffer = Serial.readString();
        
        DynamicJsonDocument doc(1024);

        DeserializationError err = deserializeJson(doc, buffer);
        
        
        if(err == DeserializationError::Ok){
        
            if(parseData(doc))
            {
                succes = 1;
            }
        }
    }
}