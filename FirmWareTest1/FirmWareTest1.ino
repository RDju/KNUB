#include <ClickButton.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <Wire.h>


#include "memory.h"
#include "presets.h"
#include "knubFuncs.h"
#include "UI.h"
#include "knubMidi.h"


/*
!!!!!! MUST USE POINTERS AND REF WHENEVER IS POSSIBLE


change the way params are updated via the encoder use function to wich you'll pass pointer so as to save up RAM space.

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/



 //#define DEBUG_UI //uncomment this to activate UI debugging


#define encoderPin1 2
#define encoderPin2 3

#define validBut 9
#define backBut 8

#define expressionPin 0


volatile uint8_t lastValue = 0;
volatile uint8_t lastEncoderValue = 0;
volatile uint8_t encoderValue = 0;
volatile uint8_t encoderValueParam = 0;
volatile uint8_t encoderValueParamVal = 0;
volatile uint8_t scaledEncoderValueParam = 0;

//boolean time2ChangePage;

uint8_t txtParamIndx;

uint8_t lastMSB = 0;
uint8_t lastLSB = 0;

char valBuf[4];

ClickButton bValid(validBut, LOW, CLICKBTN_PULLUP);
ClickButton bckValid(backBut, LOW, CLICKBTN_PULLUP);

byte pageLevel = 0;
uint8_t tabIndx = 0;
uint8_t currFx = 0;

int8_t encoderDir;

uint8_t currentPresetID = 0;
boolean isActive;
boolean isEdited;

uint8_t currentFx = 0;
uint8_t currentParam = 0;
uint8_t currentParamVal;
uint8_t digiMapParamVal;


uint8_t currModIndx  = 0;
uint8_t currSwIndx = 0;
boolean prmChange;

char* fxState[2] = {"OFF", "ON"};
char* modOns[2] = {"___", "EXP"};
char* switchTypes[5] = {"__", "L1", "L2", "L3", "L4"};

byte baseAddr = 5;
byte lastID = 5;

byte toPrint;

uint16_t prevExpVal;
uint16_t expVal;


void setup(){


  lcd.begin(9600);
  Serial.begin(9600);
  Wire.begin();
  midiSerial.begin(31250);

  initDisplay();

  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);
  digitalWrite(encoderPin1, HIGH);
  digitalWrite(encoderPin2, HIGH);
  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);
  
  lastID = readByte(eepromAddr1, lastPresetMemSpace);
  Serial.println(lastID);
  
  readKnubPreset(eepromAddr1, lastID * presetSize, &currentPreset);
  delay(50);
  
  updateKnubs(&currentPreset);
  // fill up loopsOut array
  for(uint8_t i = 0; i<numKnubbies; i++){

    fillLoopsOut(currentPreset.knubbies[i].numLoop, currentPreset.knubbies[i].state);
  }
  
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

  updatePreset(currentPreset.name, isEdited);

 
}

void loop(){
   //testDacs();

   
  midiInRead(pageLevel);

  if(pageLevel == 2){
      
    doExpressionPedal(analogRead(expressionPin));
      
  }
  

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

       updatePreset(currentPreset.name, isEdited);

       time2ChangePage = false;
       break;
      
       case 3:
       tabIndx = 0;
       currentParam = 0;

       clearScreen();

       updateParam(0, toString(currentParam + 1));
       updateParam(1,currentPreset.knubbies[currentParam].name);
       updateParam(2,stateToString(currentPreset.knubbies[currentParam].state));
       updateParam(3,modOns[currentPreset.knubbies[currentParam].modOn]);
       updateNumParam(4,customDigits[currentPreset.knubbies[currentParam].params[0]]);
       updateNumParam(5,customDigits[currentPreset.knubbies[currentParam].params[1]]);
       updateParam(6,customCurveDigits[currentPreset.knubbies[currentParam].params[2]]);    
       updateParam(7,switchTypes[currentPreset.knubbies[currentParam].numLoop]);
       time2ChangePage = false;
       break;
       case 4:
       clearScreen();
       (*drawFuncs[pageLevel+1])("", "",  "",  "", "", "", "", "", "");
       time2ChangePage = false;
       writeKnubPreset(eepromAddr1, readAdr, &currentPreset);
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
        
        time2ChangePage = true;
      }
      break;
      case 4:

      if(bValid.click == 2){
        pageLevel ++;
        time2ChangePage = true;

      }   
      break;
      case 3:
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
        
        pageLevel = 4;
        isEdited = false;
        time2ChangePage = true;


        }else if(bckValid.click == 1 && pageLevel == 3){
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
     case 3 :
       switch(tabIndx){
         case 0:
            scaledEncoderValueParam = encoderValue%25;
            if(scaledEncoderValueParam == 0){
                 
                 txtParamIndx += encoderDir;
                 currentParam = txtParamIndx%8;
            
                 updateParam(0, toString(currentParam + 1));
                 updateParam(1,currentPreset.knubbies[currentParam].name);
                 updateParam(2,stateToString(currentPreset.knubbies[currentParam].state));
                 updateParam(3,modOns[currentPreset.knubbies[currentParam].modOn]);
                 updateNumParam(4,customDigits[currentPreset.knubbies[currentParam].params[0]]);
                 updateNumParam(5,customDigits[currentPreset.knubbies[currentParam].params[1]]);
                 updateParam(6,customCurveDigits[currentPreset.knubbies[currentParam].params[2]]);  
                 updateParam(7, switchTypes[currentPreset.knubbies[currentParam].numLoop]);
                 
             }
        break;
        case 6:
              checkEdition();
               scaledEncoderValueParam = encoderValue%25;
              if(scaledEncoderValueParam == 0){
                 txtParamIndx += encoderDir;
                 currSwIndx = txtParamIndx%13;
             
                 updateParam(7, switchTypes[currSwIndx]);
            } 
        break;
        case 4:
              checkEdition();
              scaledEncoderValueParam = encoderValue%25;
              if(scaledEncoderValueParam == 0){
                 txtParamIndx += encoderDir;
                 currModIndx = txtParamIndx%2;
             
                 updateParam(3, modOns[currModIndx]);
            } 
            break;
        case 1:
           
            ///EDITED

            checkEdition();
           ///MUST FIND A BETTER WAY OF DEALING WITH THIS
           
           currentParamVal = currentPreset.knubbies[currentParam].params[0];
           
           if(currentParamVal>0 && currentParamVal<255){
              
              currentParamVal += encoderDir;
              currentPreset.knubbies[currentParam].params[0] = currentParamVal;
              updateNumParam(4,customDigits[currentPreset.knubbies[currentParam].params[0]]);
              turnKnub(currentParam, currentPreset.knubbies[currentParam].params[0]);
               
            }else if(currentParamVal== 0 && encoderDir ==1){
                   
                    currentParamVal += encoderDir;
                    currentPreset.knubbies[currentParam].params[0] = currentParamVal;
                    updateNumParam(4,customDigits[currentPreset.knubbies[currentParam].params[0]]);
                    turnKnub(currentParam, currentPreset.knubbies[currentParam].params[0]);
                   
            }else if(currentParamVal== 255 && encoderDir ==-1){
            
                   currentParamVal += encoderDir;
                   currentPreset.knubbies[currentParam].params[0] = currentParamVal;
                   updateNumParam(4,customDigits[currentPreset.knubbies[currentParam].params[0]]);
                   turnKnub(currentParam, currentPreset.knubbies[currentParam].params[0]);
            }else if(currentParamVal == 255 && encoderDir == 1){
               
              
                    currentParamVal  = 255;
                    currentPreset.knubbies[currentParam].params[0] = currentParamVal;
                    updateNumParam(4,customDigits[currentPreset.knubbies[currentParam].params[0]]);
                    turnKnub(currentParam, currentPreset.knubbies[currentParam].params[0]);
            }else if(currentParamVal == 0 && encoderDir == -1){

                    currentParamVal = 0;
                    currentPreset.knubbies[currentParam].params[0] = currentParamVal;
                    updateNumParam(4,customDigits[currentPreset.knubbies[currentParam].params[0]]);
                    turnKnub(currentParam, currentPreset.knubbies[currentParam].params[0]);
            }
       break;
       case 2:
           checkEdition();
           
            currentParamVal = currentPreset.knubbies[currentParam].params[1];
           
            if(currentParamVal>0 && currentParamVal<256){
              
              currentParamVal += encoderDir;
              
              currentParamVal = currentPreset.knubbies[currentParam].params[1] = currentParamVal;

              updateNumParam(5, customDigits[currentParamVal]);
              
            }else if(currentParamVal== 0 && encoderDir ==1){
                   
                   currentParamVal += encoderDir;
                   
                   currentParamVal = currentPreset.knubbies[currentParam].params[1] = currentParamVal;

                   updateNumParam(5, customDigits[currentParamVal]);
                   
                   
                   
            }else if(currentParamVal== 255 && encoderDir ==-1){
                   
                   currentParamVal += encoderDir;

                   currentParamVal = currentPreset.knubbies[currentParam].params[1] = currentParamVal;
                   
                   updateNumParam(5, customDigits[currentParamVal]);
            
            }else if(currentParamVal == 255 && encoderDir == 1){
               
              
                    currentParamVal  = 255;
                    currentPreset.knubbies[currentParam].params[0] = currentParamVal;
                    updateNumParam(5,customDigits[currentPreset.knubbies[currentParam].params[0]]);
            }else if(currentParamVal == 0 && encoderDir == -1){

                    currentParamVal = 0;
                    currentPreset.knubbies[currentParam].params[0] = currentParamVal;
                    updateNumParam(5,customDigits[currentPreset.knubbies[currentParam].params[0]]);
            }
       break;
       /*
       case 3:
           ///not acitve yet
           
           currentParamVal = currentPreset.knubbies[currentParam].params[2];
           
           scaledEncoderValueParam = encoderValue%25;
           if(scaledEncoderValueParam == 0){
              txtParamIndx += encoderDir;
              //updateParam(tabIndx, curves[txtParamIndx%3]);
           }
       break;
       */
       case 5:
            checkEdition();
           ///not acitve yet
           currentParamVal = currentPreset.knubbies[currentParam].params[2];
           
           scaledEncoderValueParam = encoderValue%25;
           if(scaledEncoderValueParam == 0){
              txtParamIndx += encoderDir;
              //updateParam(2,boolToString(currentPreset.knubbies[currentParam].state));
              updateParam(2, stateToString(txtParamIndx%2));
           }
       break;
      
     }
     break;
   case 4:
      
      switch(tabIndx){
       
        case 0:
            /*
            scaledEncoderValueParam = encoderValue%25;
            if(scaledEncoderValueParam == 0){
                 txtParamIndx += encoderDir;
                 currentFx = txtParamIndx%3;
                 
                 updatePedalName(currentPreset.knubbies[currentFx].name);
                 updatePedalState(fxState[currentPreset.knubbies[currentFx].isOn]);
                 
                 if(currentFx != 0){
                   
                   currentFx += encoderDir;
                   currentFx -= 1;
                   (currentFx);
                 
                 }
               }
          */
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
              
                 readKnubPresetName(eepromAddr1, currentPresetID*maxNameLength, &currentPreset);
                 
                 updatePreset(currentPreset.name, isEdited);
        }else if(encoderDir == -1 && currentPresetID > 0){
        
          currentPresetID += encoderDir;
          itoa(currentPresetID, valBuf, 10);
          
          readKnubPresetName(eepromAddr1, currentPresetID*maxNameLength, &currentPreset);  
          
          updatePreset(currentPreset.name, isEdited);
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


void checkEdition(){

  if(isEdited == false){


      isEdited = true;

  }
}