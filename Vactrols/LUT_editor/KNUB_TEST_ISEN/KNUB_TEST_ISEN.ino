
#include <SPI.h>
#include <Ethernet.h>
#include <Z_OSC.h>
#include <Wire.h>
#include "luts.h"

byte write_cmds[] = {B01011000, B01011010, B01011100, B01011110};

byte dacID = B1100000; 

 byte knob1_ch1  = B01000000;
byte knob1_ch2 = B01000010;

byte knob2_ch1 = B01000100;
byte knob2_ch2 = B01000110;

///function for multiWrite : 

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


int val, analogInVal, minDacVal, maxDacVal;


byte myMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte myIp[]  = { 192, 168, 0, 8 };
int  myPort  = 10000;


char *subAddress[6]={"/knub/k1", "/dac/cvB", "/dac/cvC", "/dac/cvD", "/dac/minVal", "/dac/maxVal"};

Z_OSCServer server;
Z_OSCMessage *rcvMes;

void setup(){

  Wire.begin();
  Ethernet.begin(myMac, myIp);
  server.sockOpen(myPort);
  Serial.begin(9600);
  
  Serial.println(sizeof(redLUT)/sizeof(char));

  minDacVal = 1500;
  maxDacVal = 3000;
  int v1 = map(0, 0, 255, minDacVal, maxDacVal);
  int v2 = map(255, 0, 255, minDacVal, maxDacVal);
  multiWriteDac(dacID, knob1_ch1, knob1_ch2, v1, v2);

}


void loop(){

if(server.available()){
  
    rcvMes = server.getMessage();
    
    //deals with dacA 
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[0])){
        
        val = rcvMes->getInteger32(0);
        
        analogInVal = analogRead(0);
        
        
        int v1 = map(redLUT[val], 0, 255, minDacVal, maxDacVal);
        int v2 = map(redLUT[255-val], 0, 255, minDacVal, maxDacVal);
  
        multiWriteDac(dacID, knob1_ch1, knob1_ch2, v1, v2);
        
        //writeDac(dacID, write_cmds[0], map(val, 0, 255, minDacVal, maxDacVal));
        
        
        
        Serial.write(map(analogInVal, 0, 1024, 0, 255));
     
   }
   
   //deals with dacB 
    /*
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[1])){
        
        val = rcvMes->getInteger32(0);
        analogInVal = analogRead(1);
        writeDac(dacID, write_cmds[1], map(val, 0, 255, minVal, maxVal));
        Serial.write(map(analogInVal, 0, 1024, 0, 255));
     
   }
   */
   if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[4])){
        
       minDacVal = rcvMes->getInteger32(0);
   }
   if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[5])){
        
       maxDacVal = rcvMes->getInteger32(0);
   }
  }
}

void writeDac(byte addr, byte wrid, int val){
  
  Wire.beginTransmission(addr);
  Wire.write(wrid);
  Wire.write(highByte(val));
  Wire.write(lowByte(val)); 
  Wire.endTransmission();
}
