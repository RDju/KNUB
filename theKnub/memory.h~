
#define eepromAddr1 = B01010000;

void writeSingleKnub(byte device, uint16_t address, byte knubValue){

  
  Wire.beginTransmission((device << 1)+0);
  Wire.send(lowByte(address));
  Wire.send(highByte(address));
  Wire.send(knubValue);
  Wire.endTransmission();

};

void writeKnubPreset( byte device, uint16_t address, byte *preset){

  Wire.beginTransmission((device << 1)+0);
  Wire.send(lowByte(address));
  Wire.send(highByte(address));
  
  for(byte c = 0; c<8; c++){
  
    Wire.send(preset[c]);
  
  }
  Wire.endTransmission();
  


}

byte readSingleKnub(byte device, uint16_t address){

  byte rval;
  
  Wire.beginTransmission((device << 1)+1);
  Wire.send(lowByte(address));
  Wire.send(highByte(address));
  Wire.endTransmission();

   Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rVal = Wire.receive();
  return rdata;
  
}


void readKnubPreset(byte device, uint16_t address, byte *preset){

  Wire.beginTransmission((device << 1)+1);
  Wire.send(lowByte(address));
  Wire.send(highByte(address));
  Wire.endTransmission();
  
   Wire.requestFrom(address,8);
    
    for (byte  c = 0; c < 8; c++ )
      if (Wire.available()) preset[c] = Wire.receive();

}


