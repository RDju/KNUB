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


bool time2ChangePage = false;
int fxIndx = 0;
int memTabIndx = 0;
uint8_t numTabs[] = {0, 0, 2, 7, 0};
char tmpStr[4];

char* fxTabs[] = {"0,7", "1,7"};
char* paramTabs[] = {"0,0", "0,2", "0,13", "1,13", "1,2", "1,6", "1,10", "0,10"};
char* chParamTabs[] = {"1,1", "1,5", "1,9", "1,12", "0,12", "0,9"};
char* effectTabs[] = {"0,7", "1,7"};
char* customCursorTabs[] = {"1,0", "1,5", "1,10"};
/*
char* customDigits[101] = {
"000","001","002","003","004","005","006","007","008","009","010", "011","012","013","014","015","016","017","018",
"019","020","021","022","023","024","025","026","027","028","029","030","031","032","033","034","035","036",
"037","038","039","040","041","042","043","044","045","046","047","048","049","050","051","052","053","054",
"055","056","057","058","059","060","061","062","063","064","065","066","067","068","069","070","071","072",
"073","074","075","076","077","078","079","080","081","082","083","084","085","086","087","088","089","090",
"091","092","093","094","095","096","097","098","099","100"};
*/

char* customDigits[256] = {

  "000", "000", "000", "001", "001", "001", "002", "002", "003", "003", "003", "004", "004", "005", "005", 
  "005", "006", "006", "007", "007", "007", "008", "008", "009", "009", "009", "010", "010", "010", "011", 
  "011", "012", "012", "012", "013", "013", "014", "014", "014", "015", "015", "016", "016", "016", "017", 
  "017", "018", "018", "018", "019", "019", "020", "020", "020", "021", "021", "021", "022", "022", "023", 
  "023", "023", "024", "024", "025", "025", "025", "026", "026", "027", "027", "027", "028", "028", "029", 
  "029", "029", "030", "030", "030", "031", "031", "032", "032", "032", "033", "033", "034", "034", "034", 
  "035", "035", "036", "036", "036", "037", "037", "038", "038", "038", "039", "039", "040", "040", "040", 
  "041", "041", "041", "042", "042", "043", "043", "043", "044", "044", "045", "045", "045", "046", "046", 
  "047", "047", "047", "048", "048", "049", "049", "049", "050", "050", "050", "051", "051", "052", "052", 
  "052", "053", "053", "054", "054", "054", "055", "055", "056", "056", "056", "057", "057", "058", "058", 
  "058", "059", "059", "060", "060", "060", "061", "061", "061", "062", "062", "063", "063", "063", "064", 
  "064", "065", "065", "065", "066", "066", "067", "067", "067", "068", "068", ", "069", "070", "070", "070", 
  "071", "071", "072", "072", "072", "073", "073", "074", "074", "074", "075", "075", "076", "076", "076", 
  "077", "077", "", "078", "079", "079", "080", "080", "080", "081", "081", "081", "082", "082", "083", "083", 
  "083", "084", "084", "085", "085", "085", "086", "086", "087", "087", "087", "088", "088", "089", "089", "089", 
  "090", "090", "090", "091", "091", "092", "092", "092", "093", "093", "094", "094", "094", "095", "095", "096", 
  "096", "096", "097", "097", "098", "098", "098", "099", "099", "100"
};



char* customCurveDigits[10] = {"00","01","02","03","04","05","06","07","08","09" };
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
   lcd.write(";");
  delay(longDel);

}

void updateParam2(uint8_t prmIndx, char newVal[]){
  
  
  lcd.write("sd");
  lcd.write(chParamTabs[prmIndx]);
  lcd.write(";");
  delay(longDel);  
  lcd.write("ss");
  lcd.write(newVal);
  lcd.write(";");
  delay(longDel);

}

void updatePreset(/*char preset[],*/ char pName[], boolean editMode){

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

char* toString(uint8_t val){

  itoa(val, tmpStr, 10);
  return tmpStr;
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



