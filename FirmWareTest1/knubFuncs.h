#define preLUT {0 ,  4 ,  9 ,  11 ,  13 ,  15 ,  16, 17 ,  19 ,  20,  21 ,  22 ,  24 ,  25 ,  26 ,  27 ,  27 ,  28 ,  29 ,  29 ,  30,  31 ,  31 ,  32 ,  33 ,  33 ,  34 ,  34 ,  35 ,  35 ,  36 ,  36 , 37 ,  37 ,  37 ,  38 ,  38 ,  39 ,  39 ,  39 ,  39 ,  40 ,  40 ,  41 ,  41 ,42 ,  42 ,  42 ,  43 ,  43 ,  43 ,  43 ,  43 ,  44 ,  44 ,  44 ,  44 ,  44 ,  45 ,  45 ,  46 ,  46 ,  46 ,  46 ,  46 ,  46 ,  46 ,  47 ,  47 ,  47 ,  47 ,  47 ,  47 ,  48 ,  48 ,  48 ,  48 ,  49 ,  49 ,  49 ,  49 ,  49 ,  49 ,  49 ,  50 ,  50 ,  50,  50 ,  50 ,  50 ,  50 ,  50 ,  50 ,  51 ,  51 ,  51 ,  51 ,  51,  51 ,  52 ,  52 ,  52 ,  52 ,  52 ,  52 ,  53 ,  53 ,  53 ,  53,  53 ,  54 ,  54 ,  54 ,  54 ,  54 ,  55 ,  55 ,  55 ,  55 ,  55 ,  56 ,  56 ,  56 ,  56 ,  57 ,  57 ,  57 ,  58 ,  58 ,  58 ,  58 ,  59 ,  59 ,  59 ,  60 ,  60 ,  60 ,  61 ,  61 ,  61 ,  61}
//62,62 ,  62 ,  63 ,  63 ,  63 ,  64 ,  64 ,  64 ,  64 ,  65 ,  65 , 65 ,  66 ,  66 ,  66 ,  66 ,  67 ,  67 ,  68 ,  68 ,  68 ,  69 ,  69 ,  70 ,  70 ,  71 ,  72 ,  72 ,  73 ,  74 ,  74 ,  75 ,  76 ,  76 ,  77 ,  78 ,  78 ,  79 ,  80 ,  81 ,  82 ,  83 ,  84 ,  85 ,  86 ,  87 ,  88 ,  89 ,  90 ,  90 ,  91 ,  92 ,  93 ,  94 ,  96 ,  96 ,  97 ,  99 ,  100 ,  102 ,  103 ,  105 ,  107 ,  109 ,  110,  112 ,  115 ,  117 ,  120 ,  122 ,  125 ,  127 ,  131 ,  134 ,  138 ,  141 ,  143 ,  146 ,  149 ,  152 ,  155 ,  161 ,  164 ,  167 ,  172 ,  176 ,  188 ,  203 ,  215 ,  224 ,  233 ,  238 ,  241 ,  244 ,  247 ,  249 ,  251 ,  252 ,  254 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 }

//DAC communication MCP4728
byte write_cmds[] = {B01011000, B01011010, B01011100, B01011110}; //for single writes not used here.

byte dacIDZ[] = {B1100000, B1100001, B1100010, B1100011};// 4x4 dacs = 16 dacs so 8 knubs.

byte knob1_ch1  = B01000000;//chanA
byte knob1_ch2 =  B01000010;//chanB

byte knob2_ch1 = B01000100;//chanC
byte knob2_ch2 = B01000110;//chanD

byte redLUT[] = preLUT;


byte getValueFromLUT(byte indx){


  return redLUT[indx];


}


int  lowVal, highVal;

//function for multiWrite : 

void multiWriteDac(byte addr, byte wrid, byte wrid2, int val, int val2){
  
  Wire.beginTransmission(addr);
  Wire.write(wrid);
  Wire.write(highByte(val));
  Wire.write(lowByte(val)); 
  Wire.write(wrid2);
  Wire.write(highByte(val2));
  Wire.write(lowByte(val2)); 
  Wire.endTransmission();
};

//actual turn knub func

void turnKnub(byte knubNum, byte knubType, byte * knubVal){
  
  
  //knubVal = (int)knubVal;
  /*
  //knubVal = map(knubVal, 0, 100, 0, 255);
  // depending on the type of knob we want to replace : linear or log , change the LUT
  */
  //if(knubType == 1){
    //
    //highVal = map(redLUT[255 - knubVal], 0, 255, vacMin, vacMax);
  //}
  
  
  ///int mapped = redLUT[knubVal];
  
   //byte mapped = redLUT[knubVal];
   
   
   


   //lowVal  = map(mapped, 0, 255, 0 , 255);
   //highVal = map(knubVal, 0, 255, vacMin , vacMax);
  /*
  switch(knubNum){
  
    case 1:
      multiWriteDac(dacIDZ[0], knob1_ch1, knob1_ch2, lowVal, highVal);
    break;
    case 2:
     multiWriteDac(dacIDZ[0], knob2_ch1, knob2_ch2, lowVal, highVal);
    break;
    case 3:
      multiWriteDac(dacIDZ[1], knob1_ch1, knob1_ch2, lowVal, highVal);
    break;
    case 4:
      multiWriteDac(dacIDZ[1], knob2_ch1, knob2_ch2, lowVal, highVal);
    break;
    
    case 5:
      multiWriteDac(dacIDZ[2], knob1_ch1, knob1_ch2, lowVal, highVal);
    break;
    case 6:
      multiWriteDac(dacIDZ[2], knob2_ch1, knob2_ch2, lowVal, highVal);
    break;
    case 7:
      multiWriteDac(dacIDZ[3], knob1_ch1, knob1_ch2, lowVal, highVal);
    break;
    case 8:
      multiWriteDac(dacIDZ[3], knob2_ch1, knob2_ch2, lowVal, highVal);
    break;
  }
   */
}; 
  


