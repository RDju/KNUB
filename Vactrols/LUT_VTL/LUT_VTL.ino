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
//#include "knubFuncs.h"

 
int vacMin, vacMax;

//DAC communication MCP4728
// might change those to PROGMEM prog_uchar arrays

byte write_cmds[] = {B01011000, B01011010, B01011100, B01011110}; //for single writes not used here.

byte dacIDZ[] = {B1100000, B1100001, B1100010, B1100011};// 4x4 dacs = 16 dacs so 8 knubs.

byte knob1_ch1  = B01000000;
byte knob1_ch2 = B01000010;

byte knob2_ch1 = B01000100;
byte knob2_ch2 = B01000110;

uint16_t  lowVal, highVal, prevExp;


Z_OSCClient client;
int optoId;
int val;
int an1, an2;
int lg, mult;
boolean goLinear;

int vacResp[256];

int lookup[256] = {0 ,  0,  4 ,  9 ,  11 ,  13 ,  15 ,  16 ,  17 ,  19 ,  20 ,  
21 ,  22 ,  24 ,  25 ,  26 ,  27 ,  27 ,  28 ,  29 ,  29 ,  30 ,  31 ,  31 ,  
32 ,  33 ,  33 ,  34 ,  34 ,  35 ,  35 ,  36 ,  36 ,  37 ,  37 ,  37 ,  38 ,  
38 ,  39 ,  39 ,  39 ,  39 ,  40 ,  40 ,  41 ,  41 ,  42 ,  42 ,  42 ,  43 ,  
43 ,  43 ,  43 ,  43 ,  44 ,  44 ,  44 ,  44 ,  44 ,  45 ,  45 ,  46 ,  46 ,  
46 ,  46 ,  46 ,  46 ,  46 ,  47 ,  47 ,  47 ,  47 ,  47 ,  47 ,  48 ,  48 ,  
48 ,  48 ,  49 ,  49 ,  49 ,  49 ,  49 ,  49 ,  49 ,  50 ,  50 ,  50 ,  50 ,  
50 ,  50 ,  50 ,  50 ,  50 ,  51 ,  51 ,  51 ,  51 ,  51 ,  51 ,  52 ,  52 ,  
52 ,  52 ,  52 ,  52 ,  53 ,  53 ,  53 ,  53 ,  53 ,  54 ,  54 ,  54 ,  54 ,  
54 ,  55 ,  55 ,  55 ,  55 ,  55 ,  56 ,  56 ,  56 ,  56 ,  57 ,  57 ,  57 ,  
58 ,  58 ,  58 ,  58 ,  59 ,  59 ,  59 ,  60 ,  60 ,  60 ,  61 ,  61 ,  61 ,  
61 ,  62 ,  62 ,  62 ,  63 ,  63 ,  63 ,  64 ,  64 ,  64 ,  64 ,  65 ,  65 ,  
65 ,  66 ,  66 ,  66 ,  66 ,  67 ,  67 ,  68 ,  68 ,  68 ,  69 ,  69 ,  70 ,  
70 ,  71 ,  72 ,  72 ,  73 ,  74 ,  74 ,  75 ,  76 ,  76 ,  77 ,  78 ,  78 ,  
79 ,  80 ,  81 ,  82 ,  83 ,  84 ,  85 ,  86 ,  87 ,  88 ,  89 ,  90 ,  90 ,  
91 ,  92 ,  93 ,  94 ,  96 ,  96 ,  97 ,  99 ,  100 ,  102 ,  103 ,  105 ,  107 ,  
109 ,  110 ,  112 ,  115 ,  117 ,  120 ,  122 ,  125 ,  127 ,  131 ,  134 ,  138 ,  
141 ,  143 ,  146 ,  149 ,  152 ,  155 ,  161 ,  164 ,  167 ,  172 ,  176 ,  188 ,  
203 ,  215 ,  224 ,  233 ,  238 ,  241 ,  244 ,  247 ,  249 ,  251 ,  252 ,  254 ,  
255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  
255, 255};


byte myMac[] = { 11, 22, 33, 44, 55};
byte myIp[]  = { 192, 168, 0, 23 };
int  myPort  = 10000;

byte computer[] = {192, 168, 0, 5};
int scPort = 57120;

char *subAddress[6]={"/O2A/move","/O2A/doPot", "/O2A/lookupVal", "/O2A/tellMe", "/vacMin", "/vacMax"};

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



vacMin = 1210;
vacMax = 2500;

}


void loop(){

  //Serial.println(optoId);
if(server.available()){
  
    rcvMes = server.getMessage();

    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[0])){
      
            int val = rcvMes->getInteger32(0)
     
            turnKnub(0, i);
           
      }


    //deals with pot1
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[1])){
      
          for(int i = 0; i <255; i++){
     
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

    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[4])){

      vacMin = rcvMes->getInteger32(0);
    }
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[5])){

      vacMax = rcvMes->getInteger32(0);
    }
  }
}
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


    lowVal = map(lookup[knubVal], 0, 255, vacMin, vacMax);
    highVal = map(lookup[hiRead], 0, 255, vacMin, vacMax);
 

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
