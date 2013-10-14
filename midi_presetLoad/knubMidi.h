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
			
			/* then here test if message is a PGM or CC */
			/*
			if(inMessage[2]!= 0){

			//Serial.println(inMessage[0]);
				//Serial.println(inMessage[1]);
				time = millis();
				readKnubPreset(eepromAddr1, (inMessage[1] - 56)*presetSize, &currentPreset);
				time = millis() - time;
				Serial.println(time);
				printCurrentPreset();

			}
			*/
		}
	}
}