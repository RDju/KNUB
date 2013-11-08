
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

//startUp sequence
  (*drawFuncs[0])("", "", "", "", "", "", "", "", "");
  delay(500);
  (*drawFuncs[1])("", "", "", "", "", "", "", "", "");
  delay(500);



}

