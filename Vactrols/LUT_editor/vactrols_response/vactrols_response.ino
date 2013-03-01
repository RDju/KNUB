#include <SPI.h>

/*
This is used to test the "active" region of the DAC 

*/

byte write_cmds[] = {B01011000, B01011010, B01011100, B01011110};

byte dacID = B1100000; 


//#include <Client.h>
#include <Ethernet.h>
//#include <Server.h>

#include <Z_OSC.h>

#include <Wire.h>


int val;
int an1, an2;
int lg, mult;
boolean goLinear;

int lookup[256] = {0,0,31,50,63,74,82,89,95,101,105,110,114,118,121,124,127,130,133,135,137,140,
142,144,146,148,149,151,153,154,156,158,159,160,162,163,164,166,167,168,169,170,172,173,174,175,
176,177,178,179,180,180,181,182,183,184,185,186,186,187,188,189,189,190,191,192,192,193,194,194,
195,196,196,197,198,198,199,199,200,201,201,202,202,203,203,204,204,205,206,206,207,207,208,208,
209,209,210,210,210,211,211,212,212,213,213,214,214,215,215,215,216,216,217,217,217,218,218,219,
219,219,220,220,221,221,221,222,222,222,223,223,223,224,224,225,225,225,226,226,226,227,227,227,
228,228,228,229,229,229,229,230,230,230,231,231,231,232,232,232,232,233,233,233,234,234,234,234,
235,235,235,236,236,236,236,237,237,237,237,238,238,238,238,239,239,239,239,240,240,240,240,241,
241,241,241,242,242,242,242,243,243,243,243,244,244,244,244,244,245,245,245,245,246,246,246,246,
246,247,247,247,247,247,248,248,248,248,249,249,249,249,249,250,250,250,250,250,251,251,251,251,
251,252,252,252,252,252,252,253,253,253,253,253,254,254,254,254,254};


byte myMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte myIp[]  = { 192, 168, 0, 8 };
int  myPort  = 10000;

char *subAddress[4]={"/dac/cvA", "/dac/cvB", "/dac/cvC", "/dac/cvD"};

Z_OSCServer server;
Z_OSCMessage *rcvMes;

void setup(){

  Wire.begin();
  Ethernet.begin(myMac, myIp);
  server.sockOpen(myPort);
  Serial.begin(9600);
    
  goLinear = false; 
  lg = 255;
  mult = 250;
 
}


void loop(){

if(server.available()){
  
    rcvMes = server.getMessage();
    
    //deals with dacA and dacB on the eval board
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[0])){
        
        val = rcvMes->getInteger32(0);
  
 
        an1 = analogRead(0);
        an2 = analogRead(2);
  
        
        Serial.print(an1);
        Serial.print(",");
        Serial.println(an2);
        
        //map(val, 0, 255, 1381, 1557)
        writeDac(dacID, write_cmds[0], map(val, 0, 255, 1381, 1509));
       }
  
   
   if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[1])){
        
        val = rcvMes->getInteger32(0);
  
 
        an1 = analogRead(0);
        an2 = analogRead(2);
  
        
        Serial.print(an1);
        Serial.print(",");
        Serial.println(an2);
        
        
        writeDac(dacID, write_cmds[1], map(val, 0, 255, 1381, 1557));
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
