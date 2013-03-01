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
//was 1959
#define maxSend 3276
//was 1172
#define minSend 1011

byte write_cmds[] = {B01011000, B01011010, B01011100, B01011110};

byte dacID = B1100000; 
byte knob1_ch1  = B01000000;
byte knob1_ch2 = B01000010;

byte knob2_ch1 = B01000100;
byte knob2_ch2 = B01000110;

Z_OSCClient client;
int optoId;
int val;
int an1, an2;
int lg, mult;
boolean goLinear;

int vacResp[255];

int lookup[255] = {
0 ,  55 ,  59 ,  62 ,  64 ,  66 ,  67 ,  69 ,  70 ,  72 ,  75 ,  76 ,  78 ,  
80 ,  83 ,  84 ,  85 ,  87 ,  88 ,  88 ,  89 ,  90 ,  91 ,  92 ,  93 ,  94 ,  
95 ,  96 ,  97 ,  98 ,  99 ,  101 ,  102 ,  103 ,  104 ,  105 ,  106 ,  107 ,  107 ,  108 ,  109 ,  110 ,  111 ,  
111 ,  112 ,  113 ,  113 ,  113 ,  114 ,  115 ,  115 ,  115 ,  116 ,  116 ,  117 ,  118 ,  118 ,  119 ,  119 ,  119 ,  
120 ,  120 ,  121 ,  122 ,  122 ,  122 ,  123 ,  123 ,  124 ,  124 ,  125 ,  125 ,  125 ,  126 ,  126 ,  126 ,  
127 ,  127 ,  128 ,  128 ,  129 ,  130 ,  130 ,  131 ,  131 ,  132 ,  132 ,  133 ,  133 ,  134 ,  135 ,  135 ,  136 ,  
136 ,  137 ,  137 ,  138 ,  138 ,  138 ,  139 ,  139 ,  140 ,  141 ,  141 ,  142 ,  142 ,  142 ,  143 ,  143 ,  
144 ,  145 ,  145 ,  146 ,  146 ,  146 ,  147 ,  147 ,  148 ,  148 ,  149 ,  149 ,  149 ,  150 ,  150 ,  150 ,  
151 ,  151 ,  152 ,  152 ,  153 ,  153 ,  153 ,  154 ,  154 ,  155 ,  155 ,  155 ,  156 ,  156 ,  157 ,  157 ,  
158 ,  158 ,  159 ,  159 ,  160 ,  160 ,  161 ,  161 ,  162 ,  162 ,  163 ,  163 ,  163 ,  164 ,  165 ,  165 ,  
165 ,  166 ,  167 ,  167 ,  167 ,  168 ,  168 ,  169 ,  170 ,  170 ,  171 ,  171 ,  172 ,  173 ,  173 ,  174 ,  175 ,  
175 ,  176 ,  176 ,  177 ,  177 ,  178 ,  178 ,  179 ,  179 ,  180 ,  181 ,  181 ,  181 ,  182 ,  183 ,  183 ,  184 ,  185 ,  185 ,  186 ,  
186 ,  187 ,  187 ,  188 ,  189 ,  189 ,  190 ,  190 ,  191 ,  191 ,  192 ,  193 ,  194 ,  195 ,  196 ,  197 ,  198 ,  
199 ,  201 ,  202 ,  204 ,  205 ,  206 ,  208 ,  209 ,  210 ,  214 ,  215 ,  217 ,  219 ,  220 ,  222 ,  223 ,  225 ,  228 ,  
229 ,  230 ,  231 ,  232 ,  234 ,  234 ,  235 ,  235 ,  236 ,  236 ,  236 ,  236 ,  236 ,  236 ,  236 ,  236 ,  236 ,  235 ,  235 ,  235 ,  236 ,  236 ,  236 ,  236,  236 ,  
236};


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
            
           
            int v1 = map(lookup[i], 0, 255, minSend, maxSend);
            int v2 = map(lookup[255 - i], 0, 255, minSend, maxSend);
            //int v2 = 0;
            multiWriteDac(dacID, knob1_ch1, knob1_ch2, v1, v2);
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

void writeDac(byte addr, byte wrid, int val){
  //map(val,0, 255, 0, 1024);
  Wire.beginTransmission(addr);
  Wire.write(wrid);
  Wire.write(highByte(val));
  Wire.write(lowByte(val)); 
  Wire.endTransmission();
}

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
