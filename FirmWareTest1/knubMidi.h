//#include "SoftwareSerial.h"

//#define DEBUG_LOAD_PRESET //uncomment this to activate midi debugging
#include "Arduino.h"

#define upPin 6
#define downPin 5

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
uint8_t toRead = 0;

void midiInRead(){


	/*reads incomming PGM change and CC's (for modulation of individual parameters)*/

	//must modify this to reflect preset num and pgm nums


	if(midiSerial.available()>0){
			
			inMessage[0] = midiSerial.read();//read first byte
			

			if(inMessage[0] == 192){//if first byte is PC
				
				toRead = 2;			//num of bytes to read : 2 for PC
				
				for(uint8_t i = 1; i<toRead; i++){

					inMessage[i] = midiSerial.read(); // reads remaining bytes
				}	
			
			
				//we have a valid PC message change preset accordingly

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
					toRead = 0;

				}
			}else if(inMessage[0]==176){//if first byte is CC

				toRead = 3;

				for(uint8_t i = 1; i<toRead; i++){

					inMessage[i] = midiSerial.read(); // reads remaining bytes
				}

				//we have a valid cc 

				if(inMessage[1] == 127){

					for(uint8_t i =0; i<4; i++){
       				 if(currentPreset.knubbies[i].modOn == 1){
          				turnKnub(i, map(inMessage[2], 0, 127, currentPreset.knubbies[i].params[0], currentPreset.knubbies[i].params[1]));
       					}
      				}
      			}
				toRead = 0;
			}
		}
	}
		/*
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
		*/




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
