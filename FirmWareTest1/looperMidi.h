

#define CC2Looper 176

byte switchLoops[4] = {80, 81, 82, 83};
byte loopStates[2] = {0, 127};

void switchLoop( byte loopNum, byte state){

	Serial.write(CC2Looper);
	Serial.write(switchLoops[loopNum]);
	Serial.write(loopStates[state]);
}

