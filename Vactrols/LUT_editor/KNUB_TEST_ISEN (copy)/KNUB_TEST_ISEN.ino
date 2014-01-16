
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
byte myIp[]  = { 192, 168, 144, 23 };
int  myPort  = 10000;


byte lin[255];
byte preDist[255];
byte lut[255];

char *subAddress[9]={"/dac/cvA", "/dac/cvB", "/dac/cvC", "/dac/cvD", "/dac/minVal", "/dac/maxVal", "/lut/calc", "/lut/send", "/dac/pre"};

Z_OSCServer server;
Z_OSCMessage *rcvMes;


int vacMin = 0;
int vacMax = 65535;


byte DACIDZ[2] = {B0001100, B0001101};

byte writeCommands[4] = {B00110001, B00110010,B00110100, B00111000};

uint16_t  lowVal, highVal, prevExp;

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
        
        val = rcvMes->getInteger32(0);

        turnKnub(0, val);
        
        analogInVal = analogRead(0);
        analogInVal = map(analogInVal, 0, 1024, 0, 255);
        Serial.println(analogInVal);
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



void writeDac(uint8_t id, uint8_t  wichDac, uint16_t value){

    Wire.beginTransmission(id);
    Wire.write(writeCommands[wichDac]);
    Wire.write(highByte(value));
    Wire.write(lowByte(value));
    Wire.endTransmission();
}




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


void calcLut(){

  for(int i =0; i<255;i++){

    if(preDist[i] > lin[i]){
      
      lut[i] = lin[i] - (preDist[i] - lin[i]);
    }else if (preDist[i] < lin[i]){
      
      lut[i] = lin[i] + (lin[i] - preDist[i]);
    }
  }
}