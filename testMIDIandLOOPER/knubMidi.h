	//#include "SoftwareSerial.h"

#define DEBUG_MIDI //uncomment this to activate midi debugging

SoftwareSerial midiSerial(7, 10);
byte inMessage[2];
byte inRead  = 0;


void midiInRead(byte pageLev){


	/*reads incomming PGM change and CC's (for modulation of individual parameters)*/

	if(midiSerial.available()>0){


		if(inRead < 2){

			inMessage[inRead] = midiSerial.read();
			inRead ++;

		}

		if(inRead >=2 && inMessage[0] == 192){
			
			inRead = 0;
			
		}	
				
				if(inMessage[1] == 5){


					switchLoop(0, 0);
				}else if(inMessage[1] == 6){


					switchLoop(0, 1);
				}
			
			
		}
	}

