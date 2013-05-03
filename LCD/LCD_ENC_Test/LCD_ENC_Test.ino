
#define encoderPin1 5
#define encoderPin2 6

volatile int lastEncoderValue = 0;
volatile long encoderValue = 0;

uint8_t lastMSB = 0;
uint8_t lastLSB = 0;

void setup() {
  
  
  Serial.begin (9600);

  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);

  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);

}

void loop(){ 
  //Do stuff here

  Serial.println(encoderValue);
  delay(10); 
}

void updateEncoder(){
  
  uint8_t MSB = digitalRead(encoderPin1); //MSB = most significant bit
 uint8_t LSB = digitalRead(encoderPin2); //LSB = least significant bit

  uint8_t encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  uint8_t sum  = (lastEncoderValue << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoderValue = encoded; //store this value for next time
}
