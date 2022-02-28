
#include <SoftwareSerial.h>

#include <EEPROM.h>

#define Kit_ID 37

SoftwareSerial mySerial(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2

String Kit_Location = "Kasubi Market II";


#define LED 9

int wash_signal = 4;

int addr=0; int hw1; int hw2; int hw3; int hw4;

int day1; int day2; int day3; int day4;

int c1; int c2; int c3; int c4;
int c5; int c6; int c7;

int x1; int x2; int x3; int x4;
int x5; int x6; int x7; int x8;
int x9;




int w_state=1;
int lock1=0;
int lock2=0;


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

pinMode(wash_signal, INPUT);
pinMode(LED, OUTPUT);


delay(500);
digitalWrite(LED,1);
delay(100);
digitalWrite(LED,0);
delay(100);
digitalWrite(LED,1);
delay(100);
digitalWrite(LED,0);

   addr=0;
  hw1 = EEPROM.read(addr);
  
   addr=1;
  hw2 = EEPROM.read(addr); 
  
    addr=2;
  hw3 = EEPROM.read(addr);
  
    addr=3;
  hw4 = EEPROM.read(addr);

    addr=4;
  day1 = EEPROM.read(addr);
 
   addr=5;
  day2 = EEPROM.read(addr); 
   
    addr=6;
  day3 = EEPROM.read(addr);
    
    addr=7;
  day4 = EEPROM.read(addr);

   addr=8;
  c1 = EEPROM.read(addr);

   addr=9;
  c2 = EEPROM.read(addr);
   
     addr=10;
  c3 = EEPROM.read(addr);
     
     addr=11;
  c4 = EEPROM.read(addr);
     
     addr=12;
  c5 = EEPROM.read(addr);
     
     addr=13;
  c6 = EEPROM.read(addr);
     
     addr=14;
  c7 = EEPROM.read(addr);

     addr=15;
  x1 = EEPROM.read(addr);

     addr=16;
  x2 = EEPROM.read(addr);
     
       addr=17;
  x3 = EEPROM.read(addr);
       
       addr=18;
  x4 = EEPROM.read(addr);
       
       addr=19;
  x5 = EEPROM.read(addr);
       
       addr=20;
  x6 = EEPROM.read(addr);
       
       addr=21;
  x7 = EEPROM.read(addr);
       
       addr=22;
  x8 = EEPROM.read(addr);
       
       addr=23;
  x9 = EEPROM.read(addr);

delay(2000);

}

void loop() {


if(analogRead(wash_signal)>=200 and lock1==0 ){


  digitalWrite(LED,1);
lock1=1;    hw1++;    c1++;

if(hw1==10){
      hw1=0; hw2++; 
}

if(hw2==10){
      hw2=0; hw3++; 
}

if(hw3==10){
      hw3=0; hw4++; 
}

if(hw4==10){
      hw1=0;  hw2=0;    hw3=0;  hw4=0; 
}

addr=0;EEPROM.update(addr, hw1);
addr=1;EEPROM.update(addr, hw2);
addr=2;EEPROM.update(addr, hw3);
addr=3;EEPROM.update(addr, hw4);

addr=4;EEPROM.update(addr, day1);
addr=5;EEPROM.update(addr, day2);
addr=6;EEPROM.update(addr, day3);
addr=7;EEPROM.update(addr, day4);


if(c1==10){
 c1=0; c2++; 
}

if(c2==10){
 c2=0; c3++; 
}

if(c3==10){
 c3=0; c4++; 
}

if(c4==10){
 c4=0; c5++; 
}

if(c5==10){
 c5=0; c6++; 
}

if(c6==10){
 c6=0; c7++; 
}
if(c7==10){
c1=0; c2=0; c3=0; c4=0; c5=0; c6=0; c7=0; 
}

addr=8;  EEPROM.update(addr, c1);
addr=9;  EEPROM.update(addr, c2);
addr=10; EEPROM.update(addr, c3);
addr=11; EEPROM.update(addr, c4);
addr=12; EEPROM.update(addr, c5);
addr=13; EEPROM.update(addr, c6);
addr=14; EEPROM.update(addr, c7);

digitalWrite(LED,0);




}

if(analogRead(wash_signal)<=10 and lock1==1 ){
lock1=0;  
}



 x1++;

if(x1==10){
 x1=0; x2++; 
}

if(x2==10){
 x2=0; x3++; 
}

if(x3==10){
 x3=0; x4++; 
}

if(x4==10){
 x4=0; x5++; 
}

if(x5==10){
 x5=0; x6++; 
}

if(x6==10){
 x6=0; x7++; 
}

if(x7==10){
 x7=0; x8++; 
}

if(x8==10){
 x8=0; x9++; 
}

if(x9==10){
 x1=0; x2=0;  x3=0; x4=0;
 x5=0; x6=0;  x7=0; x8=0;

}

addr=15;EEPROM.update(addr, x1);
addr=16;EEPROM.update(addr, x2);
addr=17;EEPROM.update(addr, x3);
addr=18;EEPROM.update(addr, x4);
addr=19;EEPROM.update(addr, x5);
addr=20;EEPROM.update(addr, x6);
addr=21;EEPROM.update(addr, x7);
addr=22;EEPROM.update(addr, x8);
addr=23;EEPROM.update(addr, x9);


if(x1==0){
  if(x2==0){ 
    if(x3==0){ 
      if(x4==0){
        if(x5==4){
          if(x6==6){
            if(x7==8){


        x1=0; x2=0;  x3=0; x4=0;
        x5=0; x6=0;  x7=0; x8=0;
 




//Serial.println("Initializing SIM Connection...");

                   mySerial.println("AT"); //Initiate data transmission when a certain number of handwashs has been bundled up
                      updateSerial();
                   

                    mySerial.println("AT+CMGF=1\r"); // Configuring TEXT mode
                      updateSerial();
            
                    mySerial.println("AT+CMGS=\"+256758564040\"");//Recipient number --- MasterLine
                      updateSerial();
  
                    mySerial.println(Kit_Location); mySerial.print("Kit No::"); mySerial.println(Kit_ID ); 
                    mySerial.print("Today's Hand-washes: => ");  mySerial.print(hw4);mySerial.print(hw3);mySerial.print(hw2);mySerial.println(hw1);
                    
                    mySerial.print("Cummulated washes:: "); mySerial.print(c7);mySerial.print(c6);mySerial.print(c5);mySerial.print(c4);mySerial.print(c3);mySerial.print(c2);mySerial.print(c1);



                      updateSerial();
  
                    mySerial.write(26);

digitalWrite(LED,1);
delay(100);
digitalWrite(LED,0); 
 


hw1=0;  hw2=0;  hw3=0;  hw4=0;

addr=0; EEPROM.update(addr, hw1);
addr=1; EEPROM.update(addr, hw2);
addr=2; EEPROM.update(addr, hw3);
addr=3; EEPROM.update(addr, hw4);

      delay(500);
      digitalWrite(LED,1);
      delay(100);
      digitalWrite(LED,0);
      delay(100);
      digitalWrite(LED,1);
      delay(100);
      digitalWrite(LED,0);



                 }
              }
           }
        }
     }
  }
}



}



void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());  //Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());  //Forward what Software Serial received to Serial Port
  }
}
