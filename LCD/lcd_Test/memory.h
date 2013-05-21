
#define saveTime 100
#define maxNameLength 8

struct aKnub{
  char name[maxNameLength];
  uint8_t params[3];
};
typedef struct aKnub aKnub;

struct fxPedal{

  char name[maxNameLength];
  
  uint8_t numKnub;
  uint8_t isOn;
  aKnub knubs[4];
};
typedef struct fxPedal fxPedal;


struct aKnubPreset{
  
  char name[maxNameLength];
  uint8_t numFxPed;
  
  fxPedal fxPedals[8];
};
typedef struct aKnubPreset aKnubPreset;

void writeKnubPresetName( int device,unsigned int address, aKnubPreset * kpreset){

  Wire.beginTransmission(device);
  Wire.write(int(address >> 8));
  Wire.write(int(address & 0xFF));
  
  for(byte c = 0; c<maxNameLength; c++){
  
    Wire.write(kpreset->name[c]);
  
  }
  Wire.endTransmission();

}
void readKnubPresetName(int device, unsigned int address, aKnubPreset *kpreset){

  Wire.beginTransmission(device);
  Wire.write(int(address >> 8));
  Wire.write(int(address & 0xFF));
  Wire.endTransmission();
  
   Wire.requestFrom(address,maxNameLength);
    
    for (byte  c = 0; c < maxNameLength; c++ )
      if (Wire.available()) kpreset->name[c] = Wire.read();

}

void writeSingleKnub(int device, unsigned int address, byte knubValue){

  
  Wire.beginTransmission(device);
  Wire.write(int(address >> 8));
  Wire.write(int(address & 0xFF));
  Wire.write(knubValue);
  Wire.endTransmission();


}

byte readSingleKnub(int device, unsigned int address){

  byte rVal;
  
  Wire.beginTransmission(device);
   Wire.write(int(address >> 8));
  Wire.write(int(address & 0xFF));
  Wire.endTransmission();

   Wire.requestFrom(device,1);
    if (Wire.available()) rVal = Wire.read();
  return rVal;

}

