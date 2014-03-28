#include <Wire.h>

#define minVal 0
#define maxVal 65535

byte DAC1_addr = B0001100;
byte writeDacA = B00110001;
byte writeDacB = B00110010;
byte writeDacC = B00110100;
byte writeDacD = B00111000;
 
byte writeCommands[4] = {writeDacA, writeDacB,writeDacC, writeDacD};

void setup(){
    
  Wire.begin();

}

void loop(){
  
  doDac(0, maxVal);
  doDac(1, maxVal);
  delay(500);
  doDac(0, minVal);
  doDac(1, minVal);
  delay(500);
}

void writeDac(int wichDac, int value){

    Wire.beginTransmission(DAC1_addr);
    Wire.write(writeCommands[wichDac]);
    Wire.write(highByte(value));
    Wire.write(lowByte(value));
    Wire.endTransmission();
}
