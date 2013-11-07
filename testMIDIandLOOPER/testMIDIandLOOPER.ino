
#include "SoftwareSerial.h"
#include "looperMidi.h"
#include "knubMidi.h"

void setup() {

	Serial.begin(9600);
	looperSerial.begin(31250);
	midiSerial.begin(31250);
	
	//midiSerial.listen();
}

void loop() {


	midiInRead(2);
	/*
	if(midiSerial.isListening()){

		Serial.println("midiIN");


	}
	if(looperSerial.isListening()){

		Serial.println("looperOUT");

	}
	*/

}


