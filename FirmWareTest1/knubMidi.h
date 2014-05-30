//#include "SoftwareSerial.h"

//#define DEBUG_LOAD_PRESET //uncomment this to activate midi debugging
#include "Arduino.h"

#define upPin A2
#define downPin A1

SoftwareSerial midiSerial(7, 10);

byte inMessage[3];
byte inRead  = 0;

byte wichKnubbie;
byte filteredCC;

byte pageLevel = 0;

int readindx  = 0; //this later will be change back to load ID from eeprom
int tempReadindx  = 0;
boolean isTempReadindx = false;
unsigned int readAdr;
unsigned int tempReadAdr;

bool prevUp, prevDown;


uint8_t debounceDelay = 5;

uint8_t baseAddr = 0;
uint8_t lastID = 0; // this later woul be removed for consistency with readindx
uint8_t toRead = 0;

unsigned int prevRead = ((readindx - baseAddr)*presetSize)+baseAddr;


byte midiEchoVal[8][4] = {{11, 12, 13, 14}, {21, 22, 23, 24}, {31, 32, 33, 34}, {41, 42, 43, 44}, {51, 52, 53, 54}, {61, 62, 63, 64}, {71, 72, 73, 74}, {81, 82, 83, 84}};


void sendCC(byte num, byte val){

	midiSerial.write(176);
	midiSerial.write(num);
	midiSerial.write(val);
}

void sendNote(byte num, byte val){

	midiSerial.write(144);
	midiSerial.write(num);
	midiSerial.write(val);
}


void midiEcho(aKnubPreset *kpreset){

	for(uint8_t i = 0;i<numKnubbies; i++){

		sendCC(midiEchoVal[i][0], map(kpreset->knubbies[i].params[0], 0, 255, 0, 127));
		delay(10);
		sendCC(midiEchoVal[i][1], map(kpreset->knubbies[i].params[1], 0, 255, 0, 127));
		delay(10);
		sendCC(midiEchoVal[i][2], kpreset->knubbies[i].params[2]);
		delay(10);
		sendCC(midiEchoVal[i][3], kpreset->knubbies[i].params[4]);
		delay(10);
	}

	for(uint8_t i = 0; i<4; i++){

      	if(checkLoopsOut(i) == true){
          					
          	sendNote(i + 36, 127);
          	delay(10);				
      		}else{

          	sendNote(i + 36, 0);
          	delay(10);
      		}
    	}	
}

void midiSwitch(uint8_t wich, uint8_t val){

	
		switchLoop(wich, val);
		//setLoopsOut(wich, val);	
		switchUILed(wich, val);
	
		switch(wich){
			//rat
			case 0:
				currentPreset.knubbies[0].params[3] = val;
				currentPreset.knubbies[1].params[3] = val;
				currentPreset.knubbies[2].params[3] = val;
			break;
			//wah
			case 1:
				currentPreset.knubbies[3].params[3] = val;
			break;
			//tremolo
			case 2:
				currentPreset.knubbies[4].params[3] = val;
				currentPreset.knubbies[5].params[3] = val;
			break;		
			//delay
			case 3:
				currentPreset.knubbies[6].params[3] = val;
				currentPreset.knubbies[7].params[3] = val;
			break;
		}	
}

