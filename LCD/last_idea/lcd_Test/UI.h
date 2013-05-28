/*
1. Clean the Screen
CMD : sc  Parameter : Null;
Example : sc; [ Screen clean ]
 
2. Set backlight
CMD : sb  Parameter :1 or 0 [1: Set the backlight on; 0: Set the backlight off ]
Example :
sb1; [ Backlight on ]
sb0; [ Backlight off ]
 
3. Set coordinate
CMD : sd  Parameter :C,R [C: the column (C = [0:1]); R: the row (R = [0:15])]
Example :
sd0,0;  [ move the coordinate to column 0 and row 0 ]
sd1,15;  [ move the coordinate to column 1 and row 15 ]
 
4. Send string
CMD : ss  Parameter : Char String
Example:
ss12345;  [ Send the char string and display on LCD ]
 
5. Set the cursor on/off
CMD su Parameter: 0/1 [0: Set the cursor off, 1: Set the cursor on]
Example:
su0; [ Cursor off ]
su1; [ Cursor on ]
 
6. Set the cursor type
CMD sf, Parameter: 0/1 [0: Set the cursor blink off, 1: Set the cursor blink on]
Example:
sf0; [ Set the cursor blink off ]
sf1; [ Set the cursor blink on ]
 
7. Move the cursor
CMD sm  Parameter: 0/1 [0: Move the cursor left , 1: Move the cursor right ]
Example:
sm0; [ shift left ]
sm1; [ shift right ]

*/


#define shortDel 2
#define longDel 5
#define txPin 6
#define rxPin 5

#include "Arduino.h"

SoftwareSerial lcd(rxPin, txPin);

int fxIndx = 0;
int memTabIndx = 0;
uint8_t numTabs[] = {0, 0, 0, 2, 7};
char tmpStr[4];

char* fxTabs[] = {"0,7", "1,7"};
char* paramTabs[] = {"0,0", "0,8", "0,12", "0,15", "1,3", "1,6", "1,10"};
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

/*
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
*/
void updatePreset(char preset[], char pName[], boolean editMode){


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
  
  if(editMode){
    lcd.write("ss");
    lcd.write(pName);
    lcd.write("{O_O}");
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

  for(int i=0; i<numTabs[4]; i++){
    
      lcd.write("sd");
      lcd.write(paramTabs[i]);
      lcd.write(";");
      delay(longDel);
  
  }
}
  
  

void customCursor(uint8_t cusTab, uint8_t pageLev){
  
  
  switch(pageLev){
  
    case 4:
    switch(cusTab){
  
      case 0:
      
        clearAllTabs();
        lcd.write("sd");
        lcd.write("0,6");
        lcd.write(";");
        delay(longDel);
        lcd.write("ss>;");
        delay(longDel);
        
        
      break;
      case 1:
        clearAllTabs();
        lcd.write("sd");
        lcd.write("1,0");
        lcd.write(";");
        delay(longDel);
        lcd.write("ss>;");
        delay(longDel);
      break;
      case 2:
         clearAllTabs();
         lcd.write("sd");
        lcd.write("1,5");
        lcd.write(";");
        delay(longDel);
        lcd.write("ss>;");
        delay(longDel);
      break;
      case 3:
        clearAllTabs();
        lcd.write("sd");
        lcd.write("1,10");
        lcd.write(";");
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

char* boolToString(boolean bval){

  if(bval == true){
    
    return "ON ";
  }else{
  
    return "OFF";
  
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
/*
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
/*
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
 lcd.write("ssParam:;");
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
*/



void savePage(char dummy[], char dummy1[], char dummy2[], char dummy3[], char dummy4[], char dummy5[], char dummy6[], char dummy7[], char dummy8[]){

  lcd.write("sd0,0;");
  delay(longDel);
  lcd.write("ssSAVING...;");
  delay(longDel);
}


void presetPage(char presetNum[], char dummy1[], char dummy2[], char dummy3[], char dummy4[], char dummy5[], char dummy6[], char dummy7[], char dummy8[]){

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

void knubbiePage(char knubbieName[], char knubbieV1[], char knubbieV2[], char knubbieCrv[], char knubbieMod[], char knubbieState[], char knubbieLoop[], char knubbieSwitch[], char dummy[8]){
  
  //name
  lcd.write("sd0,0;");
  delay(longDel);
  lcd.write("ss");
  lcd.write(knubbieName);
  lcd.write(";");
  delay(longDel);
  
  //mod
  lcd.write("sd0,8;");
  delay(longDel);
  lcd.write("ss");
  lcd.write(knubbieMod);
  lcd.write(";");
  delay(longDel);


  //state
  lcd.write("sd0,12");
  delay(longDel);
  lcd.write("ss");
  lcd.write(knubbieState);
  delay(longDel);
  
  //loop
  lcd.write("sd0,15;");
  delay(longDel);
  lcd.write("ss");
  lcd.write(knubbieLoop);
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
 
  lcd.write("sd1,10;");
  delay(longDel);
  lcd.write("ss");
  lcd.write(knubbieCrv);
  lcd.write(";");
  delay(longDel); 
}

void (*drawFuncs[6])(char dummy[], char dummy1[], char dummy2[], char dummy3[], char dummy4[], char dummy5[], char dummy6[], char dummy7[], char dummy8[]) = {productPage, softwareVersion, presetPage, knubbiePage, knubbiePage, savePage};



