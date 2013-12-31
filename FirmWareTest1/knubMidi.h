//#include "SoftwareSerial.h"

//#define DEBUG_LOAD_PRESET //uncomment this to activate midi debugging


//#define upPin 6
//#define checkPin 5

SoftwareSerial midiSerial(7, 10);

byte inMessage[2];
byte inRead  = 0;

uint16_t prevRead = 5*presetSize;
uint8_t readindx;
uint16_t readAdr;
bool loadFlag = false;


void midiInRead(byte pageLev){


	/*reads incomming PGM change and CC's (for modulation of individual parameters)*/

	if(midiSerial.available()>0){


		if(inRead < 2){

			inMessage[inRead] = midiSerial.read();
			inRead ++;

		}

		if(inRead >=2 && inMessage[0] == 192){
			
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
			
			if(pageLev == 2){
				if(readindx<8 && readAdr != prevRead && loadFlag == false){
					

					loadFlag = true;
	
					digitalWrite(5, HIGH);
					
					readKnubPreset(eepromAddr1, readAdr, &currentPreset);
					
					updateKnubs(&currentPreset);
					
					writeByte(eepromAddr1, lastPresetMemSpace, readindx);
					loadFlag = false;
					isEdited = false;
					
					#ifdef DEBUG_LOAD_PRESET
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
    				
    				digitalWrite(5, LOW);
    				
    				time2ChangePage = true;
					prevRead = readAdr;

				}
			}
			
		}
	}
}



// to be double Check:
/*
void doSwitchInDec(byte pageLev){

  bool currUp  = digitalRead(upPin);
  bool currDown = digitalRead(downPin);


  if(currUP != prevUp){
  	readindx ++;
  	readAdr = readindx*presetSize;
			
			if(pageLev == 2){
				if(readindx<8 && readAdr != prevRead && loadFlag == false){
					

					loadFlag = true;
	
					digitalWrite(5, HIGH);
					
					readKnubPreset(eepromAddr1, readAdr, &currentPreset);
					
					updateKnubs(&currentPreset);
					
					writeByte(eepromAddr1, lastPresetMemSpace, readindx);
					loadFlag = false;
					isEdited = false;
					
					#ifdef DEBUG_LOAD_PRESET
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
    				
    				digitalWrite(5, LOW);
    				
    				time2ChangePage = true;
					prevRead = readAdr;


					prevUp = currUP;

}

	if(currDwon != prevDown){
  	readindx -= 1;
  	readAdr = readindx*presetSize;
			
			if(pageLev == 2){
				if(readindx<8 && readAdr != prevRead && loadFlag == false){
					

					loadFlag = true;
	
					digitalWrite(5, HIGH);
					
					readKnubPreset(eepromAddr1, readAdr, &currentPreset);
					
					updateKnubs(&currentPreset);
					
					writeByte(eepromAddr1, lastPresetMemSpace, readindx);
					loadFlag = false;
					isEdited = false;
					
					#ifdef DEBUG_LOAD_PRESET
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
    				
    				digitalWrite(5, LOW);
    				
    				time2ChangePage = true;
					prevRead = readAdr;


					prevUp = currUP;

	}

}

*/