/*
This version works for editing the response of 1 vactrol

This version has been updated from a previous one that was dealing with a digital potentiometer wired as a DAC (rDAC ?)
this works with the quad DAC mcp4728 found on the evaluation board.


TODO:

check the response of 2 vactrols AT THE SAME TIME
so they can be matched

*/
//#include <Client.h>
#include <SPI.h>
#include <Ethernet.h>
//#include <Server.h>

#include <Z_OSC.h>
#include <Wire.h>
#include "knubFuncs.h"


//was 1959
#define maxSend 4095
//was 1172
#define minSend 0



Z_OSCClient client;
int optoId;
int val;
int an1, an2;
int lg, mult;
boolean goLinear;

int vacResp[255];

int lookup[255] = {3 ,  6 ,  8 ,  10 ,  11 ,  12 ,  14 ,  15 ,  16 ,  16 ,  17 ,  
  18 ,  19 ,  19 ,  20 ,  20 ,  21 ,  21 ,  22 ,  22 ,  23 ,  23 ,  24 ,  24 ,  24 ,  
  25 ,  25 ,  25 ,  26 ,  26 ,  26 ,  27 ,  27 ,  27 ,  28 ,  28 ,  28 ,  29 ,  29 ,  
  29 ,  29 ,  30 ,  30 ,  30 ,  30 ,  30 ,  31 ,  31 ,  31 ,  31 ,  32 ,  32 ,  32 ,  
  33 ,  33 ,  33 ,  33 ,  33 ,  34 ,  34 ,  34 ,  35 ,  35 ,  37 ,  37 ,  38 ,  38 ,  
  39 ,  39 ,  39 ,  39 ,  39 ,  40 ,  40 ,  40 ,  41 ,  41 ,  41 ,  41 ,  41 ,  42 ,  
  42 ,  42 ,  42 ,  43 ,  43 ,  43 ,  43 ,  44 ,  44 ,  44 ,  45 ,  45 ,  45 ,  46 ,  
  46 ,  46 ,  47 ,  47 ,  48 ,  48 ,  48 ,  49 ,  49 ,  49 ,  50 ,  50 ,  51 ,  51 ,  
  52 ,  52 ,  52 ,  53 ,  53 ,  53 ,  54 ,  54 ,  54 ,  55 ,  55 ,  56 ,  56 ,  56 ,  
  57 ,  58 ,  58 ,  58 ,  59 ,  59 ,  59 ,  60 ,  60 ,  61 ,  61 ,  62 ,  62 ,  62 ,  
  63 ,  63 ,  64 ,  64 ,  65 ,  65 ,  65 ,  66 ,  66 ,  66 ,  67 ,  67 ,  68 ,  68 ,  
  69 ,  69 ,  70 ,  71 ,  71 ,  71 ,  72 ,  73 ,  73 ,  73 ,  74 ,  75 ,  75 ,  75 ,  
  76 ,  76 ,  77 ,  77 ,  78 ,  79 ,  79 ,  80 ,  80 ,  81 ,  82 ,  83 ,  83 ,  84 ,  
  84 ,  85 ,  86 ,  86 ,  87 ,  88 ,  89 ,  89 ,  90 ,  90 ,  91 ,  92 ,  93 ,  94 ,  
  94 ,  95 ,  96 ,  97 ,  98 ,  98 ,  99 ,  100 ,  101 ,  102 ,  103 ,  104 ,  106 ,  
  107 ,  108 ,  109 ,  111 ,  112 ,  113 ,  115 ,  116 ,  118 ,  119 ,  121 ,  122 ,  
  124 ,  127 ,  129 ,  130 ,  132 ,  134 ,  136 ,  138 ,  140 ,  142 ,  145 ,  148 ,  
  151 ,  158 ,  165 ,  173 ,  180 ,  185 ,  190 ,  199 ,  208 ,  212 ,  215 ,  219 ,  
  225 ,  230 ,  235 ,  240 ,  244 ,  250 ,  252 ,  253 ,  254 ,  255 ,  255 ,  255 ,  255 };


byte myMac[] = { 11, 22, 33, 44, 55};
byte myIp[]  = { 192, 168, 144, 23 };
int  myPort  = 10000;

byte computer[] = {192, 168, 144, 21};
int scPort = 57120;

char *subAddress[4]={"/O2A/switch","/O2A/doPot", "/O2A/lookupVal", "/O2A/tellMe"};

char oscAdr1[] = "/O2A/vac1";
char oscAdr2[] = "/O2A/done";
char oscAdr3[] = "/O2A/doneVac";
char oscAdr4[] = "/O2A/doneVac2";

Z_OSCServer server;
Z_OSCMessage *rcvMes;
Z_OSCMessage sendMes;

void setup(){

  Wire.begin();
  Ethernet.begin(myMac, myIp);
  server.sockOpen(myPort);

  optoId = 0;
  pinMode(2, OUTPUT);

}


void loop(){

  //Serial.println(optoId);
if(server.available()){
  
    rcvMes = server.getMessage();
    
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[0])){
    
        optoId = rcvMes->getInteger32(0); 
    }
    //deals with pot1
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[1])){
      
          for(int i = 0; i <255; i++){
            
            //int v1 = map(lookup[i], 0, 255, minSend, maxSend);
            //int v2 = map(lookup[255 - i], 0, 255, minSend, maxSend);
            //int v2 = 0;
            //multiWriteDac(dacID, knob1_ch1, knob1_ch2, v1, v2);
            //singleWriteDac(dacIDZ[0], write_cmds[0], v1);
            turnKnub(0, i);
            delay(5);
            vacResp[i] = constrain(map(analogRead(0), 0, 1024, 0, 1024), 0, 1024);
            delay(5);  
      }
       
      delay(5);
   
         Z_OSCMessage message;
         message.setAddress(computer, scPort);
         message.setZ_OSCMessage(oscAdr3, "s", "done");
         client.send(&message);
    }
   
   
   if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[2])){
      
     int tmpIndx = rcvMes->getInteger32(0);
     int tmpVal =  rcvMes->getInteger32(1);
     
     lookup[tmpIndx] = tmpVal;
     
   
 }
   
   if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[3])){ 
    
      for(int i = 0; i<255; i++){
        
        long int tmpIndx = (long int)i;
        long int tmpValue = (long int)vacResp[i];
        
        Z_OSCMessage message;
        message.setAddress(computer, scPort);
        message.setZ_OSCMessage(oscAdr1, "ii", &tmpIndx, &tmpValue);
       
        client.send(&message);
        delay(5);
    }
         Z_OSCMessage message;
         message.setAddress(computer, scPort);
         message.setZ_OSCMessage(oscAdr2, "s", "done");
         client.send(&message);
    }
    
  }
}


void turnKnub(byte knubNum,byte knubVal){
    
    byte hiRead = 255 - knubVal;

    lowVal = map(lookup[knubVal], 0, 255, vacMin, vacMax);
    highVal = map(lookup[hiRead], 0, 255, vacMin, vacMax);

    // lowVal = map(knubVal, 0, 255, vacMin, vacMax);
    // highVal = map(hiRead, 0, 255, vacMin, vacMax);

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
   
    }
    
}
