
//#include <Client.h>
#include <SPI.h>
#include <Ethernet.h>
//#include <Server.h>

#include <Z_OSC.h>
#include <Wire.h>
//#include "knubFuncs.h"


//was 1959
#define maxSend 4095
//was 1172
#define minSend 0

byte DAC1_addr = B0001100;
byte writeDacA = B00110001;
byte writeDacB = B00110010;
byte writeDacC = B00110100;
byte writeDacD = B00111000;


uint16_t lowVal, highVal;
 
byte writeCommands[4] = {writeDacA, writeDacB,writeDacC, writeDacD};

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

char *subAddress[5]={"/turn1", "/turn2", "/turn3", "/turn4"};

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

  ///turnKnub(0, 10);

  writeDac(0, 0);
}


void loop(){

  //Serial.println(optoId);
if(server.available()){
  
    rcvMes = server.getMessage();
    
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[0])){
    
        int val = rcvMes-> getInteger32(0);

        turnKnub(0, val);
    }

    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[1])){
    
        int val = rcvMes-> getInteger32(0);

        turnKnub(1, val);
    }

    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[2])){
    
        int val = rcvMes-> getInteger32(0);

        turnKnub(2, val);
    }

    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[3])){
    
        int val = rcvMes-> getInteger32(0);

        turnKnub(3, val);
    }

    /*
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
   
   if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[4])){
      
           int val = rcvMes->getInteger32(0); 
     
            turnKnub(0, val);
            delay(5);
            vacResp[val] = constrain(map(analogRead(0), 0, 1024, 0, 1024), 0, 255);
            delay(5);  
            Serial.write(vacResp[val]);
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
  */   
  }
 
}


void turnKnub(byte knubNum,byte knubVal){
    
    byte hiRead = 255 - knubVal;

    lowVal = map(knubVal, 0, 255, minSend, maxSend);
    highVal = map(hiRead, 0, 255, minSend, maxSend);


  switch(knubNum){
  
    case 0:
      writeDac(0, lowVal);
      writeDac(1, highVal);
    break;
    case 1:
      writeDac(2, lowVal);
      writeDac(3, highVal);
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