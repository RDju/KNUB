

#define shortDel 2
#define longDel 5


#include "Arduino.h"

LiquidCrystal_I2C lcd(0x20, 16, 2);

bool time2ChangePage = false;

int fxIndx = 0;

int memTabIndx = 0;

uint8_t numTabs[] = {0, 0, 2, 7, 0};

char tmpStr[4];

//char* fxTabs[] = {"7, 0", "7, 1"};
//char* paramTabs[] = {"0,0", "2,0", "13,0", "13,1", "2,1", "6,1", "10,1", "10,0"};
//char* chParamTabs[] = {"1,1", "5,1", "9,1", "12,1", "12,0", "9,0"};
//char* effectTabs[] = {"7,0", "7,1"};
//char* customCursorTabs[] = {"0,1", "5,1", "10,1"};


byte fxTabs[] = {B01110000, B01110001};
byte paramTabs[] = {B00000000, B00100000, B11010000, B11010001, B00100001, B01100001, B10100001, B10100000};
byte customCursorTabs[] = {B00000001, B01010001, B10100001};
byte chParamTabs[] = {B00010001, B01010001, B10010001, B11000001, B11000000, B10010000};



uint8_t customDigits[256] = {

0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 12, 12, 12, 
13, 13, 14, 14, 14, 15, 15, 16, 16, 16, 17, 17, 18, 18, 18, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 23, 23, 23, 
24, 24, 25, 25, 25, 26, 26, 27, 27, 27, 28, 28, 29, 29, 29, 30, 30, 30, 31, 31, 32, 32, 32, 33, 33, 34, 34, 34, 
35, 35, 36, 36, 36, 37, 37, 38, 38, 38, 39, 39, 40, 40, 40, 41, 41, 41, 42, 42, 43, 43, 43, 44, 44, 45, 45, 45, 
46, 46, 47, 47, 47, 48, 48, 49, 49, 49, 50, 50, 50, 51, 51, 52, 52, 52, 53, 53, 54, 54, 54, 55, 55, 56, 56, 56, 
57, 57, 58, 58, 58, 59, 59, 60, 60, 60, 61, 61, 61, 62, 62, 63, 63, 63, 64, 64, 65, 65, 65, 66, 66, 67, 67, 67, 
68, 68, 69, 69, 69, 70, 70, 70, 71, 71, 72, 72, 72, 73, 73, 74, 74, 74, 75, 75, 76, 76, 76, 77, 77, 78, 78, 78, 
79, 79, 80, 80, 80, 81, 81, 81, 82, 82, 83, 83, 83, 84, 84, 85, 85, 85, 86, 86, 87, 87, 87, 88, 88, 89, 89, 89, 
90, 90, 90, 91, 91, 92, 92, 92, 93, 93, 94, 94, 94, 95, 95, 96, 96, 96, 97, 97, 98, 98, 98, 99, 99, 100
};

char* toString(uint8_t val){

  itoa(val, tmpStr, 10);
  return tmpStr;
}

char* customCurveDigits[1] = {"00"};

void tab(byte wichTab){
  
  lcd.setCursor(wichTab>>4, wichTab&B00001111);
}

void clearScreen(){

    lcd.clear();
}

void updateParam(uint8_t prmIndx, char newVal[]){
  
    tab(paramTabs[prmIndx]);
    lcd.print(newVal);
}


void updateNumParam(uint8_t prmIndx, uint8_t newVal){
  

  
  tab(paramTabs[prmIndx]);

  if(newVal < 10){

    lcd.print("00");
    lcd.print(toString(newVal));
  }else if(newVal < 100){

    lcd.print("0");
    lcd.print(toString(newVal));

  }else{

    lcd.print(toString(newVal));

  }

  lcd.print(" ");
}

void updateParam2(uint8_t prmIndx, char newVal[]){
  
  tab(chParamTabs[prmIndx]);
  lcd.print(newVal);
}

