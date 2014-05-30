#include "Arduino.h"
#define saveTime 10

#define maxNameLength 8
#define IDLength 6
#define paramLength 6
#define modOnLength 1
#define stateLength 1
#define numLoopLength 1
#define numKnubbies 8
#define LCDlength 15

#define presetSize 126
#define presetsNumber 60
/*#define modIndx maxNameLength+paramLength
#define stateIndx maxNameLength+paramLength+modOnLength
#define loopIndx maxNameLength+paramLength+modOnLength+stateLength*/

#define eepromAddr1 0x50

char nameTemp[maxNameLength];


  
typedef struct aKnub{
  char name[maxNameLength];
  byte params[paramLength];
  /*byte modOn;
  byte state;
  byte numLoop;*/
  
  boolean isNameEdited;
  boolean isParamsEdited[paramLength];
  /*boolean isModOnEdited;
  boolean isStateEdited;
  boolean isNumLoopEdited;*/
}aKnub;


typedef struct aKnubPreset{

  char name[maxNameLength];
  byte ID[IDLength];

  aKnub knubbies[numKnubbies];
}aKnubPreset;

void writeByte( int deviceaddress, unsigned int eeaddress, byte data ) {
    
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(data);
    
    Wire.endTransmission();
    delay(5);
  }

  
byte readByte( int deviceaddress, unsigned int eeaddress ) {
   
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();

    Wire.requestFrom(deviceaddress,1);
    if (Wire.available())
     return Wire.read();
    delay(5);
  
  }


//TODO: check if begin/end Transmission are always required --> faire une seule fonction
void writeKnubPresetName( int deviceaddress, int eeaddresspage, aKnubPreset *kpreset ) {

  for(uint8_t i = 0; i<maxNameLength; i++)
      writeByte(deviceaddress, eeaddresspage+i, kpreset->name[i]);
}


void writeKnubPresetID(int deviceaddress, int eeaddresspage, aKnubPreset *kpreset){

  for ( uint8_t i = 0; i < IDLength; i++){
    writeByte(deviceaddress, eeaddresspage+i,kpreset->ID[i]);
  }
}

void writeKnubbieName(int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset, uint8_t knubbieIndx){

  if (kpreset->knubbies[knubbieIndx].isNameEdited){
    for(uint8_t i =0 ; i<maxNameLength;i++){
        writeByte(deviceaddress, eeaddresspage+i, kpreset->knubbies[knubbieIndx].name[i]);
    }
  }
        
  kpreset->knubbies[knubbieIndx].isNameEdited = false;
}

void writeKnubbieParams(int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset, uint8_t knubbieIndx){
  
  for (uint8_t i = 0; i < paramLength; i++){
    if (kpreset->knubbies[knubbieIndx].isParamsEdited[i]){
      writeByte(deviceaddress, eeaddresspage+i, kpreset->knubbies[knubbieIndx].params[i]);
      Serial.print("knubbie : ");
      Serial.println(knubbieIndx);
      Serial.print("param : ");
      Serial.println(i);
      kpreset->knubbies[knubbieIndx].isParamsEdited[i] = false;
    }
  }
}


void readKnubPresetName( int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset) {
    
   for(uint8_t i = 0; i<maxNameLength; i++)
      kpreset->name[i] = readByte(deviceaddress, eeaddresspage+i);
}

void readKnubPresetNameTemp( int deviceaddress, unsigned int eeaddresspage) {
    
   for(uint8_t i = 0; i<maxNameLength; i++)
      nameTemp[i] = readByte(deviceaddress, eeaddresspage+i);
}

void readKnubPresetID( int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset) {
    
   for(uint8_t i = 0; i<IDLength;i++)
      kpreset->ID[i] = readByte(deviceaddress, eeaddresspage+i);
}

void readKnubbieName( int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset, uint8_t knubbieIndx) {
    
  for(uint8_t i =0 ; i<maxNameLength;i++){
      kpreset->knubbies[knubbieIndx].name[i] = readByte(deviceaddress, eeaddresspage+i);
   }
   kpreset->knubbies[knubbieIndx].isNameEdited=false;
   

}

void readKnubbieParams(int deviceaddress, unsigned int eeaddresspage, aKnubPreset *kpreset, uint8_t knubbieIndx){

    for(uint8_t i = 0; i<paramLength;i++){
        kpreset->knubbies[knubbieIndx].params[i] = readByte(deviceaddress, eeaddresspage+i);
        kpreset->knubbies[knubbieIndx].isParamsEdited[i]=false;
    }

}

//Fill kpreset with all the informations of the preset located in the eeadress given
void readKnubPreset(int deviceaddress, unsigned int eeaddress, aKnubPreset *kpreset){
  
   //first we read the preset's name and ID
   readKnubPresetName(deviceaddress, eeaddress, kpreset); 
   readKnubPresetID(deviceaddress, eeaddress + maxNameLength, kpreset);
     
   for(int i = 0; i<numKnubbies; i++){
     readKnubbieName(deviceaddress, eeaddress+(14*i), kpreset, i);
     readKnubbieParams(deviceaddress, (eeaddress+(14*i)) + maxNameLength, kpreset, i); 
   }
}

void writeKnubPreset(int deviceaddress,  int eeaddress, aKnubPreset *kpreset){
  
        //name
        //writeKnubPresetName(deviceaddress, eeaddress, kpreset);

        //ID
       // writeKnubPresetID(deviceaddress, eeaddress + maxNameLength, kpreset);
       
        for(int i = 0 ; i<numKnubbies ; i++){
          writeKnubbieName(deviceaddress, eeaddress+(14*i), kpreset, i);
          writeKnubbieParams(deviceaddress, (eeaddress+(14*i)) + maxNameLength, kpreset, i);  
        }
}


inline boolean isPresetEdited(aKnubPreset *kpreset){
  for (int i = 0; i < 8; i++){
    if (kpreset->knubbies[i].isNameEdited || kpreset->knubbies[i].isParamsEdited[0] || kpreset->knubbies[i].isParamsEdited[1] || kpreset->knubbies[i].isParamsEdited[2] || kpreset->knubbies[i].isParamsEdited[3] || kpreset->knubbies[i].isParamsEdited[4] || kpreset->knubbies[i].isParamsEdited[5]){
      Serial.println(i);
      return true;
    }
  }
  return false;
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





