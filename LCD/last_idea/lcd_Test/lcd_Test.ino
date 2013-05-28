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

uint8_t currModIndx  = 0;
boolean prmChange;

char* fxState[2] = {"OFF", "ON"};
char* modSources[3] = {"mid", "osc", "exp"};
////this is the dummy Preset

aKnubPreset activePreset = {"RIFF",0,
  
  {{"DIST", {22, 115, 1}, "mid", true, 1, "EXTERN"}, 
  {"TONE", {50, 50, 1}, "mid", true, 1, "EXTERN"},
  {"VOL", {3, 127, 1}, "mid", true, 1, "EXTERN"},
  {"BLEND", {110, 0, 1}, "osc", false, 1, "EXTERN"}, 
  {"DELAY", {10, 20, 1}, "osc", false, 1, "EXTERN"},
  {"FEED", {0, 127, 1}, "exp", true, 1, "EXTERN"},
  {"FREQ", {115, 20, 1}, "exp", true, 1, "EXTERN"},
  {"AMNT", {10, 127, 1}, "exp", true, 1, "EXTERN"}}
};

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
    (*drawFuncs[0])("", "", "", "", "", "", "", "", "");
    delay(500);
    (*drawFuncs[1])("", "", "", "", "", "", "", "", "");
    delay(500);
    initMemDisp();
    clearScreen();
    
    (*drawFuncs[2])("", "", "", "", "", "", "", "", "");
    
    itoa(currentPresetID, valBuf, 10);
    
    updatePreset(valBuf, activePreset.name, isEdited);
    
    clearScreen();
    
    pageLevel = 3;
     (*drawFuncs[pageLevel])("", "", "", "", "", "", "", "", "");
         itoa(currentPresetID, valBuf, 10);
         updatePreset(valBuf, activePreset.name, isEdited);
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
      
                 updateParam(0,activePreset.knubbies[currentParam].name);
                 updateParam(1,activePreset.knubbies[currentParam].modSource);
                 updateParam(2,boolToString(activePreset.knubbies[currentParam].state));
                 updateParam(3,toString(activePreset.knubbies[currentParam].numLoop));
                 updateParam(4,toString(activePreset.knubbies[currentParam].params[0]));
                 updateParam(5,toString(activePreset.knubbies[currentParam].params[1]));
                 updateParam(6,toString(activePreset.knubbies[currentParam].params[2]));    
    
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
             tab(paramTabs[tabIndx]);
        
             //customCursor(tabIndx, pageLevel);
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
             tab(paramTabs[tabIndx]);
        
             //customCursor(tabIndx, pageLevel);
       
     
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
                 
                 updateParam(tabIndx,activePreset.knubbies[currentParam].name);
                 updateParam(tabIndx+1,activePreset.knubbies[currentParam].modSource);
                 updateParam(tabIndx+2,boolToString(activePreset.knubbies[currentParam].state));
                 updateParam(tabIndx+3,toString(activePreset.knubbies[currentParam].numLoop));
                 updateParam(tabIndx+4,toString(activePreset.knubbies[currentParam].params[0]));
                 updateParam(tabIndx+5,toString(activePreset.knubbies[currentParam].params[1]));
                 updateParam(tabIndx+6,toString(activePreset.knubbies[currentParam].params[2]));
                 
                 
                 
             }
        break;
        
        case 1:
              scaledEncoderValueParam = encoderValue%25;
              if(scaledEncoderValueParam == 0){
                 txtParamIndx += encoderDir;
                 currModIndx = txtParamIndx%3;
             
                 updateParam(1, modSources[currModIndx]);
            } 
            break;
        case 4:
           ///MUST FIND A BETTER WAY OF DEALING WITH THIS
           
           currentParamVal = activePreset.knubbies[currentParam].params[0];
           
           if(currentParamVal>0 && currentParamVal<100){
               currentParamVal += encoderDir;
               
               (currentParamVal);
               itoa(currentParamVal, valBuf, 10);
               updateParam(tabIndx, valBuf);
               currentParamVal = activePreset.knubbies[currentParam].params[0] = currentParamVal;
          }else if(currentParamVal== 0 && encoderDir ==1){
                   currentParamVal += encoderDir;
                   itoa(currentParamVal, valBuf, 10);
                   updateParam(tabIndx, valBuf);
                   currentParamVal = activePreset.knubbies[currentParam].params[0] = currentParamVal;
          }else if(currentParamVal== 100 && encoderDir ==-1){
                   currentParamVal += encoderDir;
                   itoa(currentParamVal, valBuf, 10);
                   updateParam(tabIndx, valBuf);
                   currentParamVal = activePreset.knubbies[currentParam].params[0] = currentParamVal;
          }
       break;
       case 5:
           
           currentParamVal = activePreset.knubbies[currentParam].params[1];
           
           if(currentParamVal>0 && currentParamVal<100){
              currentParamVal += encoderDir;
              itoa(currentParamVal, valBuf, 10);
              updateParam(tabIndx, valBuf);
              currentParamVal = activePreset.knubbies[currentParam].params[1] = currentParamVal;
          }else if(currentParamVal== 0 && encoderDir ==1){
                   currentParamVal += encoderDir;
                   itoa(currentParamVal, valBuf, 10);
                   updateParam(tabIndx, valBuf);
                   currentParamVal = activePreset.knubbies[currentParam].params[1] = currentParamVal;
          }else if(currentParamVal== 100 && encoderDir ==-1){
                   currentParamVal += encoderDir;
                   itoa(currentParamVal, valBuf, 10);
                   updateParam(tabIndx, valBuf);
                   currentParamVal = activePreset.knubbies[currentParam].params[1] = currentParamVal;
         }
       break;
       case 6:
           ///not acitve yet
           currentParamVal = activePreset.knubbies[currentParam].params[2];
           
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
              
                 readKnubName(eepromAddr1, currentPresetID*maxNameLength, &activePreset);
                 
                 updatePreset(valBuf, activePreset.name, isEdited);
        }else if(encoderDir == -1 && currentPresetID > 0){
        
          currentPresetID += encoderDir;
          itoa(currentPresetID, valBuf, 10);
          
          readKnubName(eepromAddr1, currentPresetID*maxNameLength, &activePreset);  
          
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
