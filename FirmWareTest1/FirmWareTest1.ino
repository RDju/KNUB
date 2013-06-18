#include <ClickButton.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <Wire.h>


#include "memory.h"
#include "knubFuncs.h"
#include "UI.h"

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

volatile uint8_t lastValue = 0;
volatile uint8_t lastEncoderValue = 0;
volatile uint8_t encoderValue = 0;
volatile uint8_t encoderValueParam = 0;
volatile uint8_t encoderValueParamVal = 0;
volatile uint8_t scaledEncoderValueParam = 0;

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
uint8_t digiMapParamVal;
//uint8_t currentCurve = 0;

uint8_t currModIndx  = 0;
uint8_t currSwIndx = 0;
boolean prmChange;

char* fxState[2] = {"OFF", "ON"};
char* modSources[10] = {"___", "EXP", "M01", "M02", "M03", "M04", "M05", "M06", "M07", "M08"};
char* switchTypes[13] = {"__", "I1", "I2", "I3", "I4", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8"};


aKnubPreset activePreset = {"DEFAULT",{0, 0, 0, 0, 0},
  
  {{"DIST   ", {127, 127, 1}, 0, 1, 1}, 
  {"TONE   ", {127, 50, 1}, 0, 1, 1},
  {"OUT3   ", {127,  100, 1}, 0, 1, 1},
  {"VOL    ", {127, 0, 1}, 1, 0, 2}, 
  {"OUT5   ", {10, 20, 1}, 1, 1, 3},
  {"OUT6   ", {0, 100, 1}, 2, 0, 3},
  {"OUT7   ", {100, 20, 1}, 3, 1, 4},
  {"OUT8   ", {10, 100, 1}, 4, 1, 4}}
};

byte toPrint;


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
  
  ///writeKnubName(eepromAddr1, currentPresetID*maxNameLength, &activePreset);
  
  currentPresetID = 0;

    //startUp sequence
    (*drawFuncs[0])("", "", "", "", "", "", "", "", "");
    delay(500);
    (*drawFuncs[1])("", "", "", "", "", "", "", "", "");
    delay(500);
    initMemDisp();
    
    clearScreen();
    pageLevel = 2;
    tabIndx = 0;
    
    (*drawFuncs[pageLevel])("", "", "", "", "", "", "", "", "");
    itoa(currentPresetID, valBuf, 10);
    updatePreset(valBuf, activePreset.name, isEdited);
   
   
   ///now turn knubs
   
   for(uint8_t i = 0; i < 8; i++ ){
   
     turnKnub(i,activePreset.knubbies[i].params[0]);
   
   
   }
   

}

