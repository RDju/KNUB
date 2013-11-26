#include <SPI.h>
#include <Ethernet.h> // version IDE 0022

#include <Z_OSC.h>
#include "knubOSC.h"



void setup() {

	Serial.begin(9600);

	Ethernet.begin(myMac ,myIp); 
 
 	server.sockOpen(myPort);

 	Serial.println("READY");

}

void loop() {


  

	 knubDoOsc();
   
   
}

