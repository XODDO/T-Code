// --- Secondary Core --- //
#include <Wire.h>
volatile char buffer[40];
volatile boolean receiveFlag = false;

//void receive(int howMany){  Wire.readBytes(buffer, howMany); receiveFlag = true;}

#include <SPI.h>
#include <SD.h>
const int CS = A0;
File local_storage;
String dataFile = "data.txt";

//#include "DFRobotDFPlayerMini.h"

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"


SoftwareSerial mySoftwareSerial(10, 9); // RX, TX

DFRobotDFPlayerMini MP3;

byte Previous = A3;
byte PlayPause = A2;
byte Next = A1;

boolean isPlaying = false;

byte trigger = 7, echo = 8;
byte sopa = 5, wota = 6;

int flash = 3;
unsigned int x = 0;// complete_handwash ... 62k
unsigned int y = 0; // incomplete_handwash 62k

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Serial.begin(9600);           // start serial for output

pinMode(PlayPause, INPUT);//digitalWrite(buttonPause,HIGH);
pinMode(Next, INPUT);//digitalWrite(buttonNext,HIGH);
pinMode(Previous, INPUT);//digitalWrite(buttonPrevious,HIGH);

pinMode(sopa, OUTPUT); 
pinMode(flash, OUTPUT);

mySoftwareSerial.begin(9600);
//MP3.begin (9600);

//playFirst(); //boot sound
//isPlaying = true;


  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!MP3.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  
  MP3.volume(20);  //Set volume value. From 0 to 30
  
  
 pinMode(trigger, OUTPUT); // Sets the trigPin as an OUTPUT
 pinMode(echo, INPUT); // Sets the echoPin as an INPUT
 
 if (!SD.begin(CS)) {
    Serial.println("Sd Card initialization failed!");
    while (1);
  }
  Serial.println("Card initialization done.");

delay(100);
}


byte distance1 = 0, distance2 = 0;
bool sonar_disabled = false;

void loop() {
  /*** Cellular Network time synchronization
  * 1. look for the hands
  * 2. trigger soap pump
  * 3. play audio file
  * 4. trigger water pump 
  * 5. log data on EEPROM && SD Card
  * 6. Bind & Transmit Data when time is right
  * 7. communicate with the other IC
  ***/
  distance1 = 0; 
  
      distance1 = sonar(trigger, echo);

      if(distance1 == 0){ return; }
else{ 
      if(distance1 <= 15){analogWrite(flash, 254);} //Serial.print(" :: Too Close :: ");  //too near
else  if(distance1 >  30) {analogWrite(flash, 64);}//Serial.print(" :: Too far!");  // too far      
else  if(distance1 <= 30){ //15 - 30 cm
    //sonar_disabled = true;
    analogWrite(flash, 0);
                          Serial.print("MP3 State: "); Serial.println(MP3.readState());
      MP3.play(1);  //Play the first mp3
                          Serial.print("MP3 State: "); Serial.println(MP3.readState());
      //delay(100);
                          Serial.print("MP3 State: "); Serial.println(MP3.readState());
      trigger_soap(2000);  delay(4500);//range
     Serial.println("Soaping finished!");
    
      //1. call them back 
     //2. detect if they have come
        //3. if(they _have)trigger water
       //4 && log complete handwash
      //if not summon them back up to third time
     //log it as an incompleete handwash
      int callBack = 0;
      //while(callBack<3){callBack++; 
      MP3.play(2); //call them back
      
      int count = 0; distance2 = 0;
    while(count < 240){ Serial.println(count);
          distance2 = sonar(trigger, echo); Serial.print("Distance: "); Serial.println(distance2);
         if(distance2 >= 5 && distance2 <= 50) { Serial.println("Watering Now!");
           analogWrite(flash, 0);
           trigger_wota();
            x++; //abamaze
            log_handwash();
            break; 
           }

           count++;
       }
      // if(callBack>1)delay(100);
     //} // close call_back
        
    }
  
}

 


buttonScan();
  
  // Wire.onReceive(receiveEvent); // register event
  // delay(500);

}

bool logged = false;
bool log_handwash(){
      String Message;
      Serial.println("Saving Hand-wash");
  if(!SD.exists(dataFile)) {Serial.print(dataFile); Serial.println(" doesn't exist"); //dataFile = "/" + dataFile; //create it  
                             if(SD.mkdir(dataFile)) Serial.println("Created!"); else {Serial.println("Failed to create File: "); Serial.print(dataFile); }}
  
   local_storage = SD.open(dataFile, FILE_WRITE);

  // if the file opened wello wello, jiwandikemu:
  if (local_storage) {
    Serial.print("Writing to "); Serial.println(dataFile); 

        local_storage.println();
        local_storage.println("Kit: BTL001 2021");
        local_storage.println("Location: Badaye HQ");
        local_storage.println("Log Date: Sun 4 Oct, 2021");
        local_storage.println("Log Time: 12:12pm");

        local_storage.print("Complete Hand-washes: "); local_storage.println(x);
        local_storage.print("InComplete Hand-washes: "); local_storage.println(y);
        /*
    for(int i=0; i<dataPoints; i++){
      for(int j=0; j<2; j++){
          local_storage.print(binded_for_datalog[i][j]);
        }
        local_storage.println();
      }
    */
        local_storage.println();
        
        local_storage.close(); Message = "Handwash Logged!";
    Serial.println(Message);
        logged = true;

  } else {
    // if the file didn't open, print an error:
    Message = "Error Opening " + dataFile;
    Serial.println(Message);
    }

  return logged;
}


void trigger_soap(int duration){
  Serial.println("Releasing soap...");
  digitalWrite(sopa, HIGH); delay(duration); digitalWrite(sopa, LOW);
}

void trigger_wota(){
  Serial.println("Releasing Water...");
digitalWrite(wota, HIGH);
    delay(5000);//range
digitalWrite(wota, LOW);
}


unsigned long sonar(byte trigPin, byte echoPin){
//if(sonar_disabled) return 0;
//else{
// defines variables
long duration; // variable for the duration of sound wave travel
float distance; // variable for the distance measurement
 
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds

  digitalWrite(trigPin, HIGH);  delayMicroseconds(10);  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds

  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = 0.034 * duration / 2.00; // Speed of sound wave divided by 2 (go and back)

  // Displays the distance on the Serial Monitor
  //Serial.print("Distance: ");  Serial.print(distance);  Serial.println(" cm");

  return (long) distance;
//}
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


void buttonScan(){

  PlayPause = analogRead(PlayPause);
  Next = analogRead(Next);
  Previous = analogRead(Previous);

  if(PlayPause < 50){}
  if(Next < 50){MP3.playFolder(3, 2); }
  if(Previous<50){}  
  
}
  
