#include <ClickButton.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
//#include <SPI.h>
//#include <Ethernet.h> // version IDE 0022
//#include <Z_OSC.h>

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

//#define Do_OSC

volatile uint8_t lastValue = 0;
volatile uint8_t lastEncoderValue = 0;
volatile uint8_t encoderValue = 0;
volatile uint8_t encoderValueParam = 0;
volatile uint8_t encoderValueParamVal = 0;
volatile uint8_t scaledEncoderValueParam = 0;

//boolean time2ChangePage;

uint8_t txtParamIndx = 0;

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
char* modOns[3] = {"___", "EXP","MID"};
//Indiquer la localisation de la pédale
char* switchTypes[5] = {"L1", "L2", "L3", "L4","__"};//TODO: rename "switchOut"

uint16_t prevExpVal;
uint16_t expVal;

void setup(){

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

  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);
  digitalWrite(encoderPin1, HIGH);
  digitalWrite(encoderPin2, HIGH);
  attachInterrupt(0, updateEncoder, CHANGE); //???: why 2 interruptions ?
  attachInterrupt(1, updateEncoder, CHANGE); 

#ifdef Do_OSC 
  if(Ethernet.begin(myMac) ==0){

    //if doesn't work use fixed IP
    Ethernet.begin(myMac, myIp);

  }
  //OSC--------------------------------------
#endif


  //writeByte(eepromAddr1, lastPresetMemSpace, );
  //read last loaded ID and load that one
  lastID = 5;

  readKnubPreset(eepromAddr1, lastID * presetSize, &currentPreset);

  //delay(50);

  updateKnubs(&currentPreset);

  updateLoopsOut(&currentPreset);


  //startUp sequence
 // Serial.print((int)millis);
  (*drawFuncs[0])("", "", "", "", "", "", "", "", "");
 //productPage("", "", "", "", "", "", "", "", "");
 // Serial.print((int)millis);
  (*drawFuncs[1])("", "", "", "", "", "", "", "", "");
  //softwareVersion("", "", "", "", "", "", "", "", "");
  //initMemDisp();

  tabIndx = 0;
  pageLevel = 2;

  (*drawFuncs[2])("", "", "", "", "", "", "", "", "");

  updatePreset(currentPreset.name, isEdited);
  checkUILeds();

  pinMode(upPin, INPUT);
  pinMode(downPin, INPUT);
  digitalWrite(upPin, HIGH);
  digitalWrite(downPin, HIGH);


  prevUp = digitalRead(upPin);
  prevDown = digitalRead(downPin);

 // initEEPROM(); //TODO: put it in a distinct firmware

}

