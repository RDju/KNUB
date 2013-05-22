#include <ClickButton.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <Wire.h>
#include "UI.h"
#include "memory.h"
/*
!!!!!! MUST USE POINTERS AND REF WHENEVER IS POSSIBLE


change the way params are updated via the encoder use function to wich you'll pass pointer so as to save up RAM space.

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

#define encoderPin1 2
#define encoderPin2 3

#define validBut 9
#define backBut 8

#define eepromAddr1  B01010000

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

ClickButton bValid(validBut, LOW, CLICKBTN_PULLUP);
ClickButton bckValid(backBut, LOW, CLICKBTN_PULLUP);


int pageLevel = 0;
int tabIndx = 0;
uint8_t currFx = 0;

int8_t encoderDir;

uint8_t currentPresetID = 0;
boolean isActive;
boolean isEdited;

uint8_t currentFx = 0;
uint8_t currentParam = 0;
uint8_t currentParamVal;
uint8_t currentCurve = 0;

boolean prmChange;

char* fxState[2] = {"OFF", "ON"};

////this is the dummy Preset

aKnubPreset activePreset = {"RIFF",0,
  3,
  {{"RAT", 3, 1, {{"DIST", {5, 100, 1}}, {"TONE", {64, 64, 1}}, {"VOL", {70, 70, 1}}}}, 
   {"HOLY", 1, 0, {{"BLEND", {0, 100, 1}}}}, 
   {"DELAY", 2, 1, {{"DELAY", {15, 56, 1}}, {"FEED", {64, 100, 1}}}}
 }};

void setup(){
  
  lcd.begin(9600);
  Serial.begin(9600);
  Wire.begin();
  initDisplay();
   
  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);
  digitalWrite(encoderPin1, HIGH);
  digitalWrite(encoderPin2, HIGH);
  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);
  
  writeKnubName(eepromAddr1, currentPresetID*maxNameLength, &activePreset);
  
  currentPresetID = 0;

    //startUp sequence
    (*drawFuncs[0])("", "", "", "");
    delay(500);
    (*drawFuncs[1])("", "", "", "");
    delay(500);
    initMemDisp();
    clearScreen();
    
    (*drawFuncs[2])("", "", "", "");
    itoa(currentPresetID, valBuf, 10);
    updatePreset(valBuf, activePreset.name);
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
         tabIndx = 0;
         (*drawFuncs[pageLevel])("", "", "", "");
         itoa(currentPresetID, valBuf, 10);
         updatePreset(valBuf, activePreset.name);
         time2ChangePage = false;
     break;
     case 3:
     
     clearScreen();
     tabIndx = 0;
         (*drawFuncs[pageLevel])(activePreset.fxPedals[currentFx].name, fxState[activePreset.fxPedals[currentFx].isOn], "", "");
         tabIndx = tabIndx%numTabs[pageLevel];
             tab(effectTabs[tabIndx]);
             customCursor(tabIndx,pageLevel);
    time2ChangePage = false;
     break;
     case 4:
     tabIndx = 0;
     clearScreen();
         (*drawFuncs[pageLevel])(activePreset.fxPedals[currentFx].knubs[currentParam].name, 
                                 toString(activePreset.fxPedals[currentFx].knubs[currentParam].params[0]), 
                                 toString(activePreset.fxPedals[currentFx].knubs[currentParam].params[1]),
                                 toString(activePreset.fxPedals[currentFx].knubs[currentParam].params[2]));
    for(int i =0; i<activePreset.fxPedals[currentFx].numKnub; i++){
                 
                   updateParam(i+1, toString(activePreset.fxPedals[currentFx].knubs[currentParam].params[i]));
                 
                 }
    time2ChangePage = false;
     break;
     case 5:
       clearScreen();
       (*drawFuncs[pageLevel])("", "",  "",  "");
       time2ChangePage = false;
       delay(saveTime*5);
       pageLevel = 4;
       time2ChangePage = true;
       
     break;
   }
  }
  //////////////////////
  
  //// tab button
  bValid.Update();
  
  if(bValid.click !=0){
    switch (pageLevel){
        case 0:
          if(bValid.click == 1){     
          pageLevel ++;
          time2ChangePage = true;
          }
        break;
        case 1:
         if(bValid.click == 1){
          pageLevel ++;
          time2ChangePage = true;
         }
        break;
        case 2:
          if(bValid.click == 2){
            pageLevel ++;
            if(isEdited == false){
              isEdited = true;
            }
            time2ChangePage = true;
          }
        break;
        case 3:
        
        if(bValid.click == 2 && tabIndx == 0){
          pageLevel ++;
          time2ChangePage = true;
          
        }else if(bValid.click == 1){
         
            if(activePreset.fxPedals[currentFx].isOn == 1){
                activePreset.fxPedals[currentFx].isOn = 0;
                updatePedalState(fxState[0]);
            }else if(activePreset.fxPedals[currentFx].isOn == 0){
                activePreset.fxPedals[currentFx].isOn = 1;  
                updatePedalState(fxState[1]);
            } 
      }   
           
       break;
        case 4:
          if(bValid.click == 1){
          
             tabIndx++;
             tabIndx = tabIndx%numTabs[pageLevel];
             tab(paramTabs[tabIndx]);
        
             customCursor(tabIndx, pageLevel);
        }else if(tabIndx == 0 && bValid.click == 2){
        
            ///save this param
            pageLevel = 5;
            time2ChangePage = true;
        }
        break;
        }
    }
  //////back button
  if(bckValid.Update()){
     
    if(bckValid.click==1){
      
      if(pageLevel > 2){
        pageLevel --;
        time2ChangePage = true;
      }
     }
   }

  
  //////////////////////////////////////////////
  
  /////// encoding Wheel/////////////////////

if(encoderValue != lastValue){
   switch(pageLevel){
     case 4 :
       switch(tabIndx){
         case 0:
            scaledEncoderValueParam = encoderValue%25;
            if(scaledEncoderValueParam == 0){
                 txtParamIndx += encoderDir;
                 currentParam = txtParamIndx%3;
                 updateParam(tabIndx,activePreset.fxPedals[currentFx].knubs[currentParam].name);
                 
                 for(int i =0; i<activePreset.fxPedals[currentFx].numKnub; i++){
                 
                   updateParam(i+1, toString(activePreset.fxPedals[currentFx].knubs[currentParam].params[i]));
                 
                 }
                 
             }
        break;
        case 1:
           ///MUST FIND A BETTER WAY OF DEALING WITH THIS
           
           currentParamVal = activePreset.fxPedals[currentFx].knubs[currentParam].params[0];
           
           if(currentParamVal>0 && currentParamVal<100){
               currentParamVal += encoderDir;
               
               (currentParamVal);
               itoa(currentParamVal, valBuf, 10);
               updateParam(tabIndx, valBuf);
               activePreset.fxPedals[currentFx].knubs[currentParam].params[0] = currentParamVal;
          }else if(currentParamVal== 0 && encoderDir ==1){
                   currentParamVal += encoderDir;
                   itoa(currentParamVal, valBuf, 10);
                   updateParam(tabIndx, valBuf);
                   activePreset.fxPedals[currentFx].knubs[currentParam].params[0] = currentParamVal;
          }else if(currentParamVal== 100 && encoderDir ==-1){
                   currentParamVal += encoderDir;
                   itoa(currentParamVal, valBuf, 10);
                   updateParam(tabIndx, valBuf);
                   activePreset.fxPedals[currentFx].knubs[currentParam].params[0] = currentParamVal;
          }
       break;
       case 2:
           
           currentParamVal = activePreset.fxPedals[currentFx].knubs[currentParam].params[1];
           
           if(currentParamVal>0 && currentParamVal<100){
              currentParamVal += encoderDir;
              itoa(currentParamVal, valBuf, 10);
              updateParam(tabIndx, valBuf);
              activePreset.fxPedals[currentFx].knubs[currentParam].params[1] = currentParamVal;
          }else if(currentParamVal== 0 && encoderDir ==1){
                   currentParamVal += encoderDir;
                   itoa(currentParamVal, valBuf, 10);
                   updateParam(tabIndx, valBuf);
                   activePreset.fxPedals[currentFx].knubs[currentParam].params[1] = currentParamVal;
          }else if(currentParamVal== 100 && encoderDir ==-1){
                   currentParamVal += encoderDir;
                   itoa(currentParamVal, valBuf, 10);
                   updateParam(tabIndx, valBuf);
                   activePreset.fxPedals[currentFx].knubs[currentParam].params[1] = currentParamVal;
         }
       break;
       case 3:
           ///not acitve yet
           currentParamVal = activePreset.fxPedals[currentFx].knubs[currentParam].params[2];
           
           scaledEncoderValueParam = encoderValue%25;
           if(scaledEncoderValueParam == 0){
              txtParamIndx += encoderDir;
              //updateParam(tabIndx, curves[txtParamIndx%3]);
           }
       break;
     }
     break;
   case 3:
      switch(tabIndx){
       
        case 0:
            scaledEncoderValueParam = encoderValue%25;
            if(scaledEncoderValueParam == 0){
                 txtParamIndx += encoderDir;
                 currentFx = txtParamIndx%3;
                 
                 updatePedalName(activePreset.fxPedals[currentFx].name);
                 updatePedalState(fxState[activePreset.fxPedals[currentFx].isOn]);
                 
                 if(currentFx != 0){
                   
                   currentFx += encoderDir;
                   currentFx -= 1;
                   (currentFx);
                 
                 }
               }
        break;
        case 1:
            
        break;
      }
    break;
    case 2:
    
        scaledEncoderValueParam = encoderValue%25;
        if(scaledEncoderValueParam == 0){
            if(encoderDir == 1){     
                 currentPresetID += encoderDir;
                 itoa(currentPresetID, valBuf, 10);
              
                 readKnubName(eepromAddr1, currentPresetID*maxNameLength, &activePreset);
                 
                 updatePreset(valBuf, activePreset.name);
        }else if(encoderDir == -1 && currentPresetID > 0){
        
          currentPresetID += encoderDir;
          itoa(currentPresetID, valBuf, 10);
          
          readKnubName(eepromAddr1, currentPresetID*maxNameLength, &activePreset);  
          
          updatePreset(valBuf, activePreset.name);
          }
        }
    break;
   }
 }
   lastValue = encoderValue;

}

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
