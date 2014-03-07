/* this is used to test thib's eval*/


#include <SPI.h>
#include <Ethernet.h>
#include <Z_OSC.h>
#include <Wire.h>

int val, analogInVal, minDacVal, maxDacVal;

char knubValues[8];

byte myMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte myIp[]  = { 192, 168, 0, 23 };
int  myPort  = 10000;


byte lin[255];
byte preDist[255];
byte lut[255];

char *subAddress[3]={"/K1", "/K2", "/K3", "/K3"};

Z_OSCServer server;
Z_OSCMessage *rcvMes;


int minSend = 0;
int maxSend = 4095;

byte DAC1_addr = B0001100;
byte DAC2_addr = B0001101;


byte writeDacA = B00110001;
byte writeDacB = B00110010;
byte writeDacC = B00110100;
byte writeDacD = B00111000;

byte writeCommands[4] = {writeDacA, writeDacB,writeDacC, writeDacD};

uint16_t  lowVal, highVal, prevExp;


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

void setup(){

  Wire.begin();
  Ethernet.begin(myMac, myIp);
  server.sockOpen(myPort);
  Serial.begin(9600);
}


void loop(){

if(server.available()){
  
    rcvMes = server.getMessage();
    
    //deals with first knubbie
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[0])){
        
        val = rcvMes->getInteger32(0);
        //Serial.println(val);

        turnKnub(0, val);

        analogInVal = analogRead(0);
        analogInVal = map(analogInVal, 0, 1024, 0, 255);
        Serial.println(analogInVal);
    }
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[1])){
        
        val = rcvMes->getInteger32(0);

        turnKnub(1, val);

        analogInVal = analogRead(0);
        analogInVal = map(analogInVal, 0, 1024, 0, 255);
        Serial.println(analogInVal);
    }
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[2])){
        
        val = rcvMes->getInteger32(0);

        turnKnub(2, val);

        analogInVal = analogRead(0);
        analogInVal = map(analogInVal, 0, 1024, 0, 255);
        Serial.println(analogInVal);
    }
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[3])){
        
        val = rcvMes->getInteger32(0);

        turnKnub(3, val);

        analogInVal = analogRead(0);
        analogInVal = map(analogInVal, 0, 1024, 0, 255);
        Serial.println(analogInVal);
    }
  }
}

void turnKnub(byte knubNum,byte knubVal){
    
    byte hiRead = 255 - knubVal;

    lowVal = map(lookup[knubVal], 0, 255, minSend, maxSend);
    highVal = map(lookup[hiRead], 0, 255, minSend, maxSend);


  switch(knubNum){
  
    case 0:
      writeDac(0, lowVal);
      writeDac(1, highVal);
    break;
    case 1:
      writeDac(2, lowVal);
      writeDac(3, highVal);
    break;
    case 2:
      writeDac2(0, lowVal);
      writeDac2(1, highVal);
    break;
    case 3:
      writeDac2(2, lowVal);
      writeDac2(3, highVal);
     break;

  }
}

void writeDac(int wichDac, int value){

    Wire.beginTransmission(DAC1_addr);
    Wire.write(writeCommands[wichDac]);
    Wire.write(highByte(value));
    Wire.write(lowByte(value));
    Wire.endTransmission();
}

void writeDac2(int wichDac, int value){

    Wire.beginTransmission(DAC2_addr);
    Wire.write(writeCommands[wichDac]);
    Wire.write(highByte(value));
    Wire.write(lowByte(value));
    Wire.endTransmission();
}