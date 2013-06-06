#include "luts.h"

byte knubVal = 100;
int mappedKnub;


void setup(){

  Serial.begin(9600);



}



void loop(){

  turnKnub(knubVal);


}





void turnKnub(byte val){

  mappedKnub = map(val, 0, 255, vacMin, vacMax);
  Serial.println(mappedKnub);


}
