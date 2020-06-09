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
        }
    }
  return succes;
}

uint8_t Controller:: addInstruction(uint8_t sensornumber, uint8_t instructionnumber ,uint8_t registeraddress, uint8_t bitnum, bool bitvalue, uint8_t nrbytesreturned)
{
    uint8_t succes = 0;

    if(sensornumber != 0 || instructionnumber != 0 ||  registeraddress != 0
        || nrbytesreturned != 0)
    {
        for (int i = 0; i < numOfSens; i++)
        {
            if(sensors[i].sensorNumber == sensornumber)
            {
               
                sensors[i].sensorInstructions[sensors[i].nrInstructions] = Instruction(instructionnumber, registeraddress, bitnum, bitvalue, nrbytesreturned);
                sensors[i].nrInstructions += 1;

                succes = 1;
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
                    if(writeSingleInReg(sensors[i].sensorAddress, sensors[i].sensorInstructions[j].registerAddress, sensors[i].sensorInstructions[j].bitNum, sensors[i].sensorInstructions[j].bitValue))
                    {
                        if (readFromReg(sensors[i].sensorInstructions[j].registerAddress, sensors[i].sensorInstructions[j].nrBytesReturned, sensors[i].sensorInstructions[j].values))
                        {
                            sendData(sensors[i].sensorNumber, sensors[i].sensorInstructions[j].instructionNumber, sensors[i].sensorInstructions[j].values, sensors[i].sensorInstructions[j].nrBytesReturned);
                            Serial.println("yeet");
                        }
                    }
                }
            }
            
        }
    }
}

// void Controller::refresh2()
// {
//     for (int i = 0; i < MAX_SENSORS; i++)
//     {
//         if(sensors[i].sensorNumber != 0)
//         {
//             for (int j = 0; j < MAX_INSTRUCTIONS; j++)
//             {
//                 if(sensors[i].sensorInstructions[j].instructionNumber != 0)
//                 {
//                     if(connection.writeBit(sensors[i].sensorAddress, sensors[i].sensorInstructions[j].registerAddress, sensors[i].sensorInstructions[j].bitNum, sensors[i].sensorInstructions[j].bitValue))
//                     {
//                         if (connection.readBytes(sensors[i].sensorAddress, sensors[i].sensorInstructions[j].registerAddress, sensors[i].sensorInstructions[j].nrBytesReturned, sensors[i].sensorInstructions[j].values))
//                         {
//                             if(sensors[i].sensorInstructions[j].nrBytesReturned)
//                             {
//                                 sendData(sensors[i].sensorNumber, sensors[i].sensorInstructions[j].instructionNumber, sensors[i].sensorInstructions[j].values, sensors[i].sensorInstructions[j].nrBytesReturned);
//                                 Serial.println("yeet");
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }


uint8_t Controller::readFromReg( uint8_t address, uint8_t nrbytesreturned, uint8_t values[] )
{
    uint8_t succes = 1;

    Wire.requestFrom(address, (uint8_t)nrbytesreturned);

    unsigned long timer = millis();

    while(Wire.available() < nrbytesreturned)
    {
        if((millis() - timer) > 5000)
        {
            succes = 0;
            break;
        }
    };
    
    if(succes)
    {
        for(int i = 0; i < nrbytesreturned; i++)
        {
            values[i] = Wire.read();
        }
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

uint8_t Controller::sendData(uint8_t sensornumber, uint8_t instructionnumber, uint8_t values[], uint8_t nrbytesreturned)
{
    uint8_t succes = 0;

    DynamicJsonDocument doc(1024);

    doc["sensNr"].set(sensornumber);
    doc["instNr"].set(instructionnumber);
    doc["BRet"].set(nrbytesreturned);

    for(int k = 0; k < nrbytesreturned; k++)
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
    uint8_t registerAddress = doc["reqB"]. as<uint8_t>();
    uint8_t nrBReturned = doc["BRet"].as<uint8_t>();
    uint8_t bitNr = doc["bNr"].as<uint8_t>();
    bool bitValue = doc["bVal"].as<bool>();
    switch (instructionType)
    {
    case 1: /* Adding sensor */
        if(addSensor(sensorNr, sensorAdd))
        {
            succes = 1;
            break;
        }
        return 0;
        break;
    
    case 2: /* Adding instruction */
        if(addInstruction(sensorNr, instructionNr,registerAddress, bitNr, bitValue, nrBReturned ))
        {
            succes = 1;
            break;
        }
        break;
    
    case 3: /* Adding simple instruction */
         if(writeSingleInReg(sensorAdd, registerAddress, bitNr, bitValue))
        {
            succes = 1;
            break;
        }
        case 4: /* Executing simple instruction */
         if(writeSingleInReg(sensorAdd, registerAddress, bitNr, bitValue))
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
    if(Serial.available())
    {
        if(receiveData() == 0)
        {
            // Serial.println("receive error");
        }
    }
}


void Controller::returnFault(Sensor sensor, Instruction insutruction)
{

}