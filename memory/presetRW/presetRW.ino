
#include "utility/twi.h"
#include "Wire.h"
#include "memory.h"
#include "presets.h"


#define eepromAddr1 0x50 

byte presetIndx  = 0;


 void setup() 
{
	Serial.begin(9600);
	Wire.begin();
	
	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize);
	
	writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset2);

	delay(1000);

	Serial.println("reading");

	readKnubPreset(eepromAddr1, presetIndx*presetSize, &currentPreset);

 	delay(1000);
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
 

	Serial.println("KNUBBIE 1 MODSOURCE: ");
	Serial.println(currentPreset.knubbies[0].modSource);
	Serial.println("KNUBBIE 1 STATE: ");
	Serial.println(currentPreset.knubbies[0].state);
	Serial.println("KNUBBIE 1 NUMLOOP: ");
	Serial.println(currentPreset.knubbies[0].numLoop);

	Serial.println("----------------------");
   	Serial.println("KNUBBIE 2 PARAMS : ");

   for(int i = 0; i<paramLength; i++){

   		Serial.println(currentPreset.knubbies[1].params[i]);
	}
 

	Serial.println("KNUBBIE 1 MODSOURCE: ");
	Serial.println(currentPreset.knubbies[1].modSource);
	Serial.println("KNUBBIE 1 STATE: ");
	Serial.println(currentPreset.knubbies[1].state);
	Serial.println("KNUBBIE 1 NUMLOOP: ");
	Serial.println(currentPreset.knubbies[1].numLoop);

	Serial.println("----------------------");
   	Serial.println("KNUBBIE 3 PARAMS : ");

   for(int i = 0; i<paramLength; i++){

   		Serial.println(currentPreset.knubbies[2].params[i]);
	}
 

	Serial.println("KNUBBIE 1 MODSOURCE: ");
	Serial.println(currentPreset.knubbies[2].modSource);
	Serial.println("KNUBBIE 1 STATE: ");
	Serial.println(currentPreset.knubbies[2].state);
	Serial.println("KNUBBIE 1 NUMLOOP: ");
	Serial.println(currentPreset.knubbies[2].numLoop);


 }	


 void loop() 
{
	
}
