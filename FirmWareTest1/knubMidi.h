//#include "SoftwareSerial.h"

//#define DEBUG_LOAD_PRESET //uncomment this to activate midi debugging
#include "Arduino.h"

#define upPin 5
#define downPin 6

SoftwareSerial midiSerial(7, 10);

byte inMessage[3];
byte inRead  = 0;

uint16_t prevRead = 5*presetSize;
uint8_t readindx  = 5; //this later will be change back to load ID from eeprom
uint16_t readAdr;
bool loadFlag = false;
bool prevUp, prevDown;

uint8_t debounceDelay = 5;

uint8_t baseAddr = 5;
uint8_t lastID = 5; // this later woul be removed for consistency with readindx


void midiInRead(){


	/*reads incomming PGM change and CC's (for modulation of individual parameters)*/

	//must modify this to reflect preset num and pgm nums


	if(midiSerial.available()>0){


		if(inRead < 3){

			inMessage[inRead] = midiSerial.read();
			inRead ++;

		}

		//program change:

		if(inRead >=3 && inMessage[0] == 192){
			
			inRead = 0;
			
			#ifdef DEBUG_LOAD_PRESET
				Serial.print("MIDI message: ");
				Serial.print(" ");
				Serial.print(inMessage[0]);
				Serial.print(", ");
				Serial.println(inMessage[1]);
			#endif
			
			/* PGM change to change preset*/
			readindx = inMessage[1];
			readAdr = readindx*presetSize;
			
			
				if(readindx<8 && readAdr != prevRead && loadFlag == false){
					

					loadFlag = true;
	
					readKnubPreset(eepromAddr1, readAdr, &currentPreset);
					
					updateKnubs(&currentPreset);
					
					//writeByte(eepromAddr1, lastPresetMemSpace, readindx);
					
					loadFlag = false;
					//isEdited = false;
					
					#ifdef DEBUG_LOAD_PRESET
						Serial.println(readAdr);
						debugKnubPreset(&currentPreset);
					#endif
					clearLoopsOut();
					
					// fill up loopsOut array
  					
  					for(uint8_t i = 0; i<numKnubbies; i++){

    					fillLoopsOut(currentPreset.knubbies[i].numLoop, currentPreset.knubbies[i].state);
  					}	
  					
  					// check loops state and update
  					
  					for(uint8_t i = 0; i<4; i++){

      					if(checkLoopsOut(i) == true){
          					
          					switchLoop(i, 1);
          					
      					}else{

          					switchLoop(i, 0);

      					}
    				}	
    				
    				
    				time2ChangePage = true;
					prevRead = readAdr;

				}
			
			
		}
	}
		//
		//control change:
		if(inRead >=3 && inMessage[0] == 176){

			inRead = 0;

			///turn knub 
			switch(inMessage[1]){

				case 1:
					//maybe use a lookup instead of map to save cpu
					turnKnub(0, map(inMessage[2], 0, 127, 0, 255));
				break;
				case 2:
					
					turnKnub(1, map(inMessage[2], 0, 127, 0, 255));
				break;
				case 3:
					
					turnKnub(2, map(inMessage[2], 0, 127, 0, 255));
				break;
				case 4:
					
					turnKnub(3, map(inMessage[2], 0, 127, 0, 255));
				break;

				//and so on for other knubs.
			}
		}
}



// to be double Check:
// added debounce func
void doSwitchInDec(){

  bool currUp  = digitalRead(upPin);
  bool currDown = digitalRead(downPin);


  if(currUp != prevUp){
  	
  	delay(debounceDelay);
	//Serial.println("UP");

	//index gut up then load corresponding preset:
	
	

				if(readindx < 7){
					readindx += 1;
					readAdr = readindx*presetSize;
				}

				if(readAdr != prevRead && loadFlag == false){
					

					loadFlag = true;
	
					readKnubPreset(eepromAddr1, readAdr, &currentPreset);
					
					updateKnubs(&currentPreset);
					
					//writeByte(eepromAddr1, lastPresetMemSpace, readindx);
					
					loadFlag = false;
					//isEdited = false;
					
					#ifdef DEBUG_LOAD_PRESET
						Serial.println(readAdr);
						debugKnubPreset(&currentPreset);
					#endif
					clearLoopsOut();
					
					// fill up loopsOut array
  					
  					for(uint8_t i = 0; i<numKnubbies; i++){

    					fillLoopsOut(currentPreset.knubbies[i].numLoop, currentPreset.knubbies[i].state);
  					}	
  					
  					// check loops state and update
  					
  					for(uint8_t i = 0; i<4; i++){

      					if(checkLoopsOut(i) == true){
          					
          					switchLoop(i, 1);
          					
      					}else{

          					switchLoop(i, 0);

      					}
    				}	
    				time2ChangePage = true;
					prevRead = readAdr;

				}
			
		prevUp = currUp;
	}
	
	if(currDown != prevDown){
  	
	delay(debounceDelay);
  	//Serial.println("DOWN");

  				if(readindx > 5){
					readindx -=1;
					readAdr = readindx*presetSize;
				}

				if(readAdr != prevRead && loadFlag == false){
					

					loadFlag = true;
	
					readKnubPreset(eepromAddr1, readAdr, &currentPreset);
					
					updateKnubs(&currentPreset);
					
					//writeByte(eepromAddr1, lastPresetMemSpace, readindx);
					
					loadFlag = false;
					//isEdited = false;
					
					#ifdef DEBUG_LOAD_PRESET
						Serial.println(readAdr);
						debugKnubPreset(&currentPreset);
					#endif
					clearLoopsOut();
					
					// fill up loopsOut array
  					
  					for(uint8_t i = 0; i<numKnubbies; i++){

    					fillLoopsOut(currentPreset.knubbies[i].numLoop, currentPreset.knubbies[i].state);
  					}	
  					
  					// check loops state and update
  					
  					for(uint8_t i = 0; i<4; i++){

      					if(checkLoopsOut(i) == true){
          					
          					switchLoop(i, 1);
          					
      					}else{

          					switchLoop(i, 0);

      					}
    				}	
    				time2ChangePage = true;
					prevRead = readAdr;

				}
			prevDown = currDown;
		}
		
}
