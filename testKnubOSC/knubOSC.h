byte myMac[] = { 11, 22, 33, 44, 55};
byte myIp[]  = { 192, 168, 0, 8 };
uint16_t  myPort  = 10000;

Z_OSCServer server;
Z_OSCMessage *rcvMes;

char *subAddress[5]={"pn", "/kn", "/k", "/ks", "kl"};
char *knubbieName;

void knubDoOsc(){

  if(server.available()){
  
    rcvMes = server.getMessage();
   
   //deals with pots messages
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[0])){
        knubbieName = rcvMes->getString(0);
        Serial.println(knubbieName);    
    }
    
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[1])){
      

        unsigned long knubParams = rcvMes->getInteger32(0);
        //Serial.println(knubParams);

        Serial.println(knubParams);
        Serial.println(" ");
        

        Serial.println("knubbie Num: ");
        Serial.println((knubParams>>24)&255);


        Serial.println(knubParams&65535);
        Serial.println(" ");
        
        Serial.println("firstVal");
        Serial.println((knubParams&65535)&255);
        Serial.println(" ");
        
        Serial.println("secondVal");
        Serial.println(((knubParams&65535)>>8)&255);
        
        Serial.println("curve");
        Serial.println(((knubParams>>16)&255)&7);

        Serial.println("exp");
        Serial.println((((knubParams>>16)&255)>>3)&1);

        Serial.println("state");
        Serial.println((((knubParams>>16)&255)>>4)&1);

        Serial.println("loop");
        Serial.println((((knubParams>>16)&255)>>5)&7);

    }
   
  }
}
