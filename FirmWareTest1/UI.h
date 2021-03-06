#include "Arduino.h"

LiquidCrystal_I2C lcd(0x20, 16, 2);

bool time2ChangePage = false;

byte paramTabs[8] = {B00000000, B00100000, B11010000, B11010001, B00100001, B01100001, B10100001, B10100000};// localisation des paramètres sur l'écran
byte chParamTabs[6] = {B00010001, B01010001, B10010001, B11000001, B11000000, B10010000};//change param tab (localisation du curseur de sélection)
byte ledPositions[4] = {B00010001, B01010001, B10010001, B11010001}; // indication sur le LCD des pétales éteintes et allumées

//Customizes characters to indicate if pedals are ON or OFF
uint8_t ledOFF[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111
};

uint8_t ledON[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

uint8_t stateOFF[8] = {
  0b00000,
  0b00000,
  0b11111,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b00000
};

uint8_t stateON[8] = {
  0b00000,
  0b00000,
  0b11111,
  0b11011,
  0b10101,
  0b11011,
  0b11111,
  0b00000
};

//switch led on or off
void switchUILed(uint8_t wichLed, uint8_t status){
  lcd.setCursor(ledPositions[wichLed]>>4, ledPositions[wichLed]&B00001111);
  lcd.write(status);
  lcd.write(status);
}

inline char* toString(uint8_t val){
  char tmpStr[4];
  itoa(val, tmpStr, 10);
  return tmpStr;
}

//char* customCurveDigits[1] = {"00"};

inline void tab(byte wichTab){
  lcd.setCursor(wichTab>>4, wichTab&B00001111);
}

inline void clearScreen(){
  lcd.clear();
}

void updateParam(uint8_t prmIndx, char newVal[]){
    tab(paramTabs[prmIndx]);
    lcd.print(newVal);
}

void updateState(uint8_t state){
  tab(paramTabs[2]);
  if (state == 1)
         lcd.write(4);
  else lcd.write(3);
}

void updateNumParam(uint8_t prmIndx, uint8_t newVal){
  tab(paramTabs[prmIndx]);
  if (newVal < 10)
    lcd.print("00");
  else if (newVal < 100)
    lcd.print("0");
  lcd.print(toString(newVal));
  lcd.print(" ");
}

inline void updateParam2(uint8_t prmIndx, char newVal[]){
  tab(chParamTabs[prmIndx]);
  lcd.print(newVal);
}

//TODO: editMode not needed now
inline void updatePreset(/*char preset[],*/ char pName[], boolean editMode){
  lcd.print(pName);
  if(editMode)
    lcd.print("%");
}

//TODO: try to clear the previous cursor via another way
inline void clearTabs(byte wichTab, byte prevIndx){
  if (prevIndx!=0){
    tab(chParamTabs[prevIndx-1]);
    lcd.print(" ");
  }
}

void customCursor(uint8_t cusTab, uint8_t pageLev, byte prevIndx){
  switch(pageLev){
  case 3:
    clearTabs(cusTab, prevIndx);
    if (cusTab!=0){
      tab(chParamTabs[cusTab-1]);
      lcd.print(">");
    }
    break;
  case 4:
    lcd.setCursor(6,cusTab);
    lcd.print(">");
    lcd.setCursor(6, abs(cusTab-1)); // 0 if cusTab = 1, 1 if cusTab = 0
    lcd.print(":");
    break;
  }
}

//TODO: redo the drawing
char* stateToString(uint8_t state){
  if(state == 1)
    return "|x|";
  else
    return "| |";
}

///////////////////////////////////////////////////////


///////////// PAGES ///////////////////////////////////

//pageLevel == 0
void productPage(){

  //char** productPageString = getProductPageString();

  clearScreen();
  lcd.setCursor(0,0);
  //lcd.print(productPageString[0]);
  lcd.print("    LE KNUB   ");
  lcd.setCursor(0,1);
  //lcd.print(productPageString[1]);
  lcd.print(" by Combosquare ");
  delay(1000);
}

void softwareVersion(){

  //char* softwareVersionString = getSoftwareVersionString();

  clearScreen();
  lcd.setCursor(0,0);
  //lcd.print(softwareVersionString);
  lcd.print(" Firmware: v0.1a");
  delay(1000);
}

void wantToSave(){
  clearScreen();
  lcd.setCursor(0,0);
  lcd.print("     SAVE ?     ");
  lcd.setCursor(0,1);
  lcd.print("L: No  / R: Yes ");
}

void savePage(){
  clearScreen();
  lcd.setCursor(0,0);
  lcd.print("SAVING...");
  delay(1000);
}

//pageLevel == 1
void presetPage(){
  clearScreen();
  //lcd.print(presetName); 
}

//pageLevel == 3 && 4
void knubbiePage(int currentParam, aKnubPreset currentPreset, char* modOns[], char* switchOut[]){
  
  clearScreen();
  
  if (currentParam == 0){
      updatePreset(currentPreset.name , isPresetEdited(&currentPreset) );
  } else {
  
      updateParam(0, toString(currentParam + 1));
      updateParam(1,currentPreset.knubbies[currentParam].name);
      //updateState(currentPreset.knubbies[currentParam].state);
      //updateParam(2,stateToString(currentPreset.knubbies[currentParam].params[4]));//TODO: change the visual indicator
      updateParam(3,modOns[currentPreset.knubbies[currentParam].params[3]]);
      updateNumParam(4,map(currentPreset.knubbies[currentParam].params[0], 0, 254, 0, 100));
      updateNumParam(5,map(currentPreset.knubbies[currentParam].params[1], 0, 254, 0, 100));
      //updateParam(6,customCurveDigits[currentPreset.knubbies[currentParam].params[2]]);    
      //updateParam(7, switchOut[currentPreset.knubbies[currentParam].params[5]]);
  }

}

void pagePref(){
  clearScreen();
  lcd.setCursor(0,0);
  lcd.print("PREFS");
}

/*void (*drawFuncs[6])(char dummy[], char dummy1[], char dummy2[], char dummy3[], char dummy4[], char dummy5[], char dummy6[], char dummy7[], char dummy8[]) = {
  productPage, softwareVersion, presetPage, knubbiePage, knubbiePage, savePage};*/

