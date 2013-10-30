#include "SoftwareSerial.h"

//#define DEBUG_MIDI //uncomment this to activate midi debugging


SoftwareSerial midiSerial(7, 10);
byte inMessage[3];
byte inRead  = 0;
uint16_t prevRead = 5*presetSize;
uint16_t readAdr;
bool loadFlag = false;

void midiInRead(byte pageLev){


	/*reads incomming PGM change and CC's (for modulation of individual parameters)*/

	if(midiSerial.available()>0){


		if(inRead < 3){

			inMessage[inRead] = midiSerial.read();
			inRead ++;

		}

		if(inRead >=3){
			
			inRead = 0;
			
			#ifdef DEBUG_MIDI
				Serial.print(inMessage[0]);
				Serial.print(", ");
				Serial.print(inMessage[1]);
				Serial.print(", ");
				Serial.println(inMessage[2]);
			#endif
			
			/* then here test if message is a PGM or CC 
			for now I use noteOn messages*/
			
			 readAdr = (inMessage[1]-56)*presetSize;
			
			if(pageLev == 2){
				if(readAdr != prevRead && loadFlag == false){
					loadFlag = true;
					readKnubPreset(eepromAddr1, readAdr, &currentPreset);
					delay(50);	
					Serial.println("CALLING KNUB UPDATE");
					updateKnubs(&currentPreset);
					time2ChangePage = true;
					prevRead = readAdr;
					loadFlag = false;
				}
			}
		}
	}
}