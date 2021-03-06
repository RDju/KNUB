#include "Arduino.h"
#include "luts.h"


int vacMin = 0;
int vacMax = 65535;


byte DACIDZ[2] = {B0001100, B0001101}

byte writeCommands[4] = {B00110001, B00110010,B00110100, B00111000};

uint16_t  lowVal, highVal, prevExp;

bool prevUp, prevDown;

void writeDac(uint16_t id, uint16_t int wichDac, uint16_t value){

    Wire.beginTransmission(id);
    Wire.write(writeCommands[wichDac]);
    Wire.write(highByte(value));
    Wire.write(lowByte(value));
    Wire.endTransmission();
}



//actual turn knub func
/
void turnKnub(byte knubNum,byte knubVal){
    
    byte hiRead = 255 - knubVal;
    
    lowVal = map(knubVal, 0, 255, vacMin, vacMax);
    highVal = map(hiRead, 0, 255, vacMin, vacMax);

  switch(knubNum){
  
    case 0:
      
      writeDac(DACIDZ[0], writeCommands[0], lowVal);
      writeDac(DACIDZ[0], writeCommands[1], highVal);
    break;
    

    case 1:
     writeDac(DACIDZ[0], writeCommands[2], lowVal);
     writeDac(DACIDZ[0], writeCommands[3], highVal);
    break;
    
    case 2:
      writeDac(DACIDZ[1], writeCommands[0], lowVal);
     writeDac(DACIDZ[1], writeCommands[1], highVal);
    break;
    
    case 3:
      writeDac(DACIDZ[1], writeCommands[0], lowVal);
     writeDac(DACIDZ[1], writeCommands[1], highVal);
    break;
    }
}
 
/*
void updateKnubs(aKnubPreset * kPreset){
 
      for(uint8_t i = 0; i<3; i++){
        if(kPreset->knubbies[i].state == 1){
          turnKnub(i, kPreset->knubbies[i].params[0]);
        }
      }
}




void printPresetName(aKnubPreset *kPreset){

    Serial.println(kPreset->name);
}


void doExpressionPedal(unsigned int expVal){

  expVal = expVal >> 2;

    if(abs(expVal - prevExp) > 2){

      for(uint8_t i =0; i<3; i++){
        if(currentPreset.knubbies[i].modOn == 1){
          turnKnub(i, map(expVal, 0, 255, currentPreset.knubbies[i].params[0], currentPreset.knubbies[i].params[1]));
          //Serial.println(expVal);
        }
      }

    prevExp = expVal;
  }
}






void debugKnubPreset(aKnubPreset *kPreset){

  Serial.print("PRESET NAME: ");
  Serial.println(kPreset->name);

  for(uint8_t  i = 0; i<numKnubbies;i++){

    Serial.print("KNUBBIE: ");
    Serial.println(kPreset->knubbies[i].name);
    Serial.print("VAL1: ");
    Serial.println(kPreset->knubbies[i].params[0]);
  }


*/