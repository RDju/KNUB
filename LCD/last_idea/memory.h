
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
  uint8_t ID;
  uint8_t numFxPed;
  
  fxPedal fxPedals[8];
};
typedef struct aKnubPreset aKnubPreset;



void writeByte( int deviceaddress, unsigned int eeaddress, byte data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
  }

 
  void writeKnubName( int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    byte c;
    for ( c = 0; c < maxNameLength; c++)
      Wire.write(kpreset->name[c]);
    Wire.endTransmission();
  }
  
  
  byte readByte( int deviceaddress, unsigned int eeaddress ) {
    byte rdata = 0x00;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
  }
 
 
  void readKnubName( int deviceaddress, unsigned int eeaddress, aKnubPreset *kpreset) {
    
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    
    Wire.requestFrom(deviceaddress,maxNameLength);
    int c = 0;
    for ( c = 0; c < maxNameLength; c++ )
      if (Wire.available()) kpreset->name[c] = Wire.read();
  }

