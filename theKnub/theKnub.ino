#include <SPI.h>
#include <Z_OSC.h>
#include <MIDI.h>
#include <Ethernet.h>
#include <Wire.h>
#include "knubFuncs.h"

#define ledPin 4

byte myMac[] = { 11, 22, 33, 44, 55};
byte myIp[]  = { 192, 168, 0, 8 };
uint16_t  myPort  = 10000;

Z_OSCServer server;
Z_OSCMessage *rcvMes;

char *subAddress[3]={"/knub/turn", "/knub/loadBank/", "/knub/saveBank"};

void setup(){

  Wire.begin();
  Ethernet.begin(myMac, myIp);
  server.sockOpen(myPort);

  ///here recall dacs last settings as to avoid a short circuit
}


void loop(){
 
  if(server.available()){
  
    rcvMes = server.getMessage();
   
   //deals with pots messages
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[0])){
        
      byte wichPot = rcvMes->getInteger32(0);
      byte potVal =  rcvMes->getInteger32(1);   
        
        turnKnub(wichPot, 1, potVal);     
    }
  }
}



