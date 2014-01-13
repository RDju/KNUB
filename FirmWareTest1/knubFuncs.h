#include "Arduino.h"
#include "luts.h"



#define vacMin 1200
#define vacMax 2500



//#define vacMin  0
//#define vacMax 4095

//DAC communication MCP4728
// might change those to PROGMEM prog_uchar arrays

byte write_cmds[] = {B01011000, B01011010, B01011100, B01011110}; //for single writes not used here.

byte dacIDZ[] = {B1100000, B1100001, B1100010, B1100011};// 4x4 dacs = 16 dacs so 8 knubs.

byte knob1_ch1  = B01000000;
byte knob1_ch2 = B01000010;

byte knob2_ch1 = B01000100;
byte knob2_ch2 = B01000110;



uint16_t  lowVal, highVal, prevExp;



//function for multiWrite : 
  
void multiWriteDac(byte addr, byte wrid, byte wrid2, uint16_t val, uint16_t val2){
  
  Wire.beginTransmission(addr);
  Wire.write(wrid);
  Wire.write(highByte(val));
  Wire.write(lowByte(val)); 
  Wire.write(wrid2);
  Wire.write(highByte(val2));
  Wire.write(lowByte(val2)); 
  Wire.endTransmission();
};



void singleWriteDac(byte addr, byte wrcmd, uint16_t val){

  Wire.beginTransmission(addr);
  Wire.write(wrcmd);
  Wire.write(highByte(val));
  Wire.write(lowByte(val));
  Wire.endTransmission();
}

//actual turn knub func

void turnKnub(byte knubNum,byte knubVal){
    
    byte hiRead = 255 - knubVal;


    if(knubNum != 0){
      lowVal = map(pgm_read_byte(redLUT + knubVal), 0, 255, vacMin, vacMax);
      highVal = map(pgm_read_byte(redLUT + hiRead), 0, 255, vacMin, vacMax);
    }else{
      lowVal = map(pgm_read_byte(redLUT + knubVal), 0, 255, 0, 4095);
      highVal = map(pgm_read_byte(redLUT + hiRead), 0, 255, 0, 4095);

    }

  switch(knubNum){
  
    case 0:
      multiWriteDac(dacIDZ[0], knob1_ch1, knob1_ch2, lowVal, highVal);
    break;
    case 1:
     multiWriteDac(dacIDZ[0], knob2_ch1, knob2_ch2, lowVal, highVal);
    break;
    
    case 2:
      multiWriteDac(dacIDZ[1], knob1_ch1, knob1_ch2, lowVal, highVal);
    break;
    
    case 3:
      multiWriteDac(dacIDZ[1], knob2_ch1, knob2_ch2, lowVal, highVal);
    break;
    /*
    case 4:
     multiWriteDac(dacIDZ[2], write_cmds[0], write_cmds[1], lowVal, highVal);
    break;
    case 5:
      multiWriteDac(dacIDZ[2], write_cmds[2], write_cmds[3], lowVal, highVal);
    break;
    case 6:
      multiWriteDac(dacIDZ[3], write_cmds[0], write_cmds[1], lowVal, highVal);
    break;
    case 7:
      multiWriteDac(dacIDZ[3], write_cmds[2], write_cmds[3], lowVal, highVal);
    break;
    */
    }
    
}
 

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
}