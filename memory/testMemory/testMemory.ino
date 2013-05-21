#include <Wire.h>
#include "memory.h"

#define eepromAddr1  B01010000

byte saveMe = 127;
byte loadMe;

void setup(){

  Serial.begin(9600);

  
  writeSingleKnub(eepromAddr1, 0, saveMe);
  delay(50);
  }

}


void loop(){

  loadMe = readSingleKnub(eepromAddr1, 0);
  delay(5);
  Serial.println(loadMe);

}


