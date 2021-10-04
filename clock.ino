/* TW20FirmWare
 * 1. Keep Time
 * 2. i2C Transmit
 * 3. GSM send/receive
 * 4. Manage Power
 */


#include <EEPROM.h>
#include <Wire.h>
#include <SoftwareSerial.h>


SoftwareSerial SIM(2, 3); // RX, TX

void updateSerial();
unsigned long track_time(); 

void updateSerial(){  delay(500);
char read_time[255]; int i=0;

  while (Serial.available()){SIM.write(Serial.read()); } //Forward what Serial received to Software Serial Port
  
  while (SIM.available() && i<255)   { Serial.write(SIM.read());  read_time[i] = (char)SIM.read();  i++;}//Forward what Software Serial received to Serial Port
  Serial.println(read_time);
  }    

void pick_time(){
  //if(already_sent == false){
                    SIM.println("AT"); //Initiate data transmission when a certain number of handwashs has been bundled up
                      //updateSerial();
       
                    SIM.println("AT+CCLK?"); // +CCLK: "04/01/01,00:14:12+22"
                      //updateSerial();

                    SIM.println("AT+CLTS=1"); // Enable auto network time sync : 
                      //updateSerial();

                    SIM.println("AT+CLTS?"); // Check if value is set : 
                      //updateSerial();

                    SIM.println("AT&W"); // Save the setting to permanent memory so that module enables sync on restart also  :  
                      //updateSerial();

                    SIM.println("AT+CCLK?"); // Read current Time
                      updateSerial();
            
                   
                    SIM.write(26);

}


void readtext(){

char response[255];
int i=0;
SIM.print("AT+CCLK?");
delay(1000);

while(SIM.available()!=0 && i<255){
  response[i] = (char)SIM.read();
  i++;
}

Serial.println(response);

if( strstr( response , "LoooooL" ) )  Serial.println("there");
}


uint8_t sound_fx = 25;

// -- clock code
uint8_t pulsePin = 10;
unsigned int pulse;

uint8_t second=0, minute=0, hour=0, day=1, month=1, Y1=20, Y2=21; 
uint16_t year = 0;
const String zero = "0"; /*char Month[8] = "Null";*/ String Month;
uint8_t time_slot = EEPROM.read(1000);; 


void setup() {
//Wire.begin(); // join i2c bus (address optional for master)
Serial.begin(9600);
SIM.begin(9600);

//pick_time(); delay(2000);

/*
//update the system clock according to the GSM Nwtwork time
hour = EEPROM.read(time_slot);
minute = EEPROM.read(time_slot+1);
second = EEPROM.read(time_slot+2);

day = EEPROM.read(time_slot+3);
month=EEPROM.read(time_slot+4);
Y1 = EEPROM.read(time_slot+5); Y2 = EEPROM.read(time_slot+6);
year = 100*Y1 + Y2;
*/
}


char Date[15];
void loop() {


readtext(); 
delay(2000);
Serial.println();
  //track_time();
/*
for(int i=0; i<20; i++){
  Date[i] = day + ", " + month + year;
}

  Wire.beginTransmission(8); // transmit to device #8
   
for(int i = 0; i<20; i++){
  Wire.write(Date[i]); Wire.write(", "); Wire.write(month); 
}
  Wire.endTransmission();    // stop transmitting
*/
  
  //Wire.beginTransmission(8); // transmit to device #8
  //Wire.write(hour + ":" + minute); 
  //Wire.endTransmission();
  
}







bool locked = false, first_pass_on = false, first_pass_off = false; // tik tok lockers
char Time_Prompt[4][5];




unsigned long track_time(){


// --- clock management first
  if(second == 60){ minute++; second = 0; }
  if(minute == 60){ hour++; minute = 0; }
  if(hour == 24)  { day++; hour = 0; time_slot++; EEPROM.update(1000, time_slot); } 
      //since seconds in a day change 86,400 times ... and EEPROM dies at 100,000 writes ... 
      // ||  SHIFT ONE CELL RIGHT || \\



// --- calendar --- //
    if(month == 2){ 
        if(year%4 != 0){if(day == 29){day=0; month++;}} // lunar
        else {if(day == 30){ day=1; month++; }} // leap year
      }
  else  if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12){
        if(day == 31){month++; day = 1;}
    }
  else { //month == April || month == June || month == September || month == November
    if(day == 32){month++; day=1;}
    }

  if(month == 13){year++; month = 1; }

    // shift to the display class{};
      if(month==1) Month = "January";    else if(month==2) Month = "February"; else if(month==3) Month = "March";
 else if(month==4) Month = "April";      else if(month==5) Month = "May";      else if(month==6) Month = "June";
 else if(month==7) Month = "July";       else if(month==8) Month = "August";   else if(month==9) Month = "September";
 else if(month==10)Month = "October";    else if(month==11) Month = "November";     else if(month==12) Month = "December"; 
//if day has changed 

  
   pulse =  digitalRead(pulsePin);


     if(pulse == HIGH && locked == false){  locked = true; first_pass_off = true; }
else if(pulse == LOW) {                     locked = false; first_pass_on = true; }

     if( locked && first_pass_on == true){ first_pass_on = false; second++; 
     EEPROM.update(time_slot, hour); EEPROM.update((time_slot+1), minute); EEPROM.update((time_slot+2), second); // -- update EEPROM internal system time every time a second changes
     EEPROM.update((time_slot+3), day);  EEPROM.update((time_slot+4), month);  EEPROM.update((time_slot+6), 21); // update EEPROM internal system calendar every time a minute changes
   Serial.print(day);  Serial.print("th ");  Serial.print(Month);  Serial.print(",  "); Serial.println(year);
   
  Serial.print(hour); Serial.print(" : "); Serial.print(minute);  Serial.print(" : ");  Serial.println(second);
  Serial.println(); Serial.println();
     }

     
else if(!locked && first_pass_off == true) {first_pass_off = false; second++;
     EEPROM.update(time_slot, hour); EEPROM.update((time_slot+1), minute); EEPROM.update((time_slot+2), second); 
     EEPROM.update((time_slot+3), day);  EEPROM.update((time_slot+4), month);  EEPROM.update((time_slot+6), 21);

  Serial.print(day);  Serial.print("th ");  Serial.print(Month);  Serial.print(",  "); Serial.println(year);
  
  Serial.print(hour); Serial.print(" : "); Serial.print(minute);  Serial.print(" : ");  Serial.println(second);
  Serial.println(); Serial.println();
     } 
   

// --- 212 loop cycles in just one second --- //
      unsigned long current_time_in_secs = 0;

  current_time_in_secs = (long(3600) * hour) + (long(60) * minute) + (second);
//  current_time_in_secs = (minute * 60) + (second);
//  current_time_in_secs = second;

return current_time_in_secs;

  }
