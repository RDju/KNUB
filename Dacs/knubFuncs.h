#include "Arduino.h"
#include "luts.h"


#define vacMin 1490
#define vacMax 2050

//DAC communication AD5696
byte dacIDZ[] = {B0001100, B0001101, B0001110, B0001111};// 4x4 dacs = 16 dacs so 8 knubs.
byte write_cmds[] = {B00110001, B00110010, B00110100, B00111000}; 

int  lowVal, highVal;

//function for multiWrite : 

void multiWriteDac(byte addr, byte wrid, byte wrid2, int val, int val2){
  
  Wire.beginTransmission(addr);
  Wire.write(wrid);
  Wire.write(highByte(val));
  Wire.write(lowByte(val));
  wire.endTransmission();
  
  Wire.begingTransmission(addr) 
  Wire.write(wrid2);
  Wire.write(highByte(val2));
  Wire.write(lowByte(val2)); 
  Wire.endTransmission();
};

//actual turn knub func

void turnKnub(byte knubNum,byte knubVal){
   
    lowVal = map(pgm_read_byte(greenLUT + knubVal), 0, 255, vacMin, vacMax);
    highVal = map(pgm_read_byte(greenLUT + (255 - knubVal)), 0, 255, vacMin, vacMax);

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
    
    case 4:
      multiWriteDac(dacIDZ[2], knob1_ch1, knob1_ch2, lowVal, highVal);
    break;
    case 5:
      multiWriteDac(dacIDZ[2], knob2_ch1, knob2_ch2, lowVal, highVal);
    break;
    case 6:
      multiWriteDac(dacIDZ[3], knob1_ch1, knob1_ch2, lowVal, highVal);
    break;
    case 7:
      multiWriteDac(dacIDZ[3], knob2_ch1, knob2_ch2, lowVal, highVal);
    break;
  }
   
}; 
  


