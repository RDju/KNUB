#include <Wire.h>

#define eepromAddr1  B01010000

byte saveMe = 127;


void setup(){

  Serial.begin(9600);


  writeSingleKnub(eepromAddr1, 0, saveMe);
}


void loop(){

  byte loadMe = readSingleKnub(eepromAddr1, 0);
  
  Serial.println(loadMe);

}

void writeSingleKnub(byte device, uint16_t address, byte knubValue){

  
  Wire.beginTransmission((device << 1)+0);
  Wire.write(lowByte(address));
  Wire.write(highByte(address));
  Wire.write(knubValue);
  Wire.endTransmission();

};

byte readSingleKnub(byte device, uint16_t address){

  byte rVal;
  
  Wire.beginTransmission((device << 1)+1);
  Wire.write(lowByte(address));
  Wire.write(highByte(address));
  Wire.endTransmission();

   Wire.requestFrom((int)device,1);
    if (Wire.available()) rVal = Wire.read();
  return rVal;
  
};

