
#define saveTime 100

#define maxNameLength 8
#define IDLength 1
#define paramLength 3
#define modSourceLength 1
#define stateLength 1
#define numLoopLength 1

#define presetSize 112

struct aKnub{
  char name[maxNameLength];
  byte params[3];
  byte modSource;
  byte state;
  byte numLoop;
};

typedef struct aKnub aKnub;

struct aKnubPreset{
  
  char name[maxNameLength];
  byte ID;
  aKnub knubbies[8];
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
 
 
void readKnubPresetName( int deviceaddress, unsigned int eeaddress, aKnubPreset *kpreset) {
    
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    
    Wire.requestFrom(deviceaddress,maxNameLength);
    int c = 0;
    for ( c = 0; c < maxNameLength; c++ )
      if (Wire.available()) kpreset->name[c] = Wire.read();
}

void readKnubPresetID( int deviceaddress, unsigned int eeaddress, aKnubPreset *kpreset) {
    
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    
    Wire.requestFrom(deviceaddress,IDLength);
   
      if (Wire.available()) kpreset->ID = Wire.read();
}

void readKnubbieName( int deviceaddress, unsigned int eeaddress, aKnubPreset *kpreset, uint8_t knubbieIndx) {
    
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    
    Wire.requestFrom(deviceaddress,maxNameLength);
    int c = 0;
    for ( c = 0; c < maxNameLength; c++ )
      if (Wire.available()) kpreset->knubbies[knubbieIndx].name[c] = Wire.read();
}

void readKnubbieParams(int deviceaddress, unsigned int eeaddress, aKnubPreset *kpreset, uint8_t knubbieIndx){


  Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    
    Wire.requestFrom(deviceaddress,paramLength);
    int c = 0;
    for ( c = 0; c < paramLength; c++ )
      if (Wire.available()) kpreset->knubbies[knubbieIndx].params[c] = Wire.read();
}

void readKnubbieModSource(int deviceaddress, unsigned int eeaddress, aKnubPreset *kpreset, uint8_t knubbieIndx){


  Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    
    Wire.requestFrom(deviceaddress,1);
  
      if (Wire.available()) kpreset->knubbies[knubbieIndx].modSource = Wire.read();
}
void readKnubbieModState(int deviceaddress, unsigned int eeaddress, aKnubPreset *kpreset, uint8_t knubbieIndx){


  Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    
    Wire.requestFrom(deviceaddress,1);
  
      if (Wire.available()) kpreset->knubbies[knubbieIndx].state = Wire.read();
}
void readKnubbienumLoop(int deviceaddress, unsigned int eeaddress, aKnubPreset *kpreset, uint8_t knubbieIndx){


  Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    
    Wire.requestFrom(deviceaddress,1);
  
      if (Wire.available()) kpreset->knubbies[knubbieIndx].numLoop = Wire.read();
}



void readKnubFullPreset(int deviceaddress, unsigned int eeaddress, aKnubPreset *kpreset){


  readKnubPresetName(deviceaddress, eeaddress, kpreset);
  readKnubPresetId(deviceaddress, eeaddress + maxNameLength, kpreset);
  
  for(int i = 0; i<8; i++){
  
    readKnubbieName(devideaddress, eeadress)
  
  
  
  }





}
