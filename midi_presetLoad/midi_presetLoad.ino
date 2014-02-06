#include <SoftwareSerial.h>
#include "utility/twi.h"
#include "Wire.h"

#include "presets.h"
#include "knubUtils.h"
#include "knubMidi.h"


 unsigned long time;

byte presetIndx  = 5;


void setup(){
	

	Serial.begin(9600);
	midiSerial.begin(31250);

	Wire.begin();
	delay(500);
	
	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize+1);
	
	writeKnubPreset(eepromAddr1, presetIndx*presetSize+1, &preset1);
	
	delay(1000);
	
	presetIndx ++;
	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize+1);
	
	writeKnubPreset(eepromAddr1, presetIndx*presetSize+1, &preset2);
	
	delay(1000);

	presetIndx++;
	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize+1);
		
	writeKnubPreset(eepromAddr1, presetIndx*presetSize+1, &preset3);


	delay(1000);
		
		presetIndx = 5;
		Serial.println("ready");
		
		
		for(int i = 0; i<2; i++){
			presetIndx = 5;
			Serial.println("reading preset1 name: ");

			readKnubPresetName(eepromAddr1, presetIndx*presetSize+1, &currentPreset);

			Serial.println(currentPreset.name);

			delay(500);

			presetIndx = 6;
			Serial.println("reading preset2 name: ");

			readKnubPresetName(eepromAddr1, presetIndx*presetSize+1, &currentPreset);

			Serial.println(currentPreset.name);

			delay(500);
			presetIndx = 7;
			Serial.println("reading preset3 name: ");

			readKnubPresetName(eepromAddr1, presetIndx*presetSize+1, &currentPreset);

			Serial.println(currentPreset.name);
	}
}


 void loop(){

 	//midiInRead();


}


