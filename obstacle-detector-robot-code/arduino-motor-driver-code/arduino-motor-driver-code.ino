// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>

// DC motor on M2
AF_DCMotor motor(2);
// DC hobby servo
Servo servo1;


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor party!");
  
   
  // turn on motor #2
  motor.setSpeed(200);
  motor.run(RELEASE);
}

int i;

// Test the DC motor, stepper and servo ALL AT ONCE!
void loop() {
  motor.run(FORWARD);
  delay(3000);
  motor.run(RELEASE);
  delay(3000);  
  
}
