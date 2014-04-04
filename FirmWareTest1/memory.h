#include "Arduino.h"
#define saveTime 10

#define maxNameLength 8
#define IDLength 6
#define paramLength 3
#define modOnLength 1
#define stateLength 1
#define numLoopLength 1
#define numKnubbies 8
#define LCDlength 15

#define presetSize 126
#define modIndx maxNameLength+paramLength
#define stateIndx maxNameLength+paramLength+modOnLength
#define loopIndx maxNameLength+paramLength+modOnLength+stateLength

#define eepromAddr1 0x50

typedef struct aKnub{
  char name[maxNameLength];
  byte params[3];
  byte modOn;
  byte state;
  byte numLoop;
}aKnub;


typedef struct aKnubPreset{

  char name[maxNameLength];
  byte ID[6];

  aKnub knubbies[numKnubbies];
}aKnubPreset;


//TODO: check if begin/end Transmission are always required --> faire une seule fonction
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
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB

  byte c;
  for ( c = 0; c < IDLength; c++){
    Wire.write(kpreset->ID[c]);
  }

  Wire.endTransmission();
}

void writeKnubbieName(int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset, uint8_t knubbieIndx){

  Wire.beginTransmission(deviceaddress);
  Wire.write((int)eeaddresspage >> 8);
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB

  for ( byte c = 0; c < maxNameLength; c++){
    Wire.write(kpreset->knubbies[knubbieIndx].name[c]);
  }
  Wire.endTransmission();
}

void writeKnubbieParams(int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset, uint8_t knubbieIndx){

  Wire.beginTransmission(deviceaddress);
  Wire.write((int)eeaddresspage >> 8);
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB

  byte c;
  for ( c = 0; c < paramLength; c++){
    Wire.write(kpreset->knubbies[knubbieIndx].params[c]);
  }
  Wire.endTransmission();
}

void writeKnubbiemodOn(int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset, uint8_t knubbieIndx){

  Wire.beginTransmission(deviceaddress);
  Wire.write((int)eeaddresspage >> 8);
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB

  byte c;
  for ( c = 0; c < modOnLength; c++){
    Wire.write(kpreset->knubbies[knubbieIndx].modOn);
  }
  Wire.endTransmission();
}

void writeKnubbieModState(int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset, uint8_t knubbieIndx){

  Wire.beginTransmission(deviceaddress);
  Wire.write((int)eeaddresspage >> 8);
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB

  byte c;
  for ( c = 0; c < stateLength; c++){
    Wire.write(kpreset->knubbies[knubbieIndx].state);
  }
  Wire.endTransmission();
}

void writeKnubbieNumLoop(int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset, uint8_t knubbieIndx){

  Wire.beginTransmission(deviceaddress);
  Wire.write((int)eeaddresspage >> 8);
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB

  byte c;
  for ( c = 0; c < numLoopLength; c++){
    Wire.write(kpreset->knubbies[knubbieIndx].numLoop);
  }
  Wire.endTransmission();
}

