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

byte pageLevel = 0;
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

void setup(){
  
  baseID = 5;

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
  //lastID = 5; //TODO: understand && remove ?
  pageLevel = 2;
  lastID = 5;
  readAdr = 5;
  readKnubPreset(eepromAddr1, ((lastID-baseID) * presetSize)+baseID, &currentPreset);
  //printCurrentPreset();
  updateKnubs(&currentPreset);
  updateLoopsOut(&currentPreset);

  //startUp sequence
  productPage();
  softwareVersion();
  presetPage();

  updatePreset(currentPreset.name, isPresetEdited() );
  checkUILeds();

  //???: usefull ?
  pinMode(upPin, INPUT);
  pinMode(downPin, INPUT);
  digitalWrite(upPin, HIGH);
  digitalWrite(downPin, HIGH);

  prevUp = digitalRead(upPin);
  prevDown = digitalRead(downPin);
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
    switch(pageLevel){
    case 2: //Preset Page
      tabIndx = 0;
      presetPage();
      updatePreset(currentPreset.name, isPresetEdited());
      checkUILeds();
      break;
    case 3: //Knubbie Page
      tabIndx = 0;
      currentParam = 0;
      knubbiePage(currentParam, currentPreset,modOns, switchOut);
      break;
    case 4: //Edited Knubbie Page 
      //time to save things.
      savePage();
      //writeKnubPreset(eepromAddr1, readAdr, &currentPreset);
      saveEditedKnubbies(eepromAddr1, readAdr, &currentPreset);
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
    case 2: //Preset Page
      if(bValid.clicks == 2){ //TODO: replace by a long click ?
        pageLevel ++;
        time2ChangePage = true;
      } else if (bValid.clicks==1){
        //increment preset and load
        if(readindx < 12){
          readindx += 1;
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
      if(bckValid.clicks == 2 && isPresetEdited()){
        pageLevel = 4;
        for (int i = 0; i < 8; i++)
          currentPreset.knubbies[i].isEdited = false;
        time2ChangePage = true;
      } else if (bckValid.clicks == 1){
        if(readindx > 5){
          readindx -=1;
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
      break;
    case 3:
      if(bckValid.clicks == 1){
        Serial.println(tabIndx);
        prevIndx = tabIndx;
        if (tabIndx == 0)
          tabIndx = sizeof(chParamTabs)+1;
        tabIndx--;
        tabIndx = tabIndx%(sizeof(chParamTabs)+1);
        tab(chParamTabs[tabIndx]);

        customCursor(tabIndx, pageLevel, prevIndx);
      } else if (bckValid.clicks == 2){

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

          knubbiePage(currentParam, currentPreset,modOns, switchOut);
        }
        break;
      case 1:

        ///EDITED

        //checkEdition(isEdited);
        currentPreset.knubbies[currentParam].isEdited = true;
        ///MUST FIND A BETTER WAY OF DEALING WITH THIS

        currentParamVal = currentPreset.knubbies[currentParam].params[0];

        if ((currentParamVal>0 && currentParamVal<255) || (currentParamVal== 0 && encoderDir ==1) || (currentParamVal== 255 && encoderDir ==-1))
          currentParamVal += encoderDir;
          
        currentPreset.knubbies[currentParam].params[0] = currentParamVal;
        updateNumParam(4,map(currentPreset.knubbies[currentParam].params[0], 0, 255, 0, 100));
        turnKnub(currentParam, currentPreset.knubbies[currentParam].params[0]);
        break;
        
      case 2:
        currentPreset.knubbies[currentParam].isEdited = true;
        //checkEdition(isEdited);

        currentParamVal = currentPreset.knubbies[currentParam].params[1];

        if((currentParamVal>0 && currentParamVal<256) || (currentParamVal == 0 && encoderDir == 1) || (currentParamVal == 255 && encoderDir == -1))
          currentParamVal += encoderDir;
          
        currentParamVal = currentPreset.knubbies[currentParam].params[1] = currentParamVal;
        updateNumParam(5, map(currentParamVal, 0, 255, 0, 100));
    
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
        //checkEdition(isEdited);
        currentPreset.knubbies[currentParam].isEdited = true;
        scaledEncoderValueParam = encoderValue%25;
        if(scaledEncoderValueParam == 0){
          txtParamIndx += encoderDir;
          currModIndx = txtParamIndx%3;

          updateParam(3, modOns[currModIndx]);
        } 
        break;
      case 5:
        //checkEdition(isEdited);
        currentPreset.knubbies[currentParam].isEdited = true;

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
        //checkEdition(isEdited);
        currentPreset.knubbies[currentParam].isEdited = true;
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
