#include <SPI.h>
#include <Z_OSC.h>

#include <MIDI.h>
#include <Wire.h>
#include <Ethernet.h>


#define NO_DEBUG 

#ifdef NO_DEBUG
   #define minSend 1381
   #define maxSend 1557
#endif



#ifdef DEBUG
#define minSend 0 
#define maxSend 4095
#endif

#define ledPin 4

boolean doMidi;
boolean doOSC;



byte myMac[] = { 11, 22, 33, 44, 55};
byte myIp[]  = { 192, 168, 0, 8 };
int  myPort  = 10000;


char *subAddress[6]={"/k1", "/k2", "/k3", "/k4", "/k5", "/k6"};

Z_OSCServer server;

Z_OSCMessage *rcvMes;



byte greenLookUp[256] = {9 , 61 , 136 , 138 , 139 , 141 , 142 , 144 , 145 , 146 , 147 , 148 , 149 , 150 , 151 , 
152 , 153 , 154 , 156 , 157 , 158 , 159 , 160 , 161 , 162 , 164 , 165 , 165 , 166 , 167 , 168 , 168 , 169 , 170 , 
171 , 171 , 172 , 173 , 173 , 174 , 174 , 174 , 175 , 175 , 176 , 176 , 177 , 177 , 177 , 178 , 178 , 179 , 179 , 
179 , 180 , 180 , 180 , 181 , 182 , 182 , 183 , 183 , 184 , 184 , 185 , 185 , 186 , 186 , 187 , 187 , 188 , 188 , 
188 , 189 , 189 , 190 , 190 , 191 , 191 , 191 , 191 , 191 , 191 , 192 , 192 , 192 , 192 , 192 , 193 , 193 , 193 , 
193 , 194 , 194 , 194 , 195 , 195 , 195 , 195 , 195 , 195 , 195 , 196 , 196 , 196 , 196 , 196 , 197 , 197 , 197 , 
197 , 197 , 197 , 198 , 198 , 198 , 198 , 198 , 198 , 199 , 199 , 200 , 200 , 200 , 200 , 201 , 201 , 201 , 202 , 
202 , 202 , 203 , 203 , 203 , 204 , 204 , 204 , 205 , 205 , 205 , 205 , 205 , 206 , 206 , 206 , 207 , 207 , 207 , 
207 , 208 , 208 , 208 , 208 , 209 , 209 , 209 , 210 , 210 , 210 , 210 , 211 , 211 , 211 , 212 , 212 , 212 , 213 , 
213 , 213 , 214 , 214 , 214 , 215 , 215 , 215 , 215 , 216 , 216 , 216 , 217 , 217 , 217 , 217 , 217 , 218 , 218 , 
218 , 219 , 219 , 219 , 220 , 220 , 220 , 221 , 221 , 221 , 222 , 222 , 223 , 223 , 224 , 224 , 225 , 225 , 225 , 
226 , 226 , 226 , 227 , 227 , 227 , 228 , 228 , 228 , 229 , 229 , 230 , 230 , 230 , 231 , 231 , 231 , 232 , 232 , 
233 , 233 , 233 , 234 , 234 , 235 , 235 , 236 , 236 , 237 , 237 , 238 , 238 , 238 , 239 , 239 , 240 , 240 , 241 , 
241 , 242 , 242 , 243 , 244 , 245 , 245 , 246 , 247 , 247 , 248 , 251 , 255};


