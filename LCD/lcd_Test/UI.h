#define txPin 6
#define rxPin 5

#include "Arduino.h"

SoftwareSerial lcd(rxPin, txPin);

int fxIndx = 0;
int memTabIndx = 0;

char* effectsName[] = {"RAT", "HOLY", "WAH", "DELAY"};

char* fxTabs[] = {"0,7", "1,7"};
char* paramTabs[] = {"0,7", "1,1", "1,6", "1,11"};
char* customCursorTabs[] = {"1,0", "1,5", "1,10"};

/////////////// UTILS ////////////////
void initDisplay(){
  
 lcd.write("su0;");
 delay(5);
 lcd.write("sf0;");
 delay(5);

}

void initMemDisp(){
  
 lcd.write("su0;");
 delay(5);
 lcd.write("sf0;");
 delay(5);

}


void tab(char wichTab[]){
  lcd.write("sd");
  lcd.write(wichTab);
  lcd.write(";");
  delay(5);
}


void clearScreen(){

  lcd.write("sc;");
  delay(5);

}


void updateParam(int prmIndx, char newVal[]){
  
  
  lcd.write("sd");
  lcd.write(paramTabs[prmIndx]);
  lcd.write(";");
  delay(5);
  lcd.write("ss");
  lcd.write(newVal);
  lcd.write(" ");
  lcd.write(" ");
  lcd.write(";");
  delay(5);

}


void customCursor(int cusTab){
  
  switch(cusTab){
  
    case 0:
      lcd.write("sd1,10;");
      delay(5);
      lcd.write("ss ;");
      delay(5);
      break;
    case 1:
      lcd.write("sd1,0;");
      delay(5);
      lcd.write("ss>;");
      delay(5);
    break;
    case 2:
      lcd.write("sd1,0;");
      delay(5);
      lcd.write("ss ;");
      delay(5);
      lcd.write("sd1,5;");
      delay(5);
      lcd.write("ss>;");
      delay(5);
    break;
    case 3:
      lcd.write("sd1,5;");
      delay(5);
      lcd.write("ss ;");
      delay(5);
      lcd.write("sd1,10;");
      delay(5);
      lcd.write("ss>;");
      delay(5);
     break;
  
  }
  
  
  
  
  
  

}


///////////////////////////////////////////////////////


///////////// PAGES ///////////////////////////////////

void productPage(){
 lcd.write("sc;");
 delay(10);
 lcd.write("sd0,0;");
 delay(5);
 lcd.write("ss");
 lcd.print("    LE KNUB   ");
 lcd.write(";");
 delay(5);
 lcd.write("sd1,0;");
 delay(5);
 lcd.write("ss");
 lcd.print(" by Combosquare ");
 lcd.write(";");
 delay(5);
}

void softwareVersion(){

 lcd.write("sc;");
 delay(10);
 lcd.write("sd0,0;");
 delay(5);
 lcd.write("ss");
 lcd.print(" Firmware: v0.1b");
 lcd.write(";");
 delay(5);
}

void presetPage(){

 lcd.write("sc;");
 delay(5);
 lcd.write("sd0,0;");
 delay(5);
 lcd.write("ss");
 lcd.print("PRESET:    ");
 lcd.write(";");
 delay(5);
 lcd.write("sd0,7;");
}

void effectPage(){

 lcd.write("sc;");
 delay(5);
 lcd.write("sd0,0;");
 delay(5);
 lcd.write("ssEffect:;");
 delay(5);
 lcd.write("sd1,0;");
 delay(5);
 lcd.write("ssActive:;");
 delay(5);
}

void paramPage(char paramName[], char val1[], char val2[], char curveType[]){
 
 lcd.write("sd0,0;");
 delay(5);
 lcd.write("ssParam :;");
 delay(5);
 
 lcd.write("sd");
 lcd.write(paramTabs[0]);
 lcd.write(";");
 delay(5);
 
 lcd.write("ss");
 lcd.write(paramName);
 lcd.write(";");
 delay(5);
 
 lcd.write("sd");
 lcd.write(paramTabs[1]);
 lcd.write(";");
 delay(5);
 
 lcd.write("ss");
 lcd.write(val1);
 lcd.write(";");
 delay(5);
 
 lcd.write("sd");
 lcd.write(paramTabs[2]);
 lcd.write(";");
 delay(5);
 
 lcd.write("ss");
 lcd.write(val2);
 lcd.write(";");
 delay(5);
 
 lcd.write("sd");
 lcd.write(paramTabs[3]);
 lcd.write(";");
 delay(5);
 
 lcd.write("ss");
 lcd.write(curveType);
 lcd.write(";");
 delay(5);

 lcd.write("sd");
 lcd.write(paramTabs[0]);
 lcd.write(";");
 delay(5);

}






