#include <Wire.h>


#include "knubFuncs.h"

int knubVal = 0;

void setup(){
  Wire.begin();
  Serial.begin(9600);

}


void loop(){

writeSingle(dacIDZ[1], write_cmds[0], 4095);
delay(500);
writeSingle(dacIDZ[1], write_cmds[0], 0);
delay(500);
writeSingle(dacIDZ[1], write_cmds[1], 4095);
delay(500);
writeSingle(dacIDZ[1], write_cmds[1], 0);
delay(500);

writeSingle(dacIDZ[1], write_cmds[2], 4095);
delay(500);
writeSingle(dacIDZ[1], write_cmds[2], 0);
delay(500);
writeSingle(dacIDZ[1], write_cmds[3], 4095);
delay(500);
writeSingle(dacIDZ[1], write_cmds[3], 0);
delay(500);
}




