#include "Arduino.h"
#include "luts.h"


//#define vacMin 1490
//#define vacMax 2050


#define vacMin  0
#define vacMax 4095

//DAC communication MCP4728
// might change those to PROGMEM prog_uchar arrays

byte write_cmds[] = {B01011000, B01011010, B01011100, B01011110}; //for single writes not used here.

byte dacIDZ[] = {B1100000, B1100001, B1100010, B1100011};// 4x4 dacs = 16 dacs so 8 knubs.

byte knob1_ch1  = B01000000;
byte knob1_ch2 = B01000010;

byte knob2_ch1 = B01000100;
byte knob2_ch2 = B01000110;

uint16_t  lowVal, highVal;



/*
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
*/

/*
///custom mapping function
unsigned int knubMapFromLut(unsigned int in,  unsigned int in_min, unsigned int in_max, unsigned int out_min, unsigned int out_max){


  return (in - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

}
*/
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

    lowVal = map(pgm_read_byte(redLUT + knubVal), 0, 255, vacMin, vacMax);
    highVal = map(pgm_read_byte(redLUT + hiRead), 0, 255, vacMin, vacMax);
    
    //lowVal = map(redLUT[knubVal], 0, 255, vacMin, vacMax);
    //highVal = map(redLUT[255 - knubVal], 0, 255, vacMin, vacMax);
    
    //lowVal = knubMapFromLut(pgm_read_byte(redLUT + knubVal), 0, 255, vacMin, vacMax);
    //highVal = knubMapFromLut(pgm_read_byte(redLUT + (255- knubVal)), 0, 255,  vacMin, vacMax);
    
    //lowVal = pgm_read_byte(redLUT+knubVal);
    //highVal = pgm_read_byte(redLUT + (255 - knubVal));

    Serial.print("KNUBVAL: ");
    Serial.println(knubVal);
    Serial.print("low: ");
    Serial.print()

    Serial.print(lowVal);
    Serial.print(",  hi: ");
    Serial.println(highVal);
    
  /*
  switch(knubNum){
  
    case 0:
      multiWriteDac(dacIDZ[0], knob1_ch1, knob1_ch2, lowVal, highVal);
    break;
    case 1:
     multiWriteDac(dacIDZ[0], knob2_ch1, knob2_ch2, lowVal, highVal);
    break;
    /*
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
    
    }
    */
}
 

void updateKnubs(aKnubPreset * kPreset){
 
    Serial.println("KNUB UPDATE CALLED");
    
    for(uint8_t i = 0; i<1; i++){

        turnKnub(i, kPreset->knubbies[i].params[0]);


    }
  
    
}


/*
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

}
*/
