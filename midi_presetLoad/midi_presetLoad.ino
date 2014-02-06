#include <SoftwareSerial.h>
#include "utility/twi.h"
#include "Wire.h"

#include "presets.h"
#include "knubUtils.h"
#include "knubMidi.h"


 unsigned long time;

byte presetIndx  = 5;
byte indexes[3] = {5, 6, 7};
int choose;
int dTime;
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
		
		
		
}


 void loop(){

 	
			dTime = random(3000)+50;
			choose = random(3);

			presetIndx = indexes[choose];
			
			Serial.println("reading preset1 name: ");
			Serial.println(dTime);

			readKnubPresetName(eepromAddr1, presetIndx*presetSize+1, &currentPreset);

			Serial.println(currentPreset.name);

			delay(dTime);
}


