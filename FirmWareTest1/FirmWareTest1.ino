#include <ClickButton.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
//#include <SPI.h>
//#include <Ethernet.h> // version IDE 0022
//#include <Z_OSC.h>
#include <Bounce.h>
#include "presets.h"
#include "knubFuncs2.h"
#include "UI.h"
#include "looperMidi.h"
#include "knubMidi.h"
//#include "knubOSC.h"
#include "knubUtils.h"

#define encoderPin1 2
#define encoderPin2 3

#define validBut 9
#define backBut 8

#define expressionPin 0

/* Timer2 reload value, globally available */
unsigned int tcnt2;  
// Instantiate a Bounce object with a 5 millisecond debounce time
// Only pin1 needs to be debounced. It is assumed that pin2
// will be stable when reading pin1
Bounce bouncer1 = Bounce( encoderPin1,5 );

//#define Do_OSC

//Encoder variables :
volatile uint8_t lastValue = 0;
volatile uint8_t lastEncoderValue = 0;
volatile uint8_t encoderValue = 0;
volatile uint8_t scaledEncoderValueParam = 0;
uint8_t txtParamIndx = 0;
int8_t encoderDir; //-1 -> to the turn to the left, +1 -> turn to the right

uint16_t baseID;

ClickButton bValid(validBut, LOW, CLICKBTN_PULLUP);
ClickButton bckValid(backBut, LOW, CLICKBTN_PULLUP);

//byte pageLevel = 0;
uint8_t tabIndx = 0;
byte prevIndx;

uint8_t currentPresetID = 0;
//boolean isEdited;

uint8_t currentParam = 0;
uint8_t currentParamVal;

uint8_t currModIndx  = 0;
uint8_t currSwIndx = 0;

char* modOns[3] = {"___", "EXP","MID"};
char* switchOut[5] = {"L1", "L2", "L3", "L4","__"}; //Indicate pedal physical localisation

boolean readAdrChange;

void setupTimer(){
   /* First disable the timer overflow interrupt while we're configuring */
  TIMSK2 &= ~(1<<TOIE2);  
  /* Configure timer2 in normal mode (pure counting, no PWM etc.) */
  TCCR2A &= ~((1<<WGM21) | (1<<WGM20));  
  TCCR2B &= ~(1<<WGM22);  

  /* Select clock source: internal I/O clock */
  ASSR &= ~(1<<AS2);  

  /* Disable Compare Match A interrupt enable (only want overflow) */
  TIMSK2 &= ~(1<<OCIE2A);  

  /* Now configure the prescaler to CPU clock divided by 128 */
  TCCR2B |= (1<<CS22)  | (1<<CS20); // Set bits  
  TCCR2B &= ~(1<<CS21);             // Clear bit  


  /* We need to calculate a proper value to load the timer counter. 
   * The following loads the value 131 into the Timer 2 counter register 
   * The math behind this is: 
   * (CPU frequency) / (prescaler value) = 125000 Hz = 8us. 
   * (desired period) / 8us = 125. 
   * MAX(uint8) + 1 - 125 = 131; 
   */

  /* Save value globally for later reload in ISR */
  tcnt2 = 131;   
  /* Finally load end enable the timer */
  TCNT2 = tcnt2;  
  TIMSK2 |= (1<<TOIE2);  
}

void setup(){
  
  baseID = 0;

  Wire.begin();

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);

  midiSerial.begin(31250);
  looperSerial.begin(31250);

  //enable read for midiSerial only
  midiSerial.listen();

  sendSwitchSysEx();

  lcd.createChar(0, ledOFF);
  lcd.createChar(1, ledON);
  lcd.createChar(3, stateOFF);
  lcd.createChar(4, stateON);

  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);
  digitalWrite(encoderPin1, HIGH);
  digitalWrite(encoderPin2, HIGH);
  //attachInterrupt(0, updateEncoder, CHANGE); //???: why 2 interruptions ?
  //attachInterrupt(1, updateEncoder, CHANGE); 

#ifdef Do_OSC 
  if(Ethernet.begin(myMac) ==0){

    //if doesn't work use fixed IP
    Ethernet.begin(myMac, myIp);

  }
  //OSC--------------------------------------
