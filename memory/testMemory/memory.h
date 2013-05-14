#include "Arduino.h"
/*

for easy memeory management use struct do define what knub preset is :

typedef struct knubPreset knubPreset;
struct knubPreset {

  ///


};
*/
struct aKnub{
  char name[8];
  byte value1;
  byte value2;
  byte curveType;
};
typedef struct aKnub aKnub;

struct fxPedal{

  char name[8];
  byte numKnub;
  char* knubsNames[4];
  aKnub knubs[4];
};
typedef struct fxPedal fxPedal;


struct aKnubPreset{
  
  char name[8];
  byte numFxPed;
  char* fxPedalsNames[8]; 
  fxPedal fxPedals[8];
};
typedef struct aKnubPreset aKnubPreset;




void writeSingleKnub(int device, unsigned int address, byte knubValue){

  int wrdata=knubValue;
  Wire.beginTransmission(device);
  Wire.write(int(address >> 8));
  Wire.write(int(address & 0xFF));
  Wire.write(knubValue);
  Wire.endTransmission();

  
}

byte readSingleKnub(int device, unsigned int address){

  byte rVal = 0xFF;
  
  Wire.beginTransmission(device);
   Wire.write(int(address >> 8));
  Wire.write(int(address & 0xFF));
  Wire.endTransmission();

   Wire.requestFrom(device,1);
    if (Wire.available()) rVal = Wire.read();
  return rVal;

}



void writeKnubPreset( int device,unsigned int address, byte *preset){

  Wire.beginTransmission(device);
  Wire.write(int(address >> 8));
  Wire.write(int(address & 0xFF));
  
  for(byte c = 0; c<8; c++){
  
    Wire.write(preset[c]);
  
  }
  Wire.endTransmission();
  delay(5);
}
void readKnubPreset(int device, unsigned int address, byte *preset){

  Wire.beginTransmission(device);
  Wire.write(int(address >> 8));
  Wire.write(int(address & 0xFF));
  Wire.endTransmission();
  
   Wire.requestFrom(address,8);
    
    for (byte  c = 0; c < 8; c++ )
      if (Wire.available()) preset[c] = Wire.read();
  delay(5);
}


