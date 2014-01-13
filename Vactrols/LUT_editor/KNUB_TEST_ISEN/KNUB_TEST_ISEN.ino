
#define eepromAddr1 0x50


#include <SPI.h>
#include <Ethernet.h>
#include <Z_OSC.h>
#include <Wire.h>
#include "luts2.h"
//#include "knubFuncs.h"
//#include "memory.h"


int val, analogInVal, minDacVal, maxDacVal;

char knubValues[8];

byte myMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte myIp[]  = { 192, 168, 0, 8 };
int  myPort  = 10000;


byte lin[255];
byte preDist[255];
byte lut[255];

char *subAddress[9]={"/dac/cvA", "/dac/cvB", "/dac/cvC", "/dac/cvD", "/dac/minVal", "/dac/maxVal", "/lut/calc", "/lut/send", "/dac/pre"};

Z_OSCServer server;
Z_OSCMessage *rcvMes;


int vacMin = 0;
int vacMax = 4095;

int vacMin2 = 1200;
int vacMax2 = 2500; 

//DAC communication MCP4728
// might change those to PROGMEM prog_uchar arrays

byte write_cmds[] = {B01011000, B01011010, B01011100, B01011110}; //for single writes not used here.

byte dacIDZ[] = {B1100000, B1100001, B1100010, B1100011};// 4x4 dacs = 16 dacs so 8 knubs.

byte knob1_ch1  = B01000000;
byte knob1_ch2 = B01000010;

byte knob2_ch1 = B01000100;
byte knob2_ch2 = B01000110;

uint16_t  lowVal, highVal, prevExp;

bool prevUp, prevDown;

void setup(){

  Wire.begin();
  Ethernet.begin(myMac, myIp);
  server.sockOpen(myPort);
  Serial.begin(9600);
  

  for(int i = 0; i<255;i++){

    lin[i] = i;

 }


}


void loop(){

if(server.available()){
  
    rcvMes = server.getMessage();
    
    //deals with first knubbie
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[0])){
        
   

        turnKnub(0, val);
        
        analogInVal = analogRead(0);
        analogInVal = map(analogInVal, 0, 1024, 0, 255);
        Serial.write(analogInVal);
        //preDist[val]  = analogInVal;
         
   }
 
   
   if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[4])){
        
       vacMin = rcvMes->getInteger32(0);
   }
   if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[5])){
        
       vacMax = rcvMes->getInteger32(0);
   }
   if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[6])){
        
       calcLut();
   }
   if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[7])){
        
       for(int i = 0; i <255; i++){

          Serial.write(lut[i]);
      }
   }
   if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[8])){
        byte val =  rcvMes->getInteger32(0);   
        
        int lutVal = lut[val];

        turnKnub(1, val);
        
        analogInVal = analogRead(0);
        analogInVal = map(analogInVal, 0, 1024, 0, 255);
        Serial.write(lutVal);
          
         
   }
  }
}

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

      
      if(knubNum == 0){

        lowVal = map(pgm_read_byte(redLUT + knubVal), 0, 255, vacMin, vacMax);
        highVal = map(pgm_read_byte(redLUT + hiRead), 0, 255, vacMin, vacMax);
  
        multiWriteDac(dacIDZ[0], knob1_ch1, knob1_ch2, lowVal, highVal);
      }
      else if(knubNum == 1){

        lowVal = map(knubVal, 0, 255, vacMin2, vacMax2);
        highVal = map(hiRead, 0, 255, vacMin2, vacMax2);

        multiWriteDac(dacIDZ[0], knob2_ch1, knob2_ch2, lowVal, highVal);

      }
    
}



void calcLut(){

  for(int i =0; i<255;i++){

    if(preDist[i] > lin[i]){
      
      lut[i] = lin[i] - (preDist[i] - lin[i]);
    }else if (preDist[i] < lin[i]){
      
      lut[i] = lin[i] + (lin[i] - preDist[i]);
    }
  }
}