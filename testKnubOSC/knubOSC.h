//byte myMac[] = { 11, 22, 33, 44, 55, 66};
byte myMac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};

byte myIp[]  = { 192, 168, 0, 8 };
uint16_t  myPort  = 10000;

Z_OSCServer server;
Z_OSCMessage *rcvMes;

char *subAddress[5]={"/pn", "/kn", "/k", "/ks", "kl"};
char *knubbieName;
char *presetName;
/*
void knubDoOsc(){

  if(server.available()){
  
    rcvMes = server.getMessage();
   
   //deals with pots messages
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[0])){
        presetName = rcvMes->getString(0);
        Serial.println(presetName);    
    }

    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[1])){
        
        //Serial.println("hey");

        knubbieName = rcvMes->getString(0);
        
        /*
        then check knubbieName[0] for knubbie index
        */
/*    
        Serial.println(knubbieName);    
    }


    
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[2])){
      

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

    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[3])){

        Serial.println("SAVING");

    }
   
  }
}
*/
