#include <Bounce.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#include "UI.h"
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

TX 2
RX 4

*/

#define encoderPin1 2
#define encoderPin2 3

#define validBut 9
#define backBut 8

volatile int lastValue = 0;
volatile int lastEncoderValue = 0;
volatile int encoderValue = 0;
volatile int encoderValueParam = 0;
volatile int encoderValueParamVal = 0;
volatile int scaledEncoderValueParam = 0;

boolean time2ChangePage;

uint8_t txtParamIndx;

uint8_t lastMSB = 0;
uint8_t lastLSB = 0;

char valBuf[4];
Bounce bValid = Bounce(validBut, 5);
Bounce bckValid = Bounce(backBut, 5);

int pageLevel = 0;
int tabIndx = 0;
uint8_t currFx = 0;
uint8_t numTabs[] = {0, 0, 0, 2, 4};
int8_t encoderDir;

char* effectsName[] = {"RAT"};
char* statesOfEffects[] = {"ON", "OFF"};
char* curves[] = {"L00", "L01", "L02"};
char* params[] = {"DIST", "TONE", "VOL "};
char*  prmVals[] = {"10", "50", "60"};

int8_t  paramVals[] = {10, 50};

int currentParam = 0;
int currentCurve = 0;

boolean prmChange;
 
void setup(){
  
  lcd.begin(9600);

  initDisplay();
   
  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);

  pinMode(validBut, INPUT);
  digitalWrite(validBut, HIGH);
  pinMode(backBut, INPUT);
  digitalWrite(backBut, HIGH);
  
  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);
  
  Serial.begin(9600);
 
  Serial.println(sizeof(params)/sizeof(char*));
  
    (*drawFuncs[0])("", "", "", "");
    delay(500);
    (*drawFuncs[1])("", "", "", "");
    delay(500);
    initMemDisp();
    clearScreen();
    //paramPage(params[currentParam], prmVals[0], prmVals[1], curves[currentCurve]);
    (*drawFuncs[2])("", "", "", "");
    pageLevel = 2;
}

void loop(){
  ////dealing with pages 
  if(time2ChangePage){
   
   switch(pageLevel){
   
     case 0:
     clearScreen();
       (*drawFuncs[pageLevel])("", "", "", "");
        time2ChangePage = false;
     break;
     case 1:  
     clearScreen();
        (*drawFuncs[pageLevel])("", "", "", "");
        time2ChangePage = false;
     break;
     case 2:
     clearScreen();
         (*drawFuncs[pageLevel])("", "", "", "");
          time2ChangePage = false;
     break;
     case 3:
     clearScreen();
         (*drawFuncs[pageLevel])(effectsName[currFx], statesOfEffects[currFx], "", "");
    time2ChangePage = false;
     break;
     case 4:
     clearScreen();
         (*drawFuncs[pageLevel])(params[currentParam], prmVals[0], prmVals[1], curves[currentCurve]);
    time2ChangePage = false;
     break;
   
   
   }
       
   
  
  }
  //////////////////////
  
  //// tab button
 bValid.update(); 
  
    if(bValid.read()== LOW && bValid.duration() >1000){
      
      
      switch (pageLevel){
      
        case 0:
          pageLevel ++;
          time2ChangePage = true;
          Serial.print("UP: ");
          Serial.println(pageLevel);
        break;
        case 1:
          pageLevel ++;
          time2ChangePage = true;
           Serial.print("UP: ");
          Serial.println(pageLevel);
        break;
        case 2:
          pageLevel ++;
          time2ChangePage = true;
           Serial.print("UP: ");
          Serial.println(pageLevel);
        break;
        case 3:
          pageLevel ++;
          time2ChangePage = true;
          /*
          
           */
       break;
        case 4:
        /*
        
        */
          break;
        }
       }else if(bValid.read()== LOW && bValid.duration() < 50){
       
         switch(pageLevel){
         
           case 3:
             tabIndx++;
             tabIndx = tabIndx%numTabs[pageLevel];
             tab(effectTabs[tabIndx]);
          
          
           break;
           case 4:
             tabIndx++;
             tabIndx = tabIndx%numTabs[pageLevel];
             tab(paramTabs[tabIndx]);
        
             customCursor(tabIndx);
             
           break;
          }
       }
      

  //////back button
  if(bckValid.update()){
     
    if(bckValid.read()==LOW){
      
      if(pageLevel > 0){
        pageLevel --;
          Serial.print("DOWN : ");
          Serial.println(pageLevel);
        time2ChangePage = true;
      }
     }
   }
}
  
  //////////////////////////////////////////////
  
  /////// encoding Wheel/////////////////////
/*
if(encoderValue != lastValue){
 
 switch(tabIndx){
 
   case 0:

     scaledEncoderValueParam = encoderValue%25;
     if(scaledEncoderValueParam == 0){
       txtParamIndx += encoderDir;
       updateParam(tabIndx, params[txtParamIndx%3]);
     }
     
   break;
   case 1:
     if(paramVals[tabIndx - 1]>0 && paramVals[tabIndx -1]<100){
       paramVals[tabIndx - 1] += encoderDir;
       itoa(paramVals[tabIndx - 1], valBuf, 10);
       updateParam(tabIndx, valBuf);
     }else if(paramVals[tabIndx - 1]== 0 && encoderDir ==1){
       paramVals[tabIndx - 1] += encoderDir;
       itoa(paramVals[tabIndx - 1], valBuf, 10);
       updateParam(tabIndx, valBuf);
       
     }else if(paramVals[tabIndx - 1]== 100 && encoderDir ==-1){
       paramVals[tabIndx - 1] += encoderDir;
       itoa(paramVals[tabIndx - 1], valBuf, 10);
       updateParam(tabIndx, valBuf);
     }
   break;
   case 2:
     if(paramVals[tabIndx - 1]>0 && paramVals[tabIndx -1]<100){
       paramVals[tabIndx - 1] += encoderDir;
       itoa(paramVals[tabIndx - 1], valBuf, 10);
       updateParam(tabIndx, valBuf);
     }else if(paramVals[tabIndx - 1]== 0 && encoderDir ==1){
       paramVals[tabIndx - 1] += encoderDir;
       itoa(paramVals[tabIndx - 1], valBuf, 10);
       updateParam(tabIndx, valBuf);
       
     }else if(paramVals[tabIndx - 1]== 100 && encoderDir ==-1){
       paramVals[tabIndx - 1] += encoderDir;
       itoa(paramVals[tabIndx - 1], valBuf, 10);
       updateParam(tabIndx, valBuf);
     }
     
   break;
   case 3:
     scaledEncoderValueParam = encoderValue%25;
     if(scaledEncoderValueParam == 0){
       txtParamIndx += encoderDir;
       updateParam(tabIndx, curves[txtParamIndx%3]);
     }
   break;
 
   }
   lastValue = encoderValue;
 }
*/

void updateEncoder(){
  uint8_t MSB = digitalRead(encoderPin1); //MSB = most significant bit
  uint8_t LSB = digitalRead(encoderPin2); //LSB = least significant bit

  uint8_t encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  uint8_t sum  = (lastEncoderValue << 2) | encoded; //adding it to the previous encoded value
  
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011){
    
      encoderDir = 1;
      encoderValue ++;
  }
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000){
      encoderDir = -1;
      encoderValue -- ;
  }

  lastEncoderValue = encoded; //store this value for next time
}
