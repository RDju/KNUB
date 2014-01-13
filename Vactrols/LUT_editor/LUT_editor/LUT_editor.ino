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

int lookup[255] = {1 ,  2 ,  3 ,  3 ,  3 ,  4 ,  4 ,  5 ,  5 ,  6 ,  6 ,  6 ,  6 ,  6 ,  6 ,  6 ,  6 ,  
7 ,  7 ,  7 ,  7 ,  8 ,  8 ,  8 ,  8 ,  8 ,  8 ,  8 ,  8 ,  8 ,  8 ,  9 ,  9 ,  9 , 
 9 ,  9 ,  9 ,  9 ,  9 ,  9 ,  9 ,  10 ,  10 ,  10 ,  10 ,  10 ,  10 ,  10 ,  10 ,  
 10 ,  10 ,  10 ,  10 ,  10 ,  11 ,  11 ,  11 ,  11 ,  11 ,  11 ,  11 ,  11 ,  11 ,  
 12 ,  12 ,  12 ,  12 ,  12 ,  13 ,  13 ,  13 ,  13 ,  13 ,  13 ,  14 ,  13 ,  13 ,  
 14 ,  14 ,  14 ,  14 ,  14 ,  14 ,  14 ,  14 ,  14 ,  14 ,  14 ,  14 ,  14 ,  14 ,  
 15 ,  15 ,  15 ,  15 ,  15 ,  15 ,  16 ,  16 ,  16 ,  16 ,  16 ,  17 ,  17 ,  17 , 
 17 ,  17 ,  17 ,  17 ,  17 ,  17 ,  17 ,  18 ,  18 ,  18 ,  18 ,  18 ,  18 ,  19 ,  
 18 ,  18 ,  19 ,  19 ,  19 ,  19 ,  19 ,  20 ,  20 ,  20 ,  20 ,  21 ,  21 ,  21 ,  
 21 ,  21 ,  21 ,  21 ,  22 ,  22 ,  23 ,  24 ,  24 ,  24 ,  24 ,  25 ,  26 ,  26 ,  
 26 ,  27 ,  27 ,  27 ,  27 ,  27 ,  27 ,  28 ,  28 ,  29 ,  29 ,  30 ,  30 ,  30 ,  
 30 ,  31 ,  31 ,  32 ,  32 ,  32 ,  33 ,  33 ,  34 ,  34 ,  35 ,  35 ,  36 ,  36 ,  
 37 ,  37 ,  37 ,  38 ,  38 ,  39 ,  39 ,  39 ,  40 ,  41 ,  41 ,  42 ,  42 ,  43 ,  
 43 ,  44 ,  45 ,  45 ,  46 ,  47 ,  47 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  54 ,  
 55 ,  56 ,  57 ,  58 ,  60 ,  61 ,  61 ,  62 ,  64 ,  64 ,  66 ,  67 ,  68 ,  70 ,  
 72 ,  73 ,  75 ,  80 ,  82 ,  85 ,  90 ,  95 ,  100 ,  106 ,  111 ,  125 ,  130 ,  
 141 ,  147 ,  152 ,  163 ,  169 ,  176 ,  180 ,  188 ,  198 ,  203 ,  209 ,  216 ,  
 221 ,  225 ,  231 ,  234 ,  239 ,  243 ,  247 ,  251 ,  252 ,  254 ,  255 ,  255, 255 };


byte myMac[] = { 11, 22, 33, 44, 55};
byte myIp[]  = { 192, 168, 0, 8 };
int  myPort  = 10000;

byte computer[] = {192, 168, 0, 5};
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