/*
void updatePreset(char preset[], char pName[], boolean editMode){

  if(editMode){
    lcd.write("ss");
    lcd.write(pName);
    lcd.write("%");
    lcd.write(";");
    delay(longDel);
  }else{
    lcd.write("ss");
    lcd.write(pName);
    lcd.write(";");
    delay(longDel);
  
  }
}

void clearAllTabs(){

  for(int i=0; i<6; i++){
    
      lcd.write("sd");
      lcd.write(chParamTabs[i]);
      lcd.write(";");
      delay(longDel);
      lcd.write("ss ;");
      delay(longDel);
  
  }
}
  
  

void customCursor(uint8_t cusTab, uint8_t pageLev){
  
  
  switch(pageLev){
  
    case 3:
    switch(cusTab){
  
      case 0:
      
        clearAllTabs();

      break;
      case 1:
        clearAllTabs();
        lcd.write("sd");
        lcd.write(chParamTabs[0]);
        lcd.write(";");
        delay(longDel);
        lcd.write("ss>;");
        delay(longDel);
      break;
      case 2:
         clearAllTabs();
         lcd.write("sd");
        lcd.write(chParamTabs[1]);
        lcd.write(";");
        delay(longDel);
        lcd.write("ss>;");
        delay(longDel);
      break;
      case 3:
        clearAllTabs();
        lcd.write("sd");
        lcd.write(chParamTabs[2]);
        lcd.write(";");
        delay(longDel);
        lcd.write("ss>;");
        delay(longDel);
     break;
     case 4:
        clearAllTabs();
        lcd.write("sd");
        lcd.write(chParamTabs[3]);
        lcd.write(";");
        delay(longDel);
        lcd.write("ss>;");
        delay(longDel);
     break;
     case 5:
        clearAllTabs();
        lcd.write("sd");
        lcd.write(chParamTabs[4]);
        lcd.write(";");
        delay(longDel);
        lcd.write("ss>;");
        delay(longDel);
     break;
     case 6:
        clearAllTabs();
        lcd.write("sd");
        lcd.write(chParamTabs[5]);
        lcd.write(";");
        delay(longDel);
        lcd.write("ss>;");
        delay(longDel);
     break;
     
    }
    break;
    case 4:
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



char* stateToString(uint8_t state){

  if(state == 1){
    
    return "|x|";
  }else{
  
    return "| |";
  }
}

///////////////////////////////////////////////////////


///////////// PAGES ///////////////////////////////////

void productPage(char dummy[], char dummy1[], char dummy2[], char dummy3[], char dummy4[], char dummy5[], char dummy6[], char dummy7[], char dummy8[]){
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

void softwareVersion(char dummy[], char dummy1[], char dummy2[], char dummy3[], char dummy4[], char dummy5[], char dummy6[], char dummy7[], char dummy8[]){

 lcd.write("sc;");
 delay(10);
 lcd.write("sd0,0;");
 delay(longDel);
 lcd.write("ss");
 lcd.print(" Firmware: v0.1b");
 lcd.write(";");
 delay(longDel);
}

void savePage(char dummy[], char dummy1[], char dummy2[], char dummy3[], char dummy4[], char dummy5[], char dummy6[], char dummy7[], char dummy8[]){

  lcd.write("sd0,0;");
  delay(longDel);
  lcd.write("ssSAVING...;");
  delay(longDel);
}


void presetPage(char presetName[], char dummy1[], char dummy2[], char dummy3[], char dummy4[], char dummy5[], char dummy6[], char dummy7[], char dummy8[]){

 lcd.write("sc;");
 delay(longDel);
 lcd.write("sd0,0;");
 delay(longDel);
 lcd.write("ssPRESET:;");
 delay(longDel);
 lcd.write("ss");
 lcd.write(presetName);
 lcd.write(";");
 delay(longDel);
}

void knubbiePage(char knubbieName[], char knubbieV1[], char knubbieV2[], char knubbieCrv[], char knubbieMod[], char knubbieState[], char knubbieLoop[], char knubbieSwitch[], char knubNum[]){
  
  //name
  lcd.write("sd0,0;");
  delay(longDel);
  lcd.write("ss");
  lcd.write(knubNum);
  lcd.write(";");
  delay(longDel);
  
  //mod
  lcd.write("sd0,2;");
  delay(longDel);
  lcd.write("ss");
  lcd.write(knubbieName);
  lcd.write(";");
  delay(longDel);


  //state
  lcd.write("sd0,10");
  delay(longDel);
  lcd.write("ss");
  lcd.write(knubbieMod);
  delay(longDel);
  
  //loop
  lcd.write("sd0,15;");
  delay(longDel);
  lcd.write("ss");
  lcd.write(knubbieState);
  lcd.write(";");
  delay(longDel);  

  //val1
  lcd.write("sd1,2;");
  delay(longDel);
  lcd.write("ss");
  lcd.write(knubbieV1);
  lcd.write(";");
  delay(longDel); 
 
 //val2
  lcd.write("sd1,6;");
  delay(longDel);
  lcd.write("ss");
  lcd.write(knubbieV2);
  lcd.write(";");
  delay(longDel);   

 //curve
 
  lcd.write("sd1,13;");
  delay(longDel);
  lcd.write("ss");
  lcd.write(knubbieCrv);
  lcd.write(";");
  delay(longDel); 
}

void (*drawFuncs[6])(char dummy[], char dummy1[], char dummy2[], char dummy3[], char dummy4[], char dummy5[], char dummy6[], char dummy7[], char dummy8[]) = {productPage, softwareVersion, presetPage, knubbiePage, knubbiePage, savePage};



*/