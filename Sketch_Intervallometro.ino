#include <LowPower.h>

// these are the common anodes
int an1 = 7;
int an2 = 9;
int an3 = 10;

int interval = A0;    // pot
int gopin = A1;         // pushbutton
int minutes = A2;     // switch

// pins for Multiport
int focus = 12;
int shutter = 13;

// 7-segment LED pins:
//
//     1     _____A_____          10
//          |          |    
//     2     F          B          9
//          |          |
//     3      ____G_____           8
//          |          |    
//     4     E          C          7
//          |          |
//     5     _____D_____   (DP)    6       <- not used


int lE = 2;
int lD = 3;
int lC = 5;
int lG = 6;
int lB = 8;
int lA = 11;
int lF = 4;

int reading = 0;
int reading_out = 2;
int counter = 0;

void let(char letter, int anode) {
  digitalWrite(anode, HIGH);
  switch (letter) {
    case 'l':
      digitalWrite(lF, LOW);
      digitalWrite(lE, LOW);
      digitalWrite(lD, LOW);
      break;
    default:
      digitalWrite(lA, HIGH);
      digitalWrite(lB, HIGH);
      digitalWrite(lG, HIGH);
      digitalWrite(lE, HIGH);
      digitalWrite(lD, HIGH);
      digitalWrite(lC, HIGH);
      digitalWrite(lF, HIGH);
      break;
    }
  delay(2);
  digitalWrite(anode, LOW);
  digitalWrite(lA, HIGH);
  digitalWrite(lB, HIGH);
  digitalWrite(lG, HIGH);
  digitalWrite(lE, HIGH);
  digitalWrite(lD, HIGH);
  digitalWrite(lC, HIGH);
  digitalWrite(lF, HIGH);
  delay(1);
}


void seg(int number, int anode) {
  digitalWrite(anode, HIGH);
  switch (number) {
    case 1:
      digitalWrite(lB, LOW);
      digitalWrite(lC, LOW);
      break;
    case 2:
      digitalWrite(lA, LOW);
      digitalWrite(lB, LOW);
      digitalWrite(lG, LOW);
      digitalWrite(lE, LOW);
      digitalWrite(lD, LOW);
      break;
    case 3:
      digitalWrite(lA, LOW);
      digitalWrite(lB, LOW);
      digitalWrite(lG, LOW);
      digitalWrite(lC, LOW);
      digitalWrite(lD, LOW);
      break;
    case 4:
      digitalWrite(lF, LOW);
      digitalWrite(lG, LOW);
      digitalWrite(lB, LOW);
      digitalWrite(lC, LOW);
      break;
    case 5:
      digitalWrite(lA, LOW);
      digitalWrite(lF, LOW);
      digitalWrite(lG, LOW);
      digitalWrite(lC, LOW);
      digitalWrite(lD, LOW);
      break;
    case 6:
      digitalWrite(lA, LOW);
      digitalWrite(lF, LOW);
      digitalWrite(lG, LOW);
      digitalWrite(lE, LOW);
      digitalWrite(lC, LOW);
      digitalWrite(lD, LOW);
      break;
    case 7:
      digitalWrite(lA, LOW);
      digitalWrite(lB, LOW);
      digitalWrite(lC, LOW);
      break;
    case 8:
      digitalWrite(lA, LOW);
      digitalWrite(lB, LOW);
      digitalWrite(lC, LOW);
      digitalWrite(lG, LOW);
      digitalWrite(lE, LOW);
      digitalWrite(lD, LOW);
      digitalWrite(lF, LOW);
      break;
    case 9:
      digitalWrite(lA, LOW);
      digitalWrite(lB, LOW);
      digitalWrite(lC, LOW);
      digitalWrite(lG, LOW);
      digitalWrite(lD, LOW);
      digitalWrite(lF, LOW);
      break;
    case 0:
      digitalWrite(lA, LOW);
      digitalWrite(lB, LOW);
      digitalWrite(lC, LOW);
      digitalWrite(lE, LOW);
      digitalWrite(lD, LOW);
      digitalWrite(lF, LOW);
      break;
    default:
      digitalWrite(lA, HIGH);
      digitalWrite(lB, HIGH);
      digitalWrite(lG, HIGH);
      digitalWrite(lE, HIGH);
      digitalWrite(lD, HIGH);
      digitalWrite(lC, HIGH);
      digitalWrite(lF, HIGH);
      break;
    }
  delay(2);
  digitalWrite(anode, LOW);
  digitalWrite(lA, HIGH);
  digitalWrite(lB, HIGH);
  digitalWrite(lG, HIGH);
  digitalWrite(lE, HIGH);
  digitalWrite(lD, HIGH);
  digitalWrite(lC, HIGH);
  digitalWrite(lF, HIGH);
  delay(1);
}

void num(int digit){
  int num2 = digit % 10;
  int num1 = (digit - num2) / 10;
  seg(num1, an2);
  seg(num2, an1);
}

void go(){
  pinMode(focus, OUTPUT);
  pinMode(shutter, OUTPUT);
  delay(20);
  digitalWrite(focus, 0);
  delay(50);
  digitalWrite(shutter, 0);
  delay(50);
  pinMode(focus, INPUT);
  pinMode(shutter, INPUT);
}

void timelapse(int seconds){
    while (seconds > 15){
        seconds -= 8;
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    }
    if (seconds < 15){
        while (seconds > 5){
            seconds -= 2;
            LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
        }
        while (seconds > 2){
            seconds -= 1;
            LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
        }
        if (seconds <= 2){
                LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
                LowPower.powerDown(SLEEP_500MS, ADC_ON, BOD_ON);
            }
    }
}

void setup() {
  Serial.begin(9600);
  //assert all pins, just to make sure...
  pinMode(an1, OUTPUT);
  pinMode(an2, OUTPUT);
  pinMode(an3, OUTPUT);
  pinMode(lA, OUTPUT);
  pinMode(lB, OUTPUT);
  pinMode(lC, OUTPUT);
  pinMode(lD, OUTPUT);
  pinMode(lE, OUTPUT);
  pinMode(lF, OUTPUT);
  pinMode(lG, OUTPUT);
  pinMode(lDP, OUTPUT);
  
  pinMode(focus, INPUT);
  pinMode(shutter, INPUT);
  
  digitalWrite(an1, LOW);
  digitalWrite(an2, LOW);
  digitalWrite(an3, LOW);
  digitalWrite(lA, HIGH);
  digitalWrite(lB, HIGH);
  digitalWrite(lG, HIGH);
  digitalWrite(lE, HIGH);
  digitalWrite(lD, HIGH);
  digitalWrite(lC, HIGH);
  digitalWrite(lF, HIGH);
  digitalWrite(lDP, HIGH);
  
  while (analogRead(gopin) > 800) {
    reading = 2 + (analogRead(interval) / 11.5); // this will result in a number between 2-90
    if (digitalRead(minutes) > 0) {
      let('l', an3);
      }
    num(reading);
    }
  reading_out = reading;
  if (digitalRead(minutes) > 0){
    reading_out = reading_out * 60;
  }
  reading_out = reading_out - (3*(reading_out/60));  //timelapse() seems to be about 5% off, this will correct for it
}

void loop() {
  go();
  delay(50);
  timelapse(reading_out);
  delay(50);
}
