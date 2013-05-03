#define baseKnubCC 107

void handleControlChange(byte channel, byte number, byte value){
  
  value = map(value, 0, 127, 0, 255);
  
   if(number >= baseKnubCC && number < baseKnubCC + 8){
      turnKnub(number - 106, 1, value);
  }
}

void handleProgramChange(byte channel, byte number){

       readKnubPreset(eepromAddr1, pgmNum*memOffset, (byte *)knubsValues);
       setKnubPgm((byte *)knubsValues);
       ///saves last used pgm
       writeSingleKnub(eepromAddr1, 0, pgmNum);

}
