#include "SoftwareSerial.h"
#include "utility/twi.h"
#include "Wire.h"
#include "memory.h"
#include "presets.h"
#include "knubMidi.h"

 




unsigned long time;

byte presetIndx  = 5;


void setup(){
	

	Serial.begin(9600);
	midiSerial.begin(31250);

	Wire.begin();

	Serial.print("writing at address : ");
	
	Serial.println(presetIndx*presetSize);
	
	writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset1);
	presetIndx ++;
	delay(1000);

	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize);
	
	writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset2);
	presetIndx++;
	delay(1000);

	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize);
		
	writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset3);


	delay(1000);
	presetIndx = 5;
	Serial.println("ready");


}
/*	
for(int  i =0; i<3;i++){


	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize);
	if(i == 0){
		writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset1);
	}else if(i == 1){

		writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset2);

	}else{

		writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset3);

	}
	delay(500);
	readKnubPreset(eepromAddr1, presetIndx*presetSize, &currentPreset);
	delay(1000);
	printCurrentPreset();
	delay(2000);
	presetIndx ++;
	}	

}	
*/

 void loop(){

	

}


void printCurrentPreset(){


	Serial.println(currentPreset.name);
 	
 	Serial.println("----------------------");
   	
   	Serial.println("KNUBBIES: ");

   	for(int i = 0; i<numKnubbies; i++){
   			Serial.println(currentPreset.knubbies[i].name);

   		}   	
 	
 	Serial.println("----------------------");
   	
   	Serial.println("KNUBBIE 1 PARAMS : ");

   	for(int i = 0; i<paramLength; i++){

   		Serial.println(currentPreset.knubbies[0].params[i]);
	}
 

	Serial.println("KNUBBIE 1 modOn: ");
	Serial.println(currentPreset.knubbies[0].modOn);
	Serial.println("KNUBBIE 1 STATE: ");
	Serial.println(currentPreset.knubbies[0].state);
	Serial.println("KNUBBIE 1 NUMLOOP: ");
	Serial.println(currentPreset.knubbies[0].numLoop);

	Serial.println("----------------------");
   	Serial.println("KNUBBIE 2 PARAMS : ");

   	for(int i = 0; i<paramLength; i++){

   		Serial.println(currentPreset.knubbies[1].params[i]);
	}
 

	Serial.println("KNUBBIE 2 modOn: ");
	Serial.println(currentPreset.knubbies[1].modOn);
	Serial.println("KNUBBIE 2 STATE: ");
	Serial.println(currentPreset.knubbies[1].state);
	Serial.println("KNUBBIE 2 NUMLOOP: ");
	Serial.println(currentPreset.knubbies[1].numLoop);

	Serial.println("----------------------");
   	Serial.println("KNUBBIE 3 PARAMS : ");

   	for(int i = 0; i<paramLength; i++){

   		Serial.println(currentPreset.knubbies[2].params[i]);
	}
 

	Serial.println("KNUBBIE 3 modOn: ");
	Serial.println(currentPreset.knubbies[2].modOn);
	Serial.println("KNUBBIE 3 STATE: ");
	Serial.println(currentPreset.knubbies[2].state);
	Serial.println("KNUBBIE 3 NUMLOOP: ");
	Serial.println(currentPreset.knubbies[2].numLoop);
}