byte redLookUp[256] = {9 , 103 , 107 , 109 , 112 , 115 , 116 , 118 , 121 , 121 , 123 , 124 , 125 , 127 , 127 , 130 , 
130 , 131 , 132 , 133 , 134 , 134 , 136 , 136 , 137 , 138 , 138 , 139 , 139 , 140 , 141 , 142 , 142 , 143 , 144 , 145 , 
145 , 146 , 146 , 147 , 147 , 148 , 148 , 149 , 149 , 150 , 151 , 151 , 152 , 152 , 153 , 153 , 154 , 154 , 154 , 155 , 155 , 
156 , 157 , 157 , 158 , 158 , 158 , 159 , 159 , 160 , 160 , 161 , 161 , 161 , 162 , 162 , 162 , 162 , 163 , 163 , 163 , 164 , 
164 , 164 , 165 , 165 , 166 , 166 , 166 , 167 , 167 , 168 , 168 , 168 , 168 , 168 , 169 , 169 , 169 , 169 , 170 , 170 , 171 , 
171 , 172 , 172 , 172 , 173 , 173 , 174 , 174 , 174 , 175 , 175 , 176 , 176 , 177 , 177 , 177 , 177 , 178 , 178 , 179 , 179 , 
179 , 180 , 180 , 180 , 180 , 181 , 181 , 181 , 182 , 182 , 182 , 182 , 183 , 183 , 183 , 184 , 184 , 184 , 185 , 185 , 186 , 
186 , 186 , 187 , 187 , 188 , 188 , 188 , 189 , 189 , 189 , 189 , 189 , 190 , 190 , 191 , 191 , 191 , 191 , 191 , 192 , 192 , 
192 , 193 , 193 , 194 , 194 , 194 , 195 , 195 , 195 , 196 , 196 , 197 , 197 , 197 , 198 , 198 , 198 , 199 , 199 , 200 , 200 ,
200 , 201 , 201 , 202 , 203 , 203 , 204 , 205 , 205 , 206 , 206 , 206 , 207 , 207 , 208 , 208 , 209 , 209 , 209 , 210 , 210 , 
211 , 211 , 212 , 212 , 212 , 213 , 213 , 214 , 214 , 215 , 216 , 217 , 217 , 218 , 218 , 218 , 220 , 220 , 221 , 221 , 222 ,
223 , 224 , 224 , 225 , 226 , 227 , 227 , 228 , 228 , 229 , 230 , 231 , 231 , 232 , 233 , 234 , 236 , 237 , 238 , 239 , 239 , 
240 , 242 , 243 , 244 , 245 , 246 , 248 , 249 , 251 , 255};


///MIDI CC function
///save those in progmem 
byte cc2knobs[] = {12, 13, 14, 15, 16, 17};


//DAC communication

byte write_cmds[] = {B01011000, B01011010, B01011100, B01011110};

byte dacIDZ[] = {B1100000, B1100001, B1100010}; 

byte knob1_ch1  = B01000000;
byte knob1_ch2 = B01000010;

byte knob2_ch1 = B01000100;
byte knob2_ch2 = B01000110;

///function for multiWrite : 

void multiWriteDac(byte addr, byte wrid, byte wrid2, int val, int val2){
  
  Wire.beginTransmission(addr);
  Wire.write(wrid);
  Wire.write(highByte(val));
  Wire.write(lowByte(val)); 
  Wire.write(wrid2);
  Wire.write(highByte(val2));
  Wire.write(lowByte(val2)); 
  Wire.endTransmission();
};

//function for SingleWrite : 

void writeDac(byte addr, byte wrid, int val){
  
  Wire.beginTransmission(addr);
  Wire.write(wrid);
  Wire.write(highByte(val));
  Wire.write(lowByte(val)); 
  Wire.endTransmission();
};




void setup(){

  Wire.begin();
  MIDI.begin(MIDI_CHANNEL_OMNI);

  MIDI.setHandleControlChange(HandleControlChange);
  
  Ethernet.begin(myMac, myIp);
  server.sockOpen(myPort);
 
  doMidi = false;
  doOSC = true;
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  #ifdef DEBUG
  Serial.begin(9600);
  #endif



}