void loop(){
  
   ////dealing with pages 
  if(time2ChangePage){
   switch(pageLevel){
     case 0:
     clearScreen();
       (*drawFuncs[pageLevel])("", "", "", "", "", "", "", "", "");
        time2ChangePage = false;
     break;
     case 1:  
     clearScreen();
        (*drawFuncs[pageLevel])("", "", "", "", "", "", "", "", "");
        time2ChangePage = false;
     break;
     case 2:
     clearScreen();
         tabIndx = 0;
         (*drawFuncs[pageLevel])("", "", "", "", "", "", "", "", "");
         itoa(currentPresetID, valBuf, 10);
         updatePreset(valBuf, activePreset.name, isEdited);
         time2ChangePage = false;
     break;
     case 3:
  
     break;
     case 4:
     tabIndx = 0;
     clearScreen();
                 
                 updateParam(0, toString(currentParam + 1));
                 updateParam(1,activePreset.knubbies[currentParam].name);
                 updateParam(2,stateToString(activePreset.knubbies[currentParam].state));
                 updateParam(3,modSources[activePreset.knubbies[currentParam].modSource]);
                 updateParam(4,customDigits[map(activePreset.knubbies[currentParam].params[0], 0, 255, 0, 100)]);
                 updateParam(5,customDigits[map(activePreset.knubbies[currentParam].params[1], 0, 255, 0, 100)]);
                 updateParam(6,customCurveDigits[activePreset.knubbies[currentParam].params[2]]);    
                 updateParam(7,switchTypes[activePreset.knubbies[currentParam].numLoop]);
    time2ChangePage = false;
     break;
     case 5:
       clearScreen();
       (*drawFuncs[pageLevel])("", "",  "",  "", "", "", "", "", "");
       time2ChangePage = false;
       delay(saveTime*5);
       pageLevel = 2;
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
        
        if(bValid.click == 2){
          pageLevel ++;
          time2ChangePage = true;
          
        }   
       break;
        case 4:
          if(bValid.click == 1){
          
             tabIndx++;
             tabIndx = tabIndx%numTabs[pageLevel];
             tab(chParamTabs[tabIndx]);
        
             customCursor(tabIndx, pageLevel);
          }
        break;
        }
    }
  //////back button
  bckValid.Update();
     
   if(bckValid.click != 0){
    if(bckValid.click==2 && pageLevel > 2){
      
  
        pageLevel = 2;
        time2ChangePage = true;
   
        
      }else if(bckValid.click == 2 && pageLevel == 2){
       
          Serial.println("saving");
          pageLevel = 5;
          Serial.println(pageLevel);
          isEdited = false;
          time2ChangePage = true;
     
     }else if(bckValid.click == 1 && pageLevel == 4){
             tabIndx--;
             tabIndx = tabIndx%numTabs[pageLevel];
             tab(chParamTabs[tabIndx]);
        
             customCursor(tabIndx, pageLevel);
       
     
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
                 currentParam = txtParamIndx%8;
            
                 updateParam(0, toString(currentParam + 1));
                 updateParam(1,activePreset.knubbies[currentParam].name);
                 updateParam(2,stateToString(activePreset.knubbies[currentParam].state));
                 updateParam(3,modSources[activePreset.knubbies[currentParam].modSource]);
                 updateParam(4,customDigits[map(activePreset.knubbies[currentParam].params[0], 0, 255, 0, 100)]);
                 updateParam(5,customDigits[map(activePreset.knubbies[currentParam].params[1], 0, 255, 0, 100)]);
                 updateParam(6,customCurveDigits[activePreset.knubbies[currentParam].params[2]]);  
                 updateParam(7, switchTypes[activePreset.knubbies[currentParam].numLoop]);
                 
             }
        break;
        case 6:
               scaledEncoderValueParam = encoderValue%25;
              if(scaledEncoderValueParam == 0){
                 txtParamIndx += encoderDir;
                 currSwIndx = txtParamIndx%13;
             
                 updateParam(7, switchTypes[currSwIndx]);
            } 
        break;
        case 4:
              scaledEncoderValueParam = encoderValue%25;
              if(scaledEncoderValueParam == 0){
                 txtParamIndx += encoderDir;
                 currModIndx = txtParamIndx%10;
             
                 updateParam(3, modSources[currModIndx]);
            } 
            break;
        case 1:
           ///MUST FIND A BETTER WAY OF DEALING WITH THIS
           
           currentParamVal = activePreset.knubbies[currentParam].params[0];
           
           if(currentParamVal>0 && currentParamVal<255){
               currentParamVal += encoderDir;
               
               turnKnub(currentParam, currentParamVal); 
               
               
               updateParam(4, customDigits[map(currentParamVal, 0, 255, 0, 100)]);
               
               activePreset.knubbies[currentParam].params[0] = currentParamVal;
                
        }else if(currentParamVal== 0 && encoderDir ==1){
                   
                   currentParamVal += encoderDir;
                   turnKnub(currentParam, currentParamVal);
                   updateParam(4, customDigits[map(currentParamVal, 0, 255, 0, 100)]);
                 
               
                   activePreset.knubbies[currentParam].params[0] = currentParamVal;
                   
          }else if(currentParamVal== 255 && encoderDir ==-1){
                   
                   currentParamVal += encoderDir;
                   turnKnub(currentParam, currentParamVal);
                   updateParam(4, customDigits[map(currentParamVal, 0, 255, 0, 100)]);
                 
                   activePreset.knubbies[currentParam].params[0] = currentParamVal;
          }
       break;
       case 2:
           
           currentParamVal = activePreset.knubbies[currentParam].params[1];
           
           if(currentParamVal>0 && currentParamVal<100){
              currentParamVal += encoderDir;
              
              updateParam(5, customDigits[currentParamVal]);
              currentParamVal = activePreset.knubbies[currentParam].params[1] = currentParamVal;
          }else if(currentParamVal== 0 && encoderDir ==1){
                   currentParamVal += encoderDir;
                   updateParam(5, customDigits[currentParamVal]);
                   currentParamVal = activePreset.knubbies[currentParam].params[1] = currentParamVal;
                   
          }else if(currentParamVal== 100 && encoderDir ==-1){
                   currentParamVal += encoderDir;
                   
                   updateParam(5, customDigits[currentParamVal]);
                   
                   currentParamVal = activePreset.knubbies[currentParam].params[1] = currentParamVal;
         }
       break;
       case 3:
           ///not acitve yet
           currentParamVal = activePreset.knubbies[currentParam].params[2];
           
           scaledEncoderValueParam = encoderValue%25;
           if(scaledEncoderValueParam == 0){
              txtParamIndx += encoderDir;
              //updateParam(tabIndx, curves[txtParamIndx%3]);
           }
       break;
       case 5:
           ///not acitve yet
           currentParamVal = activePreset.knubbies[currentParam].params[2];
           
           scaledEncoderValueParam = encoderValue%25;
           if(scaledEncoderValueParam == 0){
              txtParamIndx += encoderDir;
              //updateParam(2,boolToString(activePreset.knubbies[currentParam].state));
              updateParam(2, stateToString(txtParamIndx%2));
           }
       break;
     }
     break;
   case 3:
      
      switch(tabIndx){
       /*
        case 0:
            scaledEncoderValueParam = encoderValue%25;
            if(scaledEncoderValueParam == 0){
                 txtParamIndx += encoderDir;
                 currentFx = txtParamIndx%3;
                 
                 updatePedalName(activePreset.knubbies[currentFx].name);
                 updatePedalState(fxState[activePreset.knubbies[currentFx].isOn]);
                 
                 if(currentFx != 0){
                   
                   currentFx += encoderDir;
                   currentFx -= 1;
                   (currentFx);
                 
                 }
               }
        
        break;
        case 1:
            
        break;
      */  
    }
    break;
    case 2:
    
        scaledEncoderValueParam = encoderValue%25;
        if(scaledEncoderValueParam == 0){
            if(encoderDir == 1){     
                 currentPresetID += encoderDir;
                 itoa(currentPresetID, valBuf, 10);
              
                 readKnubPresetName(eepromAddr1, currentPresetID*maxNameLength, &activePreset);
                 
                 updatePreset(valBuf, activePreset.name, isEdited);
        }else if(encoderDir == -1 && currentPresetID > 0){
        
          currentPresetID += encoderDir;
          itoa(currentPresetID, valBuf, 10);
          
          readKnubPresetName(eepromAddr1, currentPresetID*maxNameLength, &activePreset);  
          
          updatePreset(valBuf, activePreset.name, isEdited);
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
