//DFRobot.com
//Compatible with the Arduino IDE 1.0
//Library version:1.1


#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);  // set the LCD address to 0x20(Cooperate with 3 short circuit caps) for a 16 chars and 2 line display

void setup(){
  
                      // initialize the lcd 
  //lcd.backlight();
  
  //lcd.home();
  
  lcd.setCursor(0, 0);
  lcd.print("Hello world...");
  lcd.setCursor(0, 1);
  lcd.print("dfrobot.com");

}


  
void loop(){
  
 
}
