#include <MIDI.h>



void handleControlChange(byte channel, byte number, byte value){
  
  value = map(value, 0, 127, 0, 255);
  //deals with curves from value 1 to 2 here 
}

