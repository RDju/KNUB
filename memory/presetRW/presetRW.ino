
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
	
	writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset1);
	
	delay(1000);

	Serial.println("reading");

	readKnubPreset(eepromAddr1, presetIndx*presetSize, &currentPreset);
 	delay(1000);
 	
   	Serial.println("loaded NAME: ");
   		for(int i = 0; i<maxNameLength; i++){

   			Serial.println(currentPreset.name[i]);

   		}


   	Serial.println("loaded ID: ");
   	for(int  i= 0; i<IDLength; i++){
   		Serial.println(currentPreset.ID[i]);
   }
 }


 void loop() 
{
	
}
