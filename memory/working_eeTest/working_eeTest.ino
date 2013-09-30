 #include <Wire.h> //I2C library



  

  // WARNING: address is a page address, 6-bit end will wrap around
  // also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
  void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    byte c;
    for ( c = 0; c < length; c++)
      Wire.write(data[c]);
    Wire.endTransmission();
  }
// maybe let's not read more than 30 or 32 bytes at a time!
  void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) {
    
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    
    Wire.requestFrom(deviceaddress,length);
    int c = 0;
    for ( c = 0; c < length; c++ )
      if (Wire.available()) buffer[c] = Wire.read();
  }

void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
    //int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(data);
    Wire.endTransmission();
  }
  byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
 
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available());
    return Wire.read();
  }


  void setup() 
  {
    char somedata[] = "hello world"; // data to write
    
    Wire.begin(); // initialise the connection
    
    Serial.begin(9600);
    
    i2c_eeprom_write_byte(0x50, 0, 55);
    
    delay(10); //add a small delay

    Serial.println("Memory written");
  }

  void loop() 
  {
    int addr=0; //first address
    
    byte b = i2c_eeprom_read_byte(0x50, 0); // access the first address from the memory
    
    Serial.println(b);
    
    delay(2000);

  }

