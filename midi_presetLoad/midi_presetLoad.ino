#include <SoftwareSerial.h>
#include "utility/twi.h"
#include "Wire.h"



#include "presets.h"
#include "knubUtils.h"
#include "knubMidi.h"


 unsigned long time;

byte presetIndx  = 5;
byte indexes[6] = {5, 6, 7, 8, 9, 10};

int choose;
int dTime;
void setup(){
	

	Serial.begin(9600);
	midiSerial.begin(31250);

	Wire.begin();
	delay(500);
	
	

	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize);
	writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset1);
	presetIndx ++;
	delay(500);

	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize);
	writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset2);
	presetIndx ++;
	delay(500);
	
	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize);
	writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset3);
	presetIndx ++;
	delay(500);

	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize);
	writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset4);
	presetIndx ++;
	delay(500);


	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize);
	writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset5);
	presetIndx ++;
	delay(500);

	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize);
	writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset6);
	presetIndx ++;
	delay(500);

	delay(1000);
		
	presetIndx = 5;
	Serial.println("ready");
	
	
	for(int i = 5; i < 11; i++){
		readKnubPreset(eepromAddr1, i*presetSize, &currentPreset);
		delay(200);
		Serial.println(currentPreset.name);
		delay(200);
	}
		
	Serial.println("DONE");	
}


 void loop(){
}

void eraseEE(){

	for(int i = 5; i < 205; i ++){


	Serial.print("writing at address : ");
	Serial.println(i*presetSize);
	
	writeKnubPreset(eepromAddr1, i*presetSize, &emptyPreset);
	
	delay(100);
	
	}

	Serial.println("ready");
}


