#include "SoftwareSerial.h"

#define baseKnubCC 107


SoftwareSerial midiSerial(2, 3);
byte inMessage[3];
byte inRead  = 0;


void midiInRead(){


	/*reads incomming PGM change and CC's (for modulation of individual parameters)*/

	if(midiSerial.available()>0 && time2ChangePage == false){


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
				
			if(inMessage[2]!= 0){


				
				readKnubPreset(eepromAddr1, (inMessage[1] - 56)*presetSize, &currentPreset);
				delay(50);
		
				time2ChangePage = true;

			}
			
		

		}
	}
}