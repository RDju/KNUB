#include <Wire.h>
#include "memory.h"

#define eepromAddr1  B01010000

byte saveMe = 255;
byte loadMe;

void setup(){

  Serial.begin(9600);

  for(unsigned int i = 0; i<5;i++){
  writeSingleKnub(eepromAddr1, i, saveMe);
  delay(50);
  }

}


void loop(){

  loadMe = readSingleKnub(eepromAddr1, 0);
  delay(5);
  Serial.println(loadMe);

}