void readKnubPreset(int deviceaddress, unsigned int eeaddress, aKnubPreset *kpreset){
  
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  
  Wire.requestFrom(deviceaddress,maxNameLength);
  for (int c = 0; c < maxNameLength; c++ )
      if (Wire.available()) kpreset->name[c] = Wire.read();
  
  Wire.requestFrom(deviceaddress,IDLength);
  for(int c = 0; c<IDLength; c++)
      if (Wire.available()) kpreset->ID[c] = Wire.read();
      
  

  ///first we read the preset's name and ID
  /*readKnubPresetName(deviceaddress, eeaddress, kpreset); 
  readKnubPresetID(deviceaddress, eeaddress + maxNameLength, kpreset);*/

  //so now eeaddress is maxNameLength + IDLength
  unsigned int addrPtr = eeaddress + maxNameLength + IDLength;

  for(int i = 0; i<numKnubbies; i++){

    ///move addrPtr to start of knubbie
    addrPtr = addrPtr*(i+1);
    
    //read knubbie name
    Wire.requestFrom(deviceaddress,maxNameLength);  
    for ( byte c = 0; c < maxNameLength; c++ )
      if (Wire.available()) kpreset->knubbies[i].name[c] = Wire.read();
    
    //read knubbie param
    Wire.requestFrom(deviceaddress,paramLength);
    for (int  c = 0; c < paramLength; c++ )
      if (Wire.available()) kpreset->knubbies[i].params[c] = Wire.read();
    
    //read knubbie modOn
    Wire.requestFrom(deviceaddress, 1);
    if (Wire.available()) kpreset->knubbies[i].modOn = Wire.read();
    
    //read knubbie modState
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) kpreset->knubbies[i].state = Wire.read();
      
    //read knubbie num loop
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) kpreset->knubbies[i].numLoop = Wire.read();
    /*
    readKnubbieName(deviceaddress,addrPtr , kpreset, i);
    readKnubbieParams(deviceaddress, addrPtr+maxNameLength, kpreset, i);
    readKnubbiemodOn(deviceaddress, addrPtr+modIndx, kpreset, i);
    readKnubbieModState(deviceaddress, addrPtr+stateIndx, kpreset, i);
    readKnubbieNumLoop(deviceaddress, addrPtr+loopIndx, kpreset, i); */
  }
}

void writeKnubPreset(int deviceaddress, unsigned int eeaddress, aKnubPreset *kpreset){

  writeKnubPresetName(deviceaddress, eeaddress, kpreset); 
  writeKnubPresetID(deviceaddress, eeaddress + maxNameLength, kpreset);
  //so now eeaddress is maxNameLength + IDLength
  unsigned int addrPtr = eeaddress + maxNameLength + IDLength;

  for(int i = 0; i<numKnubbies; i++){

    ///move addrPtr to start of knubbie
    addrPtr = addrPtr*(i+1);

    writeKnubbieName(deviceaddress,addrPtr , kpreset, i);
    writeKnubbieParams(deviceaddress, addrPtr+maxNameLength, kpreset, i);
    writeKnubbiemodOn(deviceaddress, addrPtr+maxNameLength+paramLength, kpreset, i);
    writeKnubbieModState(deviceaddress, addrPtr+maxNameLength+paramLength+modOnLength, kpreset, i);
    writeKnubbieNumLoop(deviceaddress, addrPtr+maxNameLength+paramLength+modOnLength+stateLength, kpreset, i);
  }
}

/***********************************************************************
 * internal EEPROM
 ***************************************************************************/
/*boolean isEepromInitialised(){
  int isInit = EEPROM.read(0);
  if (isInit == 1)
    return true;
  else return false;
}

//TODO: cut the size of strings if too long for the LCD
void initProductPage(){
  char* prodPageString[] = {
    "    LE KNUB    ", " by Combosquare "  };
  for (int j = 0; j < 2; j++)
    for (int i = 0; i < LCDlength; i++)
      EEPROM.write(i + 1 + j*LCDlength, prodPageString[j][i]);
}

void initSoftwareVersion(){
  char initSoftString[] = " Firmware: v0.1a";
  for (int i = 0; i < LCDlength; i++)
    EEPROM.write(i+1+2*LCDlength, initSoftString[i]);
}

void initEEPROM(){
  if (!isEepromInitialised()){
    EEPROM.write(0, 1);
    initProductPage();
    initSoftwareVersion();
  }
}

char ** getProductPageString(){
  char *prodPageString[] = {(char *)malloc(LCDlength*sizeof (char)), (char *)malloc(LCDlength*sizeof (char))  };
  for (int j = 0; j < 2; j++)
    for (int i = 0; i < LCDlength; i++)
      prodPageString[j][i] = EEPROM.read(i+1+j*LCDlength);
  return prodPageString;
}

char* getSoftwareVersionString(){
  char *initSoftString = (char *)malloc(LCDlength*sizeof (char));
  for (int i = 0; i < LCDlength; i++)
    initSoftString[i] = EEPROM.read(i+1+2*LCDlength);
  return initSoftString;
}*/





