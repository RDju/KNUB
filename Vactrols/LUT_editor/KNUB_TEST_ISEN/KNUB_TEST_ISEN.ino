
#define eepromAddr1 0x50


#include <SPI.h>
#include <Ethernet.h>
#include <Z_OSC.h>
#include <Wire.h>
//#include "luts.h"
#include "knubFuncs.h"
#include "memory.h"


int val, analogInVal, minDacVal, maxDacVal;

char knubValues[8];

byte myMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte myIp[]  = { 192, 168, 0, 8 };
int  myPort  = 10000;

char *subAddress[6]={"/knub/turn", "/knub/save", "/knub/load", "/dac/cvD", "/dac/minVal", "/dac/maxVal"};

Z_OSCServer server;
Z_OSCMessage *rcvMes;

void setup(){

  Wire.begin();
  Ethernet.begin(myMac, myIp);
  server.sockOpen(myPort);
  Serial.begin(9600);
  
}


void loop(){

if(server.available()){
  
    rcvMes = server.getMessage();
    
    //deals with dacA 
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[0])){
        
        
        
        byte wichPot = rcvMes->getInteger32(0);
        byte val =  rcvMes->getInteger32(1);   
        
        knubValues[wichPot - 1] = val;
        /*
        turnKnub(wichPot, 1, knubValues[wichPot - 1]);
        
        analogInVal = analogRead(0);
        Serial.write(map(analogInVal, 0, 1024, 0, 255));
         */
   }

   if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[4])){
        
       minDacVal = rcvMes->getInteger32(0);
   }
   if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[5])){
        
       maxDacVal = rcvMes->getInteger32(0);
   }
    
   
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[1])){
        
         writeSingleKnub(eepromAddr1, 0, knubValues[0]);
         Serial.write(knubValues[0]);
   }
     if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[2])){
        
         knubValues[0] = readSingleKnub(eepromAddr1, 0);
         Serial.write(knubValues[0]);
         
   }

  }
}

