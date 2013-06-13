#include <Wire.h>


#include "knubFuncs.h"

int knubVal = 0;

void setup(){
  Wire.begin();
  Serial.begin(9600);

}


void loop(){

  for(int i = 0; i<256; i++){
  
    turnKnub(0, i);
    delay(50);
  
  }
  for(int i = 255; i > -1; i--){
  
    turnKnub(0, i);
    delay(50);
  }



}




