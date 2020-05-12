#include "I2Ccontroller.h"

uint8_t addSensor(char &name, int  address){

  if(name != nullptr && address != NULL){
  

      numOfSens += 1;
      return 1;

  }

  return 0;
}

uint8_t addInstruction(char sensorname[], char instrucionname[], 
    uint8_t reg , uint8_t nrbytesreturned)
{

     if(sensorname != nullptr && instrucionname != "" && reg != NULL 
        && nrbytesreturned != NULL)
    {
        for(int i = 0; i <= MAX_SENSORS; i ++)
        {
            if(sensors[i].name == sensorname)
            {
                sensors[i].instructions[sensors[i].numberInstructions].name = instrucionname;
                sensors[i].instructions[sensors[i].numberInstructions].reg = reg;
                sensors[i].instructions[sensors[i].numberInstructions].nrBytesReturned = nrbytesreturned;
                sensors[i].numberInstructions += 1;
                return 1;
            }
        }
    }
    return 0;
}