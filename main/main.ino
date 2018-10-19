
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

volatile int pwm_value = 0;
volatile int oldPwm_value = 0;
volatile int prev_time = 0;
const byte interruptPin = 2;
int mp3Number = 1;
int oldMp3Number = 0;
boolean forceChange = false;
unsigned long microSecondsLastChange = 0;
unsigned long playedCount = 0;


SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  attachInterrupt(digitalPinToInterrupt(interruptPin), rising, RISING);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(4);  //Set volume value. From 0 to 30
  
}

void rising(){
  attachInterrupt(digitalPinToInterrupt(interruptPin), falling, FALLING);
  prev_time = micros();
}

void startUp(){
  myDFPlayer.play(1);  //Play the first mp3
}

void changeUp(){
  if(pwm_value<2400){
    mp3Number = 2;
    }
  if(pwm_value<2200){
    mp3Number = 3;
   }
  if(pwm_value<2000){
    mp3Number = 4;
   }
  if(pwm_value<1800){
    mp3Number = 5;
   }
  if(pwm_value<1600){
    mp3Number = 6;
  }
  if(pwm_value<1400){
    mp3Number = 7;
  }
  if(pwm_value<1200){
    mp3Number = 8;
  }

Serial.println(pwm_value);
playIfChange();
microSecondsLastChange = 0;
}

void changeDown(){
    if(pwm_value>1200){
    mp3Number = 9;
  }
    if(pwm_value>1600){
    mp3Number = 10;
  }
    if(pwm_value>1900){
    mp3Number = 11;
  }
  if(pwm_value>2200){
    mp3Number = 12;
   }


Serial.println(pwm_value);
playIfChange();
microSecondsLastChange = 0;
}


void playIfChange(){
  if((oldMp3Number != mp3Number) || forceChange == true ){
   forceChange == false;
   oldMp3Number = mp3Number;
   myDFPlayer.play(mp3Number);
   playedCount = playedCount + 1;
   Serial.print(F("Actual Play: "));
   Serial.println(mp3Number);
   Serial.print(F("Play Count: "));
   Serial.println(playedCount);
   }
 }

 void playSame(){
   myDFPlayer.play(mp3Number);
   playedCount = playedCount + 1;
   Serial.print(F("Actual Play: "));
   Serial.println(mp3Number);
   Serial.print(F("Play Count: "));
   Serial.println(playedCount);
 }
          
 
void falling(){
  attachInterrupt(digitalPinToInterrupt(interruptPin), rising, RISING);
  pwm_value = micros() - prev_time;
  microSecondsLastChange = microSecondsLastChange+1;
  if (microSecondsLastChange>100){
    Serial.println(F("Tick MacroScopic "));
           Serial.println(microSecondsLastChange);
    forceChange = false;
    if(pwm_value < oldPwm_value-120 || pwm_value > oldPwm_value+120){
      Serial.println(F("Koekcia "));
      oldPwm_value  = pwm_value;
      }
      
      changeUp();
      

    }
  }
void loop()
{
  static unsigned long timer = millis();
  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      forceChange = true;
      if(value == 2 || value == 4 || value == 6 || value == 8){
        mp3Number = value;
        playSame();
      }
      else{
          mp3Number = value+1;   
          playSame();
      }
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}