void loop(){
 
 if(doOSC){
   
   if(server.available()){
  
    rcvMes = server.getMessage();
   
   //deals with pot1
    if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[0])){
        
        #ifdef DEBUG
          Serial.print(rcvMes->getZ_OSCAddress());
          Serial.print(" ");
          Serial.println(rcvMes->getInteger32(0));
        #endif
        
        
        
        int potval = rcvMes->getInteger32(0);
        int v1 = map(greenLookUp[potval], 0, 255, maxSend, minSend);
        int v2 = map(greenLookUp[255-potval], 0, 255, maxSend, minSend);
  
    multiWriteDac(dacIDZ[2], knob1_ch1, knob1_ch2, v1, v2);
       
    } 
    //deals with pot2
    else if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[1])){
          
         #ifdef DEBUG
          Serial.print(rcvMes->getZ_OSCAddress());
          Serial.print(" ");
          Serial.println(rcvMes->getInteger32(0));
        #endif
      
        int potval = rcvMes->getInteger32(0);
        int v1 = map(greenLookUp[potval], 0, 255, maxSend, minSend);
        int v2 = map(greenLookUp[255-potval], 0, 255, maxSend, minSend);
  
    multiWriteDac(dacIDZ[2], knob2_ch1, knob2_ch2, v1, v2);
       
    }
   //deals with pot3
    else if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[2])){
        
         #ifdef DEBUG
          Serial.print(rcvMes->getZ_OSCAddress());
          Serial.print(" ");
          Serial.println(rcvMes->getInteger32(0));
        #endif
      
      
        int potval = rcvMes->getInteger32(0);
        int v1 = map(greenLookUp[potval], 0, 255, maxSend, minSend);
        int v2 = map(greenLookUp[255-potval], 0, 255, maxSend, minSend);
  
    multiWriteDac(dacIDZ[1], knob1_ch1, knob1_ch2, v1, v2);
       
    }    
    //deals with pot4
    else if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[3])){
        
         #ifdef DEBUG
          Serial.print(rcvMes->getZ_OSCAddress());
          Serial.print(" ");
          Serial.println(rcvMes->getInteger32(0));
        #endif
      
      
      
        int potval = rcvMes->getInteger32(0);
        int v1 = map(greenLookUp[potval], 0, 255, maxSend, minSend);
        int v2 = map(greenLookUp[255-potval], 0, 255, maxSend, minSend);
  
    multiWriteDac(dacIDZ[1], knob2_ch1, knob2_ch2, v1, v2);
       
    }    
    //deals with pot5
    else if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[4])){
        
         #ifdef DEBUG
          Serial.print(rcvMes->getZ_OSCAddress());
          Serial.print(" ");
          Serial.println(rcvMes->getInteger32(0));
        #endif
      
      
      
        int potval = rcvMes->getInteger32(0);
        int v1 = map(greenLookUp[potval], 0, 255, maxSend, minSend);
        int v2 = map(greenLookUp[255-potval], 0, 255, maxSend, minSend);
  
    multiWriteDac(dacIDZ[0], knob1_ch1, knob1_ch2, v1, v2);
       
    }    
    //deals with pot6
    else if(!strcmp(rcvMes->getZ_OSCAddress(), subAddress[5])){
        
         #ifdef DEBUG
          Serial.print(rcvMes->getZ_OSCAddress());
          Serial.print(" ");
          Serial.println(rcvMes->getInteger32(0));
        #endif
      
      
      
        int potval = rcvMes->getInteger32(0);
        int v1 = map(greenLookUp[potval], 0, 255, maxSend, minSend);
        int v2 = map(greenLookUp[255-potval], 0, 255, maxSend, minSend);
  
    multiWriteDac(dacIDZ[0], knob2_ch1, knob2_ch2, v1, v2);
       
      }    
    }
  }
}

void HandleControlChange(byte channel, byte number, byte val){
  
 byte value = map(val, 0, 127, 0, 254);
  
  
  ///third DAC  
  if(number == cc2knobs[0]){
    //blinkSpeed = map(value, 0, 255, 10, 1000);
    int v1 = map(greenLookUp[value], 0, 255, maxSend, minSend);
    int v2 = map(greenLookUp[255-value], 0, 255, maxSend, minSend);
  
    multiWriteDac(dacIDZ[2], knob1_ch1, knob1_ch2, v1, v2);
  };
  
  if(number == cc2knobs[1]){
    
    int v1 = map(greenLookUp[value], 0, 255, maxSend, minSend);
    int v2 = map(greenLookUp[255-value], 0, 255, maxSend, minSend);
  
    multiWriteDac(dacIDZ[2], knob2_ch1, knob2_ch2, v1, v2);
  };

  ///second DAC
  if(number == cc2knobs[2]){
    
    int v1 = map(greenLookUp[value], 0, 255, maxSend, minSend);
    int v2 = map(greenLookUp[255-value], 0, 255, maxSend, minSend);
  
    multiWriteDac(dacIDZ[1], knob1_ch1, knob1_ch2, v1, v2);
  };
  if(number == cc2knobs[3]){
    
    int v1 = map(greenLookUp[value], 0, 255, maxSend, minSend);
    int v2 = map(greenLookUp[255-value], 0, 255, maxSend, minSend);
  
    multiWriteDac(dacIDZ[1], knob2_ch1, knob2_ch2, v1, v2);
  };

  ///first DAC
  if(number == cc2knobs[4]){
    
    int v1 = map(redLookUp[value], 0, 255, maxSend, minSend);
    int v2 = map(redLookUp[255-value], 0, 255, maxSend, minSend);
  
    multiWriteDac(dacIDZ[0], knob1_ch1, knob1_ch2, v1, v2);
  };
  if(number == cc2knobs[5]){
    
    int v1 = map(redLookUp[value], 0, 255, maxSend, minSend);
    int v2 = map(redLookUp[255-value], 0, 255, maxSend, minSend);
  
    multiWriteDac(dacIDZ[0], knob2_ch1, knob2_ch2, v1, v2);
  };
}