#endif


  //writeByte(eepromAddr1, lastPresetMemSpace, );
  //read last loaded ID and load that one
  //lastID = 5; //TODO: understand && remove ?
  pageLevel = 1;
  readAdr = 5;
  readAdrChange = false;
  readKnubPreset(eepromAddr1, ((lastID-baseID) * presetSize)+baseID, &currentPreset);
  //printCurrentPreset();
  updateKnubs(&currentPreset);
  updateLoopsOut(&currentPreset);

  //startUp sequence
  productPage();
  softwareVersion();
  presetPage();

  updatePreset(currentPreset.name , isPresetEdited(&currentPreset) );
  checkUILeds();

  //???: usefull ?
  pinMode(upPin, INPUT);
  pinMode(downPin, INPUT);
  digitalWrite(upPin, HIGH);
  digitalWrite(downPin, HIGH);

  prevUp = digitalRead(upPin);
  prevDown = digitalRead(downPin);
  
  setupTimer();
}

void loop(){
  
  //printCurrentPreset();

  if(pageLevel == 1){
    midiInRead();
    doSwitchInDec();
    doExpressionPedal(analogRead(expressionPin));
  }

  ////dealing with pages s
  if(time2ChangePage){
    time2ChangePage = false;
    switch(pageLevel){
    case 1: //Preset Page
      //Serial.print("readindx : ");
      //Serial.println(readindx);
      tabIndx = 0;
      presetPage();
      updatePreset(currentPreset.name, isPresetEdited(&currentPreset));
      checkUILeds();
      break;
    case 3: //Knubbie Page
      tabIndx = 0;
      currentParam = 0;
      knubbiePage(currentParam, currentPreset, modOns, switchOut);
      break;

    case 4: //Edited Knubbie Page 
      //time to save things.
      wantToSave();
      break;
    case 5:
      pagePref();
      break;
    }
  }
  //////////////////////

  //// tab button
  bValid.Update();
  
  if(bValid.clicks !=0){
    switch (pageLevel){
    case 1: //Preset Page
            if(bValid.clicks == -1){
              pageLevel = 3;
              time2ChangePage = true;
            } else if (bValid.clicks==1){
              
                if (readAdrChange){
                  prevRead = tempReadAdr;
                  readAdrChange = false;
                  
                  readKnubPreset(eepromAddr1, readAdr, &currentPreset);
                          updateKnubs(&currentPreset);      
                
                
                          updateLoopsOut(&currentPreset);
                
                          time2ChangePage = true;
                          prevRead = readAdr;
                          isTempReadindx = false;
                  
                } else {
                        //increment preset and load
                        if(readindx < 60){
                          readindx += 1;
                          if (readindx == 60) readindx = 0;
                          readAdr = ((readindx-baseID)*presetSize)+baseAddr;
                        }
                        if(readAdr != prevRead){
                          readKnubPreset(eepromAddr1, readAdr, &currentPreset);
                          updateKnubs(&currentPreset);      
                
                #ifdef DEBUG_LOAD_PRESET
                          Serial.println(readAdr);
                          debugKnubPreset(&currentPreset);
                #endif
                
                          updateLoopsOut(&currentPreset);
                
                          time2ChangePage = true;
                          prevRead = readAdr;
                        }
               } 
            }
      break;
      
    case 3: //knubbie page
      if(bValid.clicks == 1){
        prevIndx = tabIndx;
        tabIndx++;
        tabIndx = tabIndx%(sizeof(chParamTabs)+1);//numTabs[pageLevel];//TODO: numTabs usefull ?
        tab(chParamTabs[tabIndx]);
        customCursor(tabIndx, pageLevel, prevIndx);
      }
      break;
   case 4:
      if(bValid.clicks == 1){
          savePage();
          writeKnubPreset(eepromAddr1, readAdr, &currentPreset);
          //saveEditedKnubbies(eepromAddr1, readAdr, &currentPreset);
          delay(50);
          pageLevel = 1;
          time2ChangePage = true;
      }
      break;
    }
  }

  //////back button
  bckValid.Update();

  if(bckValid.clicks != 0){

    // if(bckValid.clicks==2 && pageLevel > 2){
    //   pageLevel = 2;
    //   time2ChangePage = true;
    // }

    switch(pageLevel){
    case 1:
      /*if(bckValid.clicks == 2 && isPresetEdited(&currentPreset)){
        pageLevel = 4;
        //for (int i = 0; i < 8; i++)
          //currentPreset.knubbies[i].isEdited = false;
        time2ChangePage = true;
      } else */
      if (bckValid.clicks == -1){
          pageLevel = 5;
          time2ChangePage = true;
      } else if (bckValid.clicks == 1){
        
               if (readAdrChange){
                      readAdr = prevRead;
                      /*readKnubPreset(eepromAddr1, readAdr, &currentPreset);
                      updateKnubs(&currentPreset);
                      
                      updateLoopsOut(&currentPreset);*/
                      readAdrChange = false;
                      readindx = tempReadindx; //TODO: get the initial indx
                      Serial.print("readAdr : ");
                      Serial.println(readAdr);
                      time2ChangePage = true;
                      isTempReadindx = false;
                      
                } else {
                      if(readindx >= 0){
                        readindx -=1;
                        if (readindx == -1) readindx = 59;
                        readAdr = ((readindx-baseID)*presetSize)+baseAddr;
                      }
              
                      if(readAdr != prevRead){
              
                        readKnubPreset(eepromAddr1, readAdr, &currentPreset);
                        updateKnubs(&currentPreset);
              
                        //writeByte(eepromAddr1, lastPresetMemSpace, readindx);
              
                        updateLoopsOut(&currentPreset);
              
                        time2ChangePage = true;
                        prevRead = readAdr;
                      }
               }
      }
      break;
    case 3:
      if(bckValid.clicks == 1){
        prevIndx = tabIndx;
        if (tabIndx == 0)
          tabIndx = sizeof(chParamTabs)+1;
        tabIndx--;
        tabIndx = tabIndx%(sizeof(chParamTabs)+1);
        tab(chParamTabs[tabIndx]);

        customCursor(tabIndx, pageLevel, prevIndx);
      } else if (bckValid.clicks == -1){
          if ( isPresetEdited(&currentPreset))
              pageLevel = 4;
          else
              pageLevel = 1;
              
          time2ChangePage = true;
      }
      break;
      
    case 4:
      if(bckValid.clicks == 1){
          //TODO: paramModified = false
          //TODO: updatePreset
  
          pageLevel = 1;
          time2ChangePage = true;
      }
      break;
    case 5:
      if (bckValid.clicks == -1){
        pageLevel = 1;
        time2ChangePage = true;
      }
      break;
    }
  }


  //////////////////////////////////////////////

  /////// encoding Wheel/////////////////////

  if(encoderValue != lastValue){
    //if (pageLevel == 3){
    switch(pageLevel){
    case 1:
        
        if (!isTempReadindx){
          tempReadindx = readindx;
          isTempReadindx = true;
        }
        
        
        readindx+=encoderDir;
        if (readindx == -1) readindx = presetsNumber - 1;
        else if (readindx == presetsNumber) readindx = 0;
          
        readAdr = ((readindx-baseID)*presetSize)+baseAddr;

        if(readAdr != tempReadAdr){
          
          readKnubPresetNameTemp(eepromAddr1, readAdr);
          presetPage();
          updatePreset(nameTemp, false);

          readAdrChange = true;

          tempReadAdr = readAdr;
        }
          
         break;
    case 3:
            switch(tabIndx){
            case 0: //change knubbies
      
              scaledEncoderValueParam = encoderValue%2; //To increment only one time of two
                
              if ((scaledEncoderValueParam == 0 && encoderDir == 1 && currentParam < numKnubbies) || (scaledEncoderValueParam == 0 && encoderDir == -1 && currentParam > -1)){

                if (currentParam == numKnubbies -1 && encoderDir == 1) txtParamIndx = 0;
                else if (currentParam == 0 && encoderDir == -1) txtParamIndx = numKnubbies - 1;
                else txtParamIndx += encoderDir;
                
                currentParam = txtParamIndx;
                knubbiePage(currentParam, currentPreset,modOns, switchOut);
              } 
              break;
            case 1: //change first value
      
              ///EDITED
      
              //checkEdition(isEdited);
              //currentPreset.knubbies[currentParam].isEdited = true;
              currentPreset.knubbies[currentParam].isParamsEdited[0] = true;
              ///MUST FIND A BETTER WAY OF DEALING WITH THIS
      
              currentParamVal = currentPreset.knubbies[currentParam].params[0];
      
              if ((currentParamVal>0 && currentParamVal<255) || (currentParamVal== 0 && encoderDir ==1) || (currentParamVal== 255 && encoderDir ==-1))
                currentParamVal += encoderDir;
                
              currentPreset.knubbies[currentParam].params[0] = currentParamVal;
              updateNumParam(4,map(currentPreset.knubbies[currentParam].params[0], 0, 255, 0, 100));
              turnKnub(currentParam, currentPreset.knubbies[currentParam].params[0]);
              break;
              
            case 2: // change 2nd value
              //currentPreset.knubbies[currentParam].isEdited = true;
              //checkEdition(isEdited);
              currentPreset.knubbies[currentParam].isParamsEdited[1] = true;
      
              currentParamVal = currentPreset.knubbies[currentParam].params[1];
      
              if((currentParamVal>0 && currentParamVal<256) || (currentParamVal == 0 && encoderDir == 1) || (currentParamVal == 255 && encoderDir == -1))
                currentParamVal += encoderDir;
                
              currentParamVal = currentPreset.knubbies[currentParam].params[1] = currentParamVal;
              updateNumParam(5, map(currentParamVal, 0, 255, 0, 100));
          
              break;
              /*
            case 3:
               ///not acitve yet
               currentPreset.knubbies[currentParam].isParamsEdited[2] = true;
               currentParamVal = currentPreset.knubbies[currentParam].params[2];
               
               scaledEncoderValueParam = encoderValue%25;
               if(scaledEncoderValueParam == 0){
               txtParamIndx += encoderDir;
               //updateParam(tabIndx, curves[txtParamIndx%3]);
               }
               break;
               */
            case 4:
              //checkEdition(isEdited);
              //currentPreset.knubbies[currentParam].isEdited = true;
              
              currentPreset.knubbies[currentParam].isParamsEdited[2] = true;
              scaledEncoderValueParam = encoderValue%5;//25;
              if(scaledEncoderValueParam == 0){
                txtParamIndx += encoderDir;
                currModIndx = txtParamIndx%3;
                
                currentPreset.knubbies[currentParam].params[3] = currModIndx;
                
                updateParam(3, modOns[currModIndx]);
              } 
              break;
            case 5:
              //checkEdition(isEdited);
              //currentPreset.knubbies[currentParam].isEdited = true;
              currentPreset.knubbies[currentParam].isParamsEdited[4] = true;
              //do all switch check here (might not be the greatest idea)
              //scaledEncoderValueParam = encoderValue%25;
      
              if((encoderDir == 1 && currentPreset.knubbies[currentParam].params[4] == 0) || (encoderDir == -1 && currentPreset.knubbies[currentParam].params[4] == 1)){
                  if (currentPreset.knubbies[currentParam].params[4] == 0)
                    currentPreset.knubbies[currentParam].params[4] = 1;
                  else currentPreset.knubbies[currentParam].params[4] = 0;
                  updateParam(2, stateToString(currentPreset.knubbies[currentParam].params[4]));
                  updateLoops(currentPreset.knubbies[currentParam].params[5], currentPreset.knubbies[currentParam].params[4]);
              }
      
              //update loop at loop indx
      
              if(!checkLoopsOut(currentPreset.knubbies[currentParam].params[5]))
                switchLoop(currentPreset.knubbies[currentParam].params[5], 0);//turn loop off
              else
                //quick and dirty
                switchLoop(currentPreset.knubbies[currentParam].params[5], 1);//turn loop on
              break;
            case 6:
              //checkEdition(isEdited);
              //currentPreset.knubbies[currentParam].isEdited = true;
              currentPreset.knubbies[currentParam].params[5] = true;
              scaledEncoderValueParam = encoderValue%25;
              if(scaledEncoderValueParam == 0){   
                txtParamIndx += encoderDir;
                currSwIndx = txtParamIndx%13;
                updateParam(7, switchOut[currSwIndx]);
              } 
              break;
            }
    }
  }
  lastValue = encoderValue;
}

ISR(TIMER2_OVF_vect) {  
  /* Reload the timer */
  TCNT2 = tcnt2;  

  bouncer1.update();
  if(bouncer1.risingEdge()){
    if (digitalRead(encoderPin2)){
      //Serial.println("CW");
      encoderDir = -1;
      encoderValue ++;
    } else {
      encoderDir = 1;
      encoderValue --;      
    }
  } 
}

