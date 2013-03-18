/*
the first 16 memory addresses are reserved for things like lastPgmNum
*/

#include <SPI.h>
#include <Z_OSC.h>
#include <Ethernet.h>
#include <Wire.h>
#include <MIDI.h>

#include "memory.h"
#include "knubVars.h"
#include "knubFuncs.h"
#include "knubMidi.h"
#include "knubOSC.h"

void setup(){

  Wire.begin();
  
  Ethernet.begin(myMac, myIp);
  server.sockOpen(myPort);
  
  MIDI.begin(1);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleProgramChange(handleProgramChange);
  
  
}


void loop(){
   
  knubDoOsc();
  
}



