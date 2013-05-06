

#define shortDel 2
#define longDel 5
#define txPin 6
#define rxPin 5

#include "Arduino.h"

SoftwareSerial lcd(rxPin, txPin);

int fxIndx = 0;
int memTabIndx = 0;

char tmpStr[4];

char* fxTabs[] = {"0,7", "1,7"};
char* paramTabs[] = {"0,7", "1,1", "1,6", "1,11"};
char* effectTabs[] = {"0,7", "1,7"};
char* customCursorTabs[] = {"1,0", "1,5", "1,10"};

/////////////// UTILS ////////////////
void initDisplay(){
  
 lcd.write("su0;");
 delay(longDel);
 lcd.write("sf0;");
 delay(longDel);

}

void initMemDisp(){
  
 lcd.write("su0;");
 delay(longDel);
 lcd.write("sf0;");
 delay(longDel);

}


void tab(char wichTab[]){
  lcd.write("sd");
  lcd.write(wichTab);
  lcd.write(";");
  delay(longDel);
}



void clearScreen(){

  lcd.write("sc;");
  delay(longDel);

}


void updateParam(uint8_t prmIndx, char newVal[]){
  
  
  lcd.write("sd");
  lcd.write(paramTabs[prmIndx]);
  lcd.write(";");
  delay(longDel);
  lcd.write("ss");
  lcd.write(newVal);
  lcd.write(" ");
  lcd.write(" ");
  lcd.write(";");
  delay(longDel);

}


void updatePedalName(char pName[]){

  lcd.write("sd0,7;");
  delay(longDel);
  
  delay(longDel);
  lcd.write("ss");
  lcd.write(pName);
  lcd.write(" ");
  lcd.write(" ");
  lcd.write(";");
  delay(longDel);
}

void updatePedalState(char state[]){

  lcd.write("sd1,7;");
  delay(longDel);
  lcd.write("ss");
  lcd.write(state);
  lcd.write(" ");
  lcd.write(";");
  delay(longDel);
}

void updatePreset(char preset[], char pName[]){


  lcd.write("sd0,7;");
  delay(longDel);
  lcd.write("ss");
  lcd.write(preset);
  lcd.write(" ");
  lcd.write(" ");
  lcd.write(";");
  delay(longDel);
  lcd.write("sd1,0;");
  delay(longDel);
  lcd.write("ss");
  lcd.write(pName);
  lcd.write(" ");
  lcd.write(" ");
  lcd.write(";");
  delay(longDel);

}

void customCursor(uint8_t cusTab, uint8_t pageLev){
  
  
  switch(pageLev){
  
    case 4:
    switch(cusTab){
  
      case 0:
      lcd.write("sd1,10;");
      delay(longDel);
      lcd.write("ss ;");
      delay(longDel);
        lcd.write("sd0,5;");
        delay(longDel);
        lcd.write("ss ;");
        delay(longDel);
        lcd.write("ss>;");
        delay(longDel);
        
        
      break;
      case 1:
        lcd.write("sd0,5;");
        delay(longDel);
        lcd.write("ss :;");
        delay(longDel);
        lcd.write("sd1,0;");
        delay(longDel);
        lcd.write("ss>;");
      delay(longDel);
      break;
      case 2:
        lcd.write("sd1,0;");
        delay(longDel);
        lcd.write("ss ;");
        delay(longDel);
        lcd.write("sd1,5;");
        delay(longDel);
        lcd.write("ss>;");
      delay(longDel);
      break;
      case 3:
        lcd.write("sd1,5;");
        delay(longDel);
        lcd.write("ss ;");
        delay(longDel);
        lcd.write("sd1,10;");
        delay(longDel);
        lcd.write("ss>;");
      delay(longDel);
     break;
    }
    break;
    case 3:
      switch(cusTab){
  
      case 0:
        lcd.write("sd0,6;");
        delay(longDel);
        lcd.write("ss>;");
        delay(longDel);
        lcd.write("sd1,6;");
        delay(longDel);
        lcd.write("ss:;");
      delay(longDel);
      break;
      case 1:
        lcd.write("sd1,6;");
        delay(longDel);
        lcd.write("ss>;");
        delay(longDel);
        lcd.write("sd0,6;");
        delay(longDel);
        lcd.write("ss:;");
      delay(longDel);
      break;
    }
    break;
  }
}

char* toString(uint8_t val){

  itoa(val, tmpStr, 10);
  return tmpStr;


}


///////////////////////////////////////////////////////


///////////// PAGES ///////////////////////////////////

void productPage(char dummy[], char dummy1[], char dummy2[], char dummy3[]){
 lcd.write("sc;");
 delay(10);
 lcd.write("sd0,0;");
 delay(longDel);
 lcd.write("ss");
 lcd.print("    LE KNUB   ");
 lcd.write(";");
 delay(longDel);
 lcd.write("sd1,0;");
 delay(longDel);
 lcd.write("ss");
 lcd.print(" by Combosquare ");
 lcd.write(";");
 delay(longDel);
}

void softwareVersion(char dummy[], char dummy1[], char dummy2[], char dummy3[]){

 lcd.write("sc;");
 delay(10);
 lcd.write("sd0,0;");
 delay(longDel);
 lcd.write("ss");
 lcd.print(" Firmware: v0.1b");
 lcd.write(";");
 delay(longDel);
}

void presetPage(char presetNum[], char dummy1[], char dummy2[], char dummy3[]){

 lcd.write("sc;");
 delay(longDel);
 lcd.write("sd0,0;");
 delay(longDel);
 lcd.write("ssPRESET:;");
 delay(longDel);
 lcd.write("ss");
 lcd.write(presetNum);
 lcd.write(";");
 delay(longDel);
}

void effectPage(char effectName[], char state[], char dummy2[], char dummy3[]){


 lcd.write("sd0,0;");
 delay(longDel);
 lcd.write("ssEffect:;");
 delay(longDel);
 lcd.write("ss");
 lcd.write(effectName);
 lcd.write(";");
 delay(longDel);
 lcd.write("sd1,0;");
 delay(longDel);
 lcd.write("ssActive:;");
 delay(longDel);
 lcd.write("ss");
 lcd.write(state);
 lcd.write(";");
 delay(longDel);
}

void paramPage(char paramName[], char val1[], char val2[], char curveType[]){
 
 lcd.write("sd0,0;");
 delay(longDel);
 lcd.write("ssParam :;");
 delay(longDel);
 
 lcd.write("sd");
 lcd.write(paramTabs[0]);
 lcd.write(";");
 delay(longDel);
 
 lcd.write("ss");
 lcd.write(paramName);
 lcd.write(";");
 delay(longDel);
 
 lcd.write("sd");
 lcd.write(paramTabs[1]);
 lcd.write(";");
 delay(longDel);
 
 lcd.write("ss");
 lcd.write(val1);
 lcd.write(";");
 delay(longDel);
 
 lcd.write("sd");
 lcd.write(paramTabs[2]);
 lcd.write(";");
 delay(longDel);
 
 lcd.write("ss");
 lcd.write(val2);
 lcd.write(";");
 delay(longDel);
 
 lcd.write("sd");
 lcd.write(paramTabs[3]);
 lcd.write(";");
 delay(longDel);
 
 lcd.write("ss");
 lcd.write(curveType);
 lcd.write(";");
 delay(longDel);

 lcd.write("sd");
 lcd.write(paramTabs[0]);
 lcd.write(";");
 delay(longDel);

}


void (*drawFuncs[5])(char dummy[], char dummy1[], char dummy2[], char dummy3[]) = {productPage, softwareVersion, presetPage, effectPage, paramPage};