void midiInRead(){


  /*reads incomming PGM change and CC's (for modulation of individual parameters)*/

  //must modify this to reflect preset num and pgm nums
  if(midiSerial.available()>0){

    inMessage[0] = midiSerial.read();//read first byte


    if(inMessage[0] == 192){

      toRead = 2;			//num of bytes to read : 2 for PC

      for(uint8_t i = 1; i<toRead; i++){

        inMessage[i] = midiSerial.read(); // reads remaining bytes
      }	


      //we have a valid PC message change preset accordingly

      //for use with midi moose PC num + 4
      readindx = inMessage[1];
      readAdr = ((readindx - baseAddr)*presetSize)+baseAddr;


      if(readindx<60 && readAdr != prevRead){


        readKnubPreset(eepromAddr1, readAdr, &currentPreset);

        updateKnubs(&currentPreset);

        //writeByte(eepromAddr1, lastPresetMemSpace, readindx);

        //isEdited = false;

#ifdef DEBUG_LOAD_PRESET
        Serial.println(readAdr);
        debugKnubPreset(&currentPreset);
#endif

        updateLoopsOut(&currentPreset);	
        
        midiEcho(&currentPreset);

        time2ChangePage = true;
        prevRead = readAdr;

      }
    } else if(inMessage[0] == 144){
				
	toRead = 3;

	for(uint8_t i = 1; i<toRead; i++){
		inMessage[i] = midiSerial.read();
	}
						
	if(inMessage[1] == 40){
		pageLevel = 4;
		time2ChangePage = true;


	}else{

	        midiSwitch(inMessage[1] - 36, inMessage[2]/127);
	}
    }else if(inMessage[0]==176){
						
	toRead = 3;

	for(uint8_t i = 1; i<toRead; i++){
	      inMessage[i] = midiSerial.read();
	}
					
	////manipulate CCNUm
	if(inMessage[1] > 10 && inMessage[1]<15){
									
		filteredCC = inMessage[1] - 11;	
						
		wichKnubbie = 0;
							
	}else if(inMessage[1]>20 && inMessage[1]<25){


		filteredCC = inMessage[1] - 21;	
							
		wichKnubbie = 1;

	}else if(inMessage[1]>30 && inMessage[1]<35){


		filteredCC = inMessage[1] - 31;	
		wichKnubbie = 2;

	}else if(inMessage[1]>40 && inMessage[1]<45){


		filteredCC = inMessage[1] - 41;	
		wichKnubbie = 3;

	}else if(inMessage[1]>50 && inMessage[1]<55){


		filteredCC = inMessage[1] - 51;	
		wichKnubbie = 4;

	}else if(inMessage[1]>60 && inMessage[1]<65){


		filteredCC = inMessage[1] - 61;	
		wichKnubbie = 5;

	}else if(inMessage[1]>70 && inMessage[1]<75){


		filteredCC = inMessage[1] - 71;	
		wichKnubbie = 6;

	}else if(inMessage[1]>80 && inMessage[1]<85){


		filteredCC = inMessage[1] - 81;	
		wichKnubbie = 7;
	}
        /////////////////////////////////////////////////

	switch(filteredCC){

		case 0:
		  currentPreset.knubbies[wichKnubbie].params[0] = map(inMessage[2], 0, 127, 0, 255);
		  turnKnub(wichKnubbie, currentPreset.knubbies[wichKnubbie].params[0]);
		  break; 
	        case 1:
		  currentPreset.knubbies[wichKnubbie].params[1] = map(inMessage[2], 0, 127, 0, 255);
		  break;
		case 2:
		  currentPreset.knubbies[wichKnubbie].params[2] = inMessage[2];
		  break;
		case 3:
		  currentPreset.knubbies[wichKnubbie].params[4] = inMessage[2];
		  break;
	}
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

    if(readindx < 60){
      readindx += 1;
      readAdr = ((readindx-baseAddr)*presetSize)+baseAddr;
    }

    if(readAdr != prevRead){

      readKnubPreset(eepromAddr1, readAdr, &currentPreset);

      updateKnubs(&currentPreset);

      //writeByte(eepromAddr1, lastPresetMemSpace, readindx);

      //isEdited = false;

#ifdef DEBUG_LOAD_PRESET
      Serial.println(readAdr);
      debugKnubPreset(&currentPreset);
#endif

      updateLoopsOut(&currentPreset);
      
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
      readAdr = ((readindx-baseAddr)*presetSize)+baseAddr;
    }

    if(readAdr != prevRead){

      readKnubPreset(eepromAddr1, readAdr, &currentPreset);

      updateKnubs(&currentPreset);

      //writeByte(eepromAddr1, lastPresetMemSpace, readindx);

      //isEdited = false;

#ifdef DEBUG_LOAD_PRESET
      Serial.println(readAdr);
      debugKnubPreset(&currentPreset);
#endif

      updateLoopsOut(&currentPreset);

      time2ChangePage = true;
      prevRead = readAdr;

    }
    prevDown = currDown;
  }

}

