
#include <SoftwareSerial.h>


//#define DEBUG_MIDI //uncomment this to activate midi debugging


#define looperCC 176
//RX, TX
SoftwareSerial looperSerial(11, 4);

//byte looperCC = 176;
byte switchLoops[4] = {80, 81, 82, 83};
byte turnOff = 0;
byte turnOn = 127;

void setup() {
	
	looperSerial.begin(31250);
}
/*
void switchLoop( byte loopNum, byte state){

	looperSerial.print(CC2Looper);
	looperSerial.print(switchLoops[loopNum]);
	looperSerial.print(state * 127);
}
*/


void loop() {


	looperSerial.write(looperCC);
	looperSerial.write(switchLoops[0]);
	looperSerial.write(turnOn);
	delay(500);
	looperSerial.write(looperCC);
	looperSerial.write(switchLoops[0]);
	looperSerial.write(turnOff);
	delay(500);

}



