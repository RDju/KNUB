
/*
FOR TESTING USE SOFTSERIAL BUT FINAL VERSION USES MAIN SERIAL OUT
*/
#include <Arduino.h>

#define CC2Looper 176

uint8_t loopsOut[4];

SoftwareSerial looperSerial(11, 4);

uint8_t switchLoops[4] = {80, 81, 82, 83};
uint8_t loopStates[2] = {0, 127};

byte switchSysEx[8] = {240, 0, 0, 7, 1, 8, 16, 247};

void sendSwitchSysEx(){


	for(uint8_t i = 0; i<8;i++){
		
		looperSerial.write(switchSysEx[i]);
	}
}

void switchLoop( uint8_t loopNum, uint8_t state){

	looperSerial.write(CC2Looper);
	looperSerial.write(switchLoops[loopNum]);
	looperSerial.write(loopStates[state]);
}

void clearLoopsOut(){

	for(uint8_t i = 0; i<4; i++){

		loopsOut[i] = 0;
	}
}

void fillLoopsOut(uint8_t indx, uint8_t val){

    loopsOut[indx] += val;
}

bool checkLoopsOut(uint8_t indx ){

  if(loopsOut[indx] != 0){

      return true;
  }else if(loopsOut[indx] == 0){

      return false;
  }
}

void updateLoops(uint8_t indx, uint8_t val){

	
		if(val == 0){
				if(loopsOut[indx] >0){
					loopsOut[indx] -= 1;
			}
		}else{
			if(loopsOut[indx]<3){
				loopsOut[indx] += 1;
			}
		}
	
}

