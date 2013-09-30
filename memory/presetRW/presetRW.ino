
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

	Serial.print("writing ID: ");
	Serial.println(preset1.ID[IDLength - 1]);
	
	writeKnubPresetID(eepromAddr1, (presetIndx*presetSize), &preset1);
	
	delay(1000);

	
	Serial.print("loading from address: ");
	Serial.println(presetIndx*presetSize);

   	readKnubPresetID(eepromAddr1, (presetIndx*presetSize), &currentPreset);
 	
 	delay(1000);
   	Serial.print("loaded ID: ");
   	Serial.println(currentPreset.ID[IDLength - 1]);
 }


 void loop() 
{
	
}
