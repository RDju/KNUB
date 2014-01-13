
#include "SPI.h"
#include "Ethernet.h" // version IDE 0022

#include "Z_OSC.h"
#include "knubOSC.h"
#include "EthernetUdp.h"
#include "EthernetBonjour.h"


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
    EthernetBonjour.addServiceRecord("THE KNUB by ComboSquare._osc",10000,MDNSServiceUDP);
    

  }

 server.sockOpen(myPort);
}

void loop() {

	
  

	 //knubDoOsc();
   
   
}

 
 /*

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // this check is only needed on the Leonardo:
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
}

void loop() {

}

*/