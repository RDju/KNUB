
#include "SoftwareSerial.h"


SoftwareSerial looperSerial(11, 4);

byte cc = 191;
byte ccn = 80;
byte ccv1 = 127;
byte ccv2 = 0;

byte switchSysEx[8] = {240, 0, 0, 7, 1, 8, 16, 247};

void setup() {

	//Serial.begin(31250);
	looperSerial.begin(31250);

	sendSwitchSysEx();
	delay(1000);
}

void loop() {
	
	looperSerial.write(cc);
	looperSerial.write(ccn);
	looperSerial.write(ccv1);	

	delay(500);
		
	
	looperSerial.write(cc);
	looperSerial.write(ccn);
	looperSerial.write(ccv2);

	delay(500);


}


void sendSwitchSysEx(){


	for(uint8_t i = 0; i<8;i++){
		
		Serial.write(switchSysEx[i]);
		}
	}

