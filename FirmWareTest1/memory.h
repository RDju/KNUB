
#define saveTime 100

#define maxNameLength 8
#define IDLength 6
#define paramLength 3
#define modSourceLength 1
#define stateLength 1
#define numLoopLength 1
#define numKnubbies 8

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
  byte ID[6];
  
  aKnub knubbies[numKnubbies];
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

 
  void writeKnubPresetName( int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    byte c;
    for ( c = 0; c < maxNameLength; c++){
      Wire.write(kpreset->name[c]);
    }
    Wire.endTransmission();
  }
  

  void writeKnubPresetID(int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset){

    Wire.beginTransmission(deviceaddress);
    Wire.write((int)eeaddresspage >> 8);
    byte c;
    for ( c = 0; c < IDLength; c++){
      Wire.write(kpreset->ID[c]);
    }
    Wire.endTransmission();
  }

  void writeKnubbieName(int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset, uint8_t knubbieIndx){

    Wire.beginTransmission(deviceaddress);
    Wire.write((int)eeaddresspage >> 8);
    byte c;
    for ( c = 0; c < maxNameLength; c++){
      Wire.write(kpreset->knubbies[knubbieIndx].name[c]);
    }
    Wire.endTransmission();
  }
  
 void writeKnubbieParams(int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset, uint8_t knubbieIndx){

    Wire.beginTransmission(deviceaddress);
    Wire.write((int)eeaddresspage >> 8);
    
    byte c;
    for ( c = 0; c < paramLength; c++){
      Wire.write(kpreset->knubbies[knubbieIndx].params[c]);
    }
    Wire.endTransmission();
  }

void writeKnubbieModSource(int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset, uint8_t knubbieIndx){

    Wire.beginTransmission(deviceaddress);
    Wire.write((int)eeaddresspage >> 8);
    
    byte c;
    for ( c = 0; c < modSourceLength; c++){
      Wire.write(kpreset->knubbies[knubbieIndx].modSource);
      }
      Wire.endTransmission();
  }

void writeKnubbieModState(int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset, uint8_t knubbieIndx){

    Wire.beginTransmission(deviceaddress);
    Wire.write((int)eeaddresspage >> 8);
    
    byte c;
    for ( c = 0; c < stateLength; c++){
      Wire.write(kpreset->knubbies[knubbieIndx].state);
    }
    Wire.endTransmission();
  }

void writeKnubbieNumLoop(int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset, uint8_t knubbieIndx){

    Wire.beginTransmission(deviceaddress);
    Wire.write((int)eeaddresspage >> 8);
    
    byte c;
    for ( c = 0; c < numLoopLength; c++){
      Wire.write(kpreset->knubbies[knubbieIndx].numLoop);
    }
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
    int c = 0;
    for(int c = 0; c<IDLength; c++){
      if (Wire.available()) kpreset->ID[c] = Wire.read();
    }
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
void readKnubbieNumLoop(int deviceaddress, unsigned int eeaddress, aKnubPreset *kpreset, uint8_t knubbieIndx){


  Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    
    Wire.requestFrom(deviceaddress,1);
  
      if (Wire.available()) kpreset->knubbies[knubbieIndx].numLoop = Wire.read();
}



void readKnubFullPreset(int deviceaddress, unsigned int eeaddress, aKnubPreset *kpreset){
  
    ///first we read the preset's name and ID
    
    // so name
    readKnubPresetName(deviceaddress, eeaddress, kpreset); 
    
    //then ID
    readKnubPresetID(deviceaddress, eeaddress + maxNameLength, kpreset);
    
    //so now eeaddress is maxNameLength + IDLength
     uint16_t addrPtr = maxNameLength + IDLength;
   
   
   for(int i = 0; i<numKnubbies; i++){
     
     ///move addrPtr to start of knubbie
     addrPtr = addrPtr*(i+1);
     
     
     readKnubbieName(deviceaddress,addrPtr , kpreset, i);
     
     readKnubbieParams(deviceaddress, addrPtr+maxNameLength, kpreset, i);
       
     readKnubbieModSource(deviceaddress, addrPtr+maxNameLength+paramLength, kpreset, i);
     
     readKnubbieModState(deviceaddress, addrPtr+maxNameLength+paramLength+modSourceLength, kpreset, i);
     
     readKnubbieNumLoop(deviceaddress, addrPtr+maxNameLength+paramLength+modSourceLength+stateLength, kpreset, i);
   }
}






