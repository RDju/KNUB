#include "SoftwareSerial.h"

#define baseKnubCC 107


SoftwareSerial midiSerial(2, 3);
byte inMessage[3];
byte inRead  = 0;


void midiInRead(){


	/*reads incomming PGM change and CC's (for modulation of individual parameters)*/

	if(midiSerial.available()>0){


		if(inRead < 3){

			inMessage[inRead] = midiSerial.read();
			inRead ++;

		}

		if(inRead >=3){
			
			inRead = 0;
			
			Serial.print(inMessage[0]);
			Serial.print(", ");
			Serial.print(inMessage[1]);
			Serial.print(", ");
			Serial.println(inMessage[2]);
			
			/* then here test if message is a PGM or CC 
			for now I use noteOn messages*/
			
			uint16_t readAdr = (inMessage[1]-56)*presetSize;

			readKnubPreset(eepromAddr1, readAdr, &currentPreset);
			time2ChangePage = true;


		}
	}
}