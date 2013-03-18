
void writeSingleKnub(int device, unsigned int address, byte knubValue){

  
  Wire.beginTransmission(device);
  Wire.write(int(address >> 8));
  Wire.write(int(address & 0xFF));
  Wire.write(knubValue);
  Wire.endTransmission();

  delay(5);
}

byte readSingleKnub(int device, unsigned int address){

  byte rVal;
  
  Wire.beginTransmission(device);
   Wire.write(int(address >> 8));
  Wire.write(int(address & 0xFF));
  Wire.endTransmission();

   Wire.requestFrom(device,1);
    if (Wire.available()) rVal = Wire.read();
  return rVal;
  delay(5);
}



void writeKnubPreset( int device,unsigned int address, byte *preset){

  Wire.beginTransmission(device);
  Wire.write(int(address >> 8));
  Wire.write(int(address & 0xFF));
  
  for(byte c = 0; c<8; c++){
  
    Wire.write(preset[c]);
  
  }
  Wire.endTransmission();
  delay(5);
}
void readKnubPreset(int device, unsigned int address, byte *preset){

  Wire.beginTransmission(device);
  Wire.write(int(address >> 8));
  Wire.write(int(address & 0xFF));
  Wire.endTransmission();
  
   Wire.requestFrom(address,8);
    
    for (byte  c = 0; c < 8; c++ )
      if (Wire.available()) preset[c] = Wire.read();
  delay(5);
}


