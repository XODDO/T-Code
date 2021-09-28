// --- Secondary Core --- //
#include <Wire.h>
volatile char buffer[40];
volatile boolean receiveFlag = false;

//void receive(int howMany){  Wire.readBytes(buffer, howMany); receiveFlag = true;}


void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Serial.begin(9600);           // start serial for output
}

void loop() {
  /*
  * 1. look for the hands
  * 2. trigger soap/water pumps
  * 3. play audio file
  * 4. trigger soap and water pumps 
  * 5. log data on EEPROM && SD Card
  * 6. Bind & Transmit Data when time is right
  * 7. communicate with the other IC
  */
  
  
  
  
  
  Wire.onReceive(receiveEvent); // register event
  delay(500);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  //int x = Wire.read();    // receive byte as an integer
  //Serial.println(x);         // print the integer
}
