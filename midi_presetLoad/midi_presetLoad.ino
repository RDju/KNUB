#include "SoftwareSerial.h"
#include "utility/twi.h"
#include "Wire.h"
#include "memory.h"
#include "presets.h"
#include "knubUtils.h"
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


 void loop(){

 	//midiInRead();


}


