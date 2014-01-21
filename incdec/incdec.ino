
#include "ClickButton.h"
// incdec.ino


#define incPin 6
#define decPin 5

bool currUP, currDOWN, prevUP, prevDOWN;

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {

	Serial.begin(9600);


	

	pinMode(incPin, INPUT);
	pinMode(decPin, INPUT);

	digitalWrite(incPin, HIGH);
	digitalWrite(decPin, HIGH);

	prevUP = digitalRead(incPin);
	prevDOWN = digitalRead(decPin); 

}

void loop() {


	currUP = digitalRead(incPin);
	currDOWN = digitalRead(decPin);

	if(currUP != prevUP){
		delay(50);
		Serial.println("UP");	
		prevUP = currUP;
	}
	if(currDOWN != prevDOWN){
		delay(50);
		Serial.println("DOWN");	
		prevDOWN = currDOWN;
	}
	
  
  







}

