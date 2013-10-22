#include "Arduino.h"
#include "luts.h"


#define vacMin 1490
#define vacMax 2050

//DAC communication AD5696
byte write_cmds[] = {B01011000, B01011010, B01011100, B01011110}; //for single writes not used here.

byte dacIDZ[] = {B1100000, B1100001, B1100010, B1100011};// 4x4 dacs = 16 dacs so 8 knubs.

int  lowVal, highVal;

//function for multiWrite : 
  
void multiWriteDac(byte addr, byte wrid, byte wrid2, int val, int val2){
  
  Wire.beginTransmission(addr);
  Wire.write(wrid);
  Wire.write(highByte(val));
  Wire.write(lowByte(val));
  Wire.endTransmission();
  
  Wire.beginTransmission(addr); 
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
   
    //lowVal = map(pgm_read_byte(redLUT + knubVal), 0, 255, vacMin, vacMax);
    //highVal = map(pgm_read_byte(redLUT + (255 - knubVal)), 0, 255, vacMin, vacMax);

    lowVal = map(redLUT[knubVal], 0, 255, vacMin, vacMax);
    highVal = map(redLUT[255 - knubVal], 0, 255, vacMin, vacMax);

  switch(knubNum){
  
    case 0:
      multiWriteDac(dacIDZ[0], write_cmds[0], write_cmds[1], lowVal, highVal);
    break;
    case 1:
     multiWriteDac(dacIDZ[0], write_cmds[2], write_cmds[3], lowVal, highVal);
    break;
    case 2:
      multiWriteDac(dacIDZ[1], write_cmds[0], write_cmds[1], lowVal, highVal);
    break;
    case 3:
      multiWriteDac(dacIDZ[1], write_cmds[2], write_cmds[3], lowVal, highVal);
    break;
    
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
  }
   
}; 
 

void updateKnubs(aKnubPreset * kPreset){

    for(int i = 0; i<8; i++){


      turnKnub(i, kPreset->knubbies[i].params[0]);
    }
}



void testDacs(){

  ///FIRST DAC ADDR = 0
  //one side A
  singleWriteDac(dacIDZ[0], write_cmds[0], 4095);
  delay(500);
  singleWriteDac(dacIDZ[0], write_cmds[0], 0);
  delay(500);
  //one side B
  singleWriteDac(dacIDZ[0], write_cmds[1], 4095);
  delay(500);
  singleWriteDac(dacIDZ[0], write_cmds[1], 0);
  delay(500);

  //two side A
  singleWriteDac(dacIDZ[0], write_cmds[2], 4095);
  delay(500);
  singleWriteDac(dacIDZ[0], write_cmds[2], 0);
  delay(500);
  //two side B
  singleWriteDac(dacIDZ[0], write_cmds[3], 4095);
  delay(500);
  singleWriteDac(dacIDZ[0], write_cmds[3], 0);
  delay(500);

/*
///FIRST DAC ADDR = 1
  //three side A
  singleWriteDac(dacIDZ[1], write_cmds[0], 4095);
  delay(500);
  singleWriteDac(dacIDZ[1], write_cmds[0], 0);
  delay(500);
  //three side B
  singleWriteDac(dacIDZ[1], write_cmds[1], 4095);
  delay(500);
  singleWriteDac(dacIDZ[1], write_cmds[1], 0);
  delay(500);

  //four side A
  singleWriteDac(dacIDZ[1], write_cmds[2], 4095);
  delay(500);
  singleWriteDac(dacIDZ[1], write_cmds[2], 0);
  delay(500);
  //four side B
  singleWriteDac(dacIDZ[1], write_cmds[3], 4095);
  delay(500);
  singleWriteDac(dacIDZ[1], write_cmds[3], 0);
  delay(500);
*/
}