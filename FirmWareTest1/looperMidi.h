
/*
FOR TESTING USE SOFTSERIAL BUT FINAL VERSION USES MAIN SERIAL OUT
*/


#define CC2Looper 176

byte loopsOut[4];

SoftwareSerial looperSerial(11, 4);

byte switchLoops[4] = {80, 81, 82, 83};
byte loopStates[2] = {0, 127};

void switchLoop( byte loopNum, byte state){

	looperSerial.write(CC2Looper);
	looperSerial.write(switchLoops[loopNum]);
	looperSerial.write(loopStates[state]);
}

void clearLoopsOut(){

	for(uint8_t i = 0; i<4; i++){

		loopsOut[i] = 0;
	}
}

void fillLoopsOut(byte indx, byte val){

    loopsOut[indx] += val;
}

bool checkLoopsOut(byte indx ){

  if(loopsOut[indx] != 0){

      return true;
  }else if(loopsOut[indx] == 0){

      return false;
  }
}

void updateLoops(byte indx, byte val){

	
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

