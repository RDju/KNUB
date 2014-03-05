#include <SoftwareSerial.h>
#include "utility/twi.h"
#include "Wire.h"
#include "presets.h"
#include "knubUtils.h"
#include "knubMidi.h"


byte presetIndx  = 5;

void setup(){
	

	Serial.begin(9600);


	Wire.begin();
	delay(500);
	
	//eraseEE();
	writeAllPresets();
	readAllPresets();
	

	//Serial.println(readByte(eepromAddr1, 126));
}


void loop(){
}

void eraseEE(){

	for(int i = 0; i < 10000; i ++){
	
	//Serial.print("writing at address : ");
	//Serial.println(i*presetSize);
	
	writeByte(eepromAddr1, i, 0xFF);
	
	delay(5);
	
	}

	Serial.println("ready");
}

void writeAllPresets(){

	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize);
	writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset1);
	presetIndx += 1;
	delay(500);

	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize);
	writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset2);
	presetIndx += 1;
	delay(500);
	
	Serial.print("writing at address : ");
	Serial.println(presetIndx*presetSize);
	writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset3);
	presetIndx += 1;
	delay(500);

	// Serial.print("writing at address : ");
	// Serial.println(presetIndx*presetSize);
	// writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset4);
	// presetIndx ++;
	// delay(500);


	// Serial.print("writing at address : ");
	// Serial.println(presetIndx*presetSize);
	// writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset5);
	// presetIndx ++;
	// delay(500);

	// Serial.print("writing at address : ");
	// Serial.println(presetIndx*presetSize);
	// writeKnubPreset(eepromAddr1, presetIndx*presetSize, &preset6);
	// presetIndx ++;
	// delay(500);

	
			
	Serial.println("DONE WRITING");	
}

void readAllPresets(){

	presetIndx = 5;

	//1
	readKnubPreset(eepromAddr1, presetIndx*presetSize, &currentPreset);
	delay(500);
	printCurrentPreset();
	delay(100);
	presetIndx ++;

	//2
	readKnubPreset(eepromAddr1, presetIndx*presetSize, &currentPreset);
	delay(500);
	printCurrentPreset();
	delay(100);
	presetIndx ++;
	//3
	readKnubPreset(eepromAddr1, presetIndx*presetSize, &currentPreset);
	delay(500);
	printCurrentPreset();
	delay(100);
	presetIndx ++;
	// //4
	// readKnubPreset(eepromAddr1, presetIndx*presetSize, &currentPreset);
	// delay(500);
		
	// printCurrentPreset();
	// delay(100);
	// presetIndx ++;
	// //5
	// readKnubPreset(eepromAddr1, presetIndx*presetSize, &currentPreset);
	// delay(500);
		
	// printCurrentPreset();
	// delay(100);
	// presetIndx ++;
	// //6
	// readKnubPreset(eepromAddr1, presetIndx*presetSize, &currentPreset);
	// delay(500);
		
	// printCurrentPreset();
	// delay(100);
	// presetIndx ++;
		
	Serial.println("DONE READING");	
}



