
//Digital caliper code to read the value off of a cheap set of digital calipers
//By Making Stuff Youtube channel https://www.youtube.com/c/makingstuff
//This code is open source and in the public domain.

const byte clockPin = 2;  //attach to clock pin on calipers
const byte dataPin = 3; //attach to data pin on calipers

//Milliseconds to wait until starting a new value
//This can be a different value depending on which flavor caliper you are using.
const int cycleTime = 32; 

unsigned volatile int clockFlag = 0; 

long now = 0;
long lastInterrupt = 0;
long value = 0;

float finalValue = 0;
float previousValue = 0;

int newValue = 0;
int sign = 1;
int currentBit = 1;

void setup() {
  Serial.begin(115200);
 
  pinMode(clockPin, INPUT);  
  pinMode(dataPin, INPUT); 
  
  //We have to take the value on the RISING edge instead of FALLING
  //because it is possible that the first bit will be missed and this
  //causes the value to be off by .01mm.
  attachInterrupt(digitalPinToInterrupt(clockPin), clockISR, RISING);
}

void loop() {  
  
  if(newValue) 
  {
   if(finalValue != previousValue) {
     previousValue = finalValue;
     Serial.println(finalValue,2);     
   }
   newValue = 0;
  }
  
 //The ISR Can't handle the arduino command millis()
 //because it uses interrupts to count. The ISR will 
 //set the clockFlag and the clockFlag will trigger 
 //a call the decode routine outside of an ISR.
 if(clockFlag == 1)
 {
  clockFlag = 0;
  decode();
 } 
  
}

void decode(){
   unsigned char dataIn;
   dataIn = digitalRead(dataPin); 
   
   now = millis();
   
   if((now - lastInterrupt) > cycleTime)
   {
     finalValue = (value * sign) / 100.00;
     currentBit = 0;
     value = 0;
     sign = 1;
     newValue = 1;      
   } 
   else if (currentBit < 16 )
   {
      
     if (dataIn == 0)
     {
       if (currentBit < 16) {
          value |= 1 << currentBit;
       }  
       else if (currentBit == 20) {
          sign = -1;
       }
               
     }
     
     currentBit++;
     
   }
   
   lastInterrupt = now;
   
}

void clockISR(){
 clockFlag = 1; 
}