void loop(){

  if(pageLevel == 2){
    midiInRead();
    doSwitchInDec();
    doExpressionPedal(analogRead(expressionPin));
  }

  ////dealing with pages s
  if(time2ChangePage){
    time2ChangePage = false;
    //TODO: call of drawFuncs here ?
    switch(pageLevel){
    case 0: //Product Page
      (*drawFuncs[pageLevel])("", "", "", "", "", "", "", "", "");
      break;
    case 1: //Version Page
      (*drawFuncs[pageLevel])("", "", "", "", "", "", "", "", "");
      break;
    case 2: //Preset Page
      tabIndx = 0;
      (*drawFuncs[pageLevel])("", "", "", "", "", "", "", "", "");
      updatePreset(currentPreset.name, isEdited);
      checkUILeds();
      break;
    case 3: //Knubbie Page
      tabIndx = 0;
      currentParam = 0;
      clearScreen();

      //TODO : put in knubbiePage() in UI.h
      updateParam(0, toString(currentParam + 1));
      updateParam(1,currentPreset.knubbies[currentParam].name);
      updateParam(2,stateToString(currentPreset.knubbies[currentParam].state));
      updateParam(3,modOns[currentPreset.knubbies[currentParam].modOn]);
      updateNumParam(4,customDigits[currentPreset.knubbies[currentParam].params[0]]);
      updateNumParam(5,customDigits[currentPreset.knubbies[currentParam].params[1]]);  
      updateParam(7,switchTypes[currentPreset.knubbies[currentParam].numLoop]);
      break;
    case 4: //Knubbie Page //???: why knubbie page to save ?
      //time to save things.
      (*drawFuncs[pageLevel+1])("", "",  "",  "", "", "", "", "", "");
      writeKnubPreset(eepromAddr1, readAdr, &currentPreset);
      //delay(saveTime*5);//TODO: usefull ?
      pageLevel = 2;
      time2ChangePage = true;
      break;
    }
  }
  //////////////////////

  //// tab button
  bValid.Update();

  if(bValid.clicks !=0){
    switch (pageLevel){
    case 0: //Product Page
      if(bValid.clicks == 1){     
        pageLevel ++;
        time2ChangePage = true;
      }
      break;
    case 1: //Software Version
      if(bValid.clicks == 1){
        pageLevel ++;
        time2ChangePage = true;
      }
      break;
    case 2: //Preset Page
      if(bValid.clicks == 2){
        pageLevel ++;
        time2ChangePage = true;
      } else if (bValid.clicks==1){

        //increment preset and load
        if(readindx < 12){
          readindx += 1;
          readAdr = readindx*presetSize;
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
      break;

    case 3: //knubbie page
      if(bValid.clicks == 1){
        tabIndx++;
        tabIndx = tabIndx%numTabs[pageLevel];
        tab(chParamTabs[tabIndx]);
        customCursor(tabIndx, pageLevel);
      }
      break;

    case 4://knubbie page
      if(bValid.clicks == 2){
        pageLevel ++;
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

    case 2:
      if(bckValid.clicks == 2){

        pageLevel = 4;
        isEdited = false;
        time2ChangePage = true;
      } 
      else if (bckValid.clicks == 1){

        if(readindx > 5){
          readindx -=1;
          readAdr = readindx*presetSize;
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
      break;
    case 3:
      if(bckValid.clicks == 1){
        tabIndx--;
        tabIndx = tabIndx%numTabs[pageLevel];
        tab(chParamTabs[tabIndx]);

        customCursor(tabIndx, pageLevel);
      } 
      else if (bckValid.clicks == 2){

        pageLevel = 2;
        time2ChangePage = true;
      }
      break;
    }
  }


  //////////////////////////////////////////////

  /////// encoding Wheel/////////////////////

  if(encoderValue != lastValue){
    if (pageLevel == 3){
      switch(tabIndx){
      case 0:

        scaledEncoderValueParam = encoderValue%2;

        if ((scaledEncoderValueParam == 0 && encoderDir == 1 && currentParam < 7) || (scaledEncoderValueParam == 0 && encoderDir == -1 && currentParam > 0)){

          txtParamIndx += encoderDir;
          currentParam = txtParamIndx%8;

          clearScreen(); //TODO: try to delete
          
          //TODO: same as first switch case 3
          updateParam(0, toString(currentParam + 1));
          updateParam(1,currentPreset.knubbies[currentParam].name);
          updateParam(2,stateToString(currentPreset.knubbies[currentParam].state));
          updateParam(3,modOns[currentPreset.knubbies[currentParam].modOn]);
          updateNumParam(4,customDigits[currentPreset.knubbies[currentParam].params[0]]);
          updateNumParam(5,customDigits[currentPreset.knubbies[currentParam].params[1]]);
          //updateParam(6,customCurveDigits[currentPreset.knubbies[currentParam].params[2]]);    
          updateParam(7,switchTypes[currentPreset.knubbies[currentParam].numLoop]);
          //time2ChangePage = false;
        }
        break;
      case 1:

        ///EDITED

        checkEdition(isEdited);
        ///MUST FIND A BETTER WAY OF DEALING WITH THIS

        currentParamVal = currentPreset.knubbies[currentParam].params[0];

        if ((currentParamVal>0 && currentParamVal<255) || (currentParamVal== 0 && encoderDir ==1) || (currentParamVal== 255 && encoderDir ==-1))
          currentParamVal += encoderDir;
        /*else if(currentParamVal == 255 && encoderDir == 1) //already have these values
          currentParamVal  = 255;     
        else if(currentParamVal == 0 && encoderDir == -1)
          currentParamVal = 0;*/
          
        currentPreset.knubbies[currentParam].params[0] = currentParamVal;
        updateNumParam(4,customDigits[currentPreset.knubbies[currentParam].params[0]]);
        turnKnub(currentParam, currentPreset.knubbies[currentParam].params[0]);
        break;
        
      case 2:
        checkEdition(isEdited);

        currentParamVal = currentPreset.knubbies[currentParam].params[1];

        if((currentParamVal>0 && currentParamVal<256) || (currentParamVal == 0 && encoderDir == 1) || (currentParamVal == 255 && encoderDir == -1))
          currentParamVal += encoderDir;
          
        currentParamVal = currentPreset.knubbies[currentParam].params[1] = currentParamVal;
        updateNumParam(5, customDigits[currentParamVal]);
    
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
      case 4:
        checkEdition(isEdited);
        scaledEncoderValueParam = encoderValue%25;
        if(scaledEncoderValueParam == 0){
          txtParamIndx += encoderDir;
          currModIndx = txtParamIndx%3;

          updateParam(3, modOns[currModIndx]);
        } 
        break;
      case 5:
        checkEdition(isEdited);

        //do all switch check here (might not be the greatest idea)
        //scaledEncoderValueParam = encoderValue%25;

        if((encoderDir == 1 && currentPreset.knubbies[currentParam].state == 0) || (encoderDir == -1 && currentPreset.knubbies[currentParam].state == 1)){
            if (currentPreset.knubbies[currentParam].state == 0)
              currentPreset.knubbies[currentParam].state = 1;
            else currentPreset.knubbies[currentParam].state = 0;
            updateParam(2, stateToString(currentPreset.knubbies[currentParam].state));
            updateLoops(currentPreset.knubbies[currentParam].numLoop, currentPreset.knubbies[currentParam].state);
        }

        //update loop at loop indx

        //check loop at numLoop
        ////Serial.printl("checking loop: ");
        ////Serial.printlln(currentPreset.knubbies[currentParam].numLoop);
        ////Serial.printl("value: ");
        ////Serial.printlln(loopsOut[currentPreset.knubbies[currentParam].numLoop]);

        if(!checkLoopsOut(currentPreset.knubbies[currentParam].numLoop))
          switchLoop(currentPreset.knubbies[currentParam].numLoop, 0);//turn loop off
        else
          //quick and dirty
          switchLoop(currentPreset.knubbies[currentParam].numLoop, 1);//turn loop on
        break;
      case 6:
        checkEdition(isEdited);
        scaledEncoderValueParam = encoderValue%25;
        if(scaledEncoderValueParam == 0){   
          txtParamIndx += encoderDir;
          currSwIndx = txtParamIndx%13;
          updateParam(7, switchTypes[currSwIndx]);
        } 
        break;
      }
    }
  }
  lastValue = encoderValue;
}

//Interruption called each time a pin change of value
void updateEncoder(){
  uint8_t MSB = digitalRead(encoderPin1); //MSB = most significant bit
  uint8_t LSB = digitalRead(encoderPin2); //LSB = least significant bit

  uint8_t encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  uint8_t sum  = (lastEncoderValue << 2) | encoded; //adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011){
    encoderDir = 1;
    encoderValue ++;
  } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000){
    encoderDir = -1;
    encoderValue -- ;
  }
  lastEncoderValue = encoded; //store this value for next time
}
