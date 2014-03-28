
#include "SPI.h"
#include "Ethernet.h" // version IDE 0022

#include "Z_OSC.h"
//#include "knubOSC.h"
#include "EthernetUdp.h"
#include "EthernetBonjour.h"

byte myMac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};

byte myIp[]  = { 192, 168, 144, 8 };
uint16_t  myPort  = 10000;

Z_OSCServer server;
Z_OSCMessage *rcvMes;

char *subAddress[5]={"/pn", "/kn", "/k", "/ks", "kl"};
char *knubbieName;
char *presetName;



void setup() {

	Serial.begin(9600);

	
  //try to get IP via DHCP
  
  if(Ethernet.begin(myMac) ==0){

    //if doesn't work use fixed IP
    Ethernet.begin(myMac, myIp);
  
  }else{
    // get IP via DHCP
    Ethernet.begin(myMac);
    //startBonjour
    EthernetBonjour.begin("KNUB");
    EthernetBonjour.addServiceRecord("theKnub._osc",10000,MDNSServiceUDP);
    

  }

  server.sockOpen(myPort);
}

void loop() {

  if(server.available()){
  
    rcvMes = server.getMessage();
   
   //deals with pots messages
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[0])){
       
        Serial.println(rcvMes->getZ_OSCAddress());    
    }

  }
   
}
