byte myMac[] = { 11, 22, 33, 44, 55};
byte myIp[]  = { 192, 168, 0, 8 };
uint16_t  myPort  = 10000;

Z_OSCServer server;
Z_OSCMessage *rcvMes;

char *subAddress[3]={"/knub/turn", "/knub/loadBank/", "/knub/saveBank"};


void knubDoOsc(){

  if(server.available()){
  
    rcvMes = server.getMessage();
   
   //deals with pots messages
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[0])){
        
      byte wichPot = rcvMes->getInteger32(0);
      byte potVal =  rcvMes->getInteger32(1);   
        
      knubsValues[wichPot - 1] = potVal;  
      turnKnub(wichPot, 1, knubsValues[wichPot - 1]);     
    }
    
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[1])){
      writeKnubPreset(eepromAddr1, pgmNum*memOffset, (byte *)knubsValues); 
    }
    
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[2])){
      
       pgmNum = rcvMes->getInteger32(0);
       
       readKnubPreset(eepromAddr1, pgmNum*memOffset, (byte *)knubsValues);
       setKnubPgm((byte *)knubsValues);
       ///saves last used pgm
       writeSingleKnub(eepromAddr1, 0, pgmNum); 
    }
  }
}
