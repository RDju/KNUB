
#include "utility/twi.h"
#include "Wire.h"
#include "LiquidCrystal_I2C.h"


#include "UI.h"
// testI2CLCD.ino




void setup() {
Serial.begin(9600);

  lcd.init();
  lcd.backlight(); 

  clearScreen();
  






}

void loop() {

	for(int i =0; i<sizeof(paramTabs)/sizeof(paramTabs[0]); i++){

		clearScreen();
		tab(paramTabs[i]);
		lcd.print("X");
		delay(500);

	}



}

