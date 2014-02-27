#include <SoftwareSerial.h>
#include "utility/twi.h"
#include "Wire.h"



#include "presets.h"
#include "knubUtils.h"
#include "knubMidi.h"


 unsigned long time;

byte presetIndx  = 11;
byte indexes[3] = {11, 12, 13};
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
		
		presetIndx = 11;
		Serial.println("ready");
	
	readKnubPreset(eepromAddr1, presetIndx*presetSize+1, &currentPreset);
	delay(200);
			//Serial.println(currentPreset.name);
			printCurrentPreset();
		
		
}


 void loop(){

 	
			
}


