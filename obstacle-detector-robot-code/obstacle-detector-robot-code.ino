/*  Arduino DC Motor Control - PWM | H-Bridge | L298N
         Project - Arduino Robot Car Control
    by Romaric Tsopnang, www.TheiTronics.com
*/
#include <AFMotor.h>
// DC motor on M2
AF_DCMotor motor1AvantDroite(1);
AF_DCMotor motor2AvantGauche(2);
AF_DCMotor motor3ArriereDroite(3);
AF_DCMotor motor4ArriereGauche(4);

//Système anti-collision
#define trigPin1 2
#define echoPin1 3
#define obstLedPin 11 //fake addressing, just to keep a number
#define freeLedPin 12

long duration, distance, UltraSensor, distance_securite = 10;

int motorSpeedA = 0;
int motorSpeedB = 0;

boolean buttonState = LOW;
int ManuAutoState = 0; // 0 == Manual and 1 == Auto
int pressed = false;


unsigned char carSpeed = 255;
char getstr;


void forward(){

  motor1AvantDroite.run(FORWARD);
  motor2AvantGauche.run(FORWARD);
  motor3ArriereDroite.run(FORWARD);
  motor4ArriereGauche.run(FORWARD); 
 
  Serial.println("Forward");//send message to serial monitor
}

void back(){
  motor1AvantDroite.run(BACKWARD);
  motor2AvantGauche.run(BACKWARD);
  motor3ArriereDroite.run(BACKWARD);
  motor4ArriereGauche.run(BACKWARD);
  Serial.println("Back");
}


void left(){  
  motor1AvantDroite.run(FORWARD);
  motor2AvantGauche.run(BACKWARD);
  motor3ArriereDroite.run(FORWARD);
  motor4ArriereGauche.run(BACKWARD);
  Serial.println("Left");
  }

void right() {
  motor1AvantDroite.run(BACKWARD);
  motor2AvantGauche.run(FORWARD);
  motor3ArriereDroite.run(BACKWARD);
  motor4ArriereGauche.run(FORWARD);
  Serial.println("Right");
  }


  void stopme(){
  motor1AvantDroite.run(RELEASE);
  motor2AvantGauche.run(RELEASE);
  motor3ArriereDroite.run(RELEASE);
  motor4ArriereGauche.run(RELEASE);
  Serial.println("Right");
}


void setup() {

  Serial.begin(9600);//open serial and set the baudrate

  //Système anti-collision
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(obstLedPin, OUTPUT);
  pinMode(freeLedPin, OUTPUT);
  
  // turn on motor #1
  motor1AvantDroite.setSpeed(200);
  motor1AvantDroite.run(RELEASE);
  
  // turn on motor #2
  motor2AvantGauche.setSpeed(200);
  motor2AvantGauche.run(RELEASE);
  
  // turn on motor #3
  motor3ArriereDroite.setSpeed(200);
  motor3ArriereDroite.run(RELEASE);
  
  // turn on motor #2
  motor4ArriereGauche.setSpeed(200);
  motor4ArriereGauche.run(RELEASE);
  
  while(!Serial){
    ;}
}
void loop() {

  //Système anti-collision
  SonarSensor(trigPin1, echoPin1);
  UltraSensor = distance;
  Serial.println(UltraSensor);

  // If button is pressed - change Manual/Auto State
  if (UltraSensor > distance_securite) { // Auto Mode
    //digitalWrite(autoPin, HIGH);
    //digitalWrite(manuPin, LOW);
    //stopme();
     }
  if(Serial.available()){
      getstr = Serial.read();
      Serial.println(getstr);
      
      switch(getstr){
        /*case 'd': forward();delay(100);forward();left(); break;
        case 'g': back();  delay(100); back();left();  break;*/
        case 'd': right(); break;
        case 'g': left(); break;
        case 'a': forward(); break;
        case 'r': back();  break;
        case 's': stopme();  break;
        default:  break;
        }
  }

 
  /*
    if(UltraSensor > distance_securite){ //on avance
      
    }else{ //on arrete
      stopme();
    }
   */
  
}



    void SonarSensor(int trigPin,int echoPin)
    {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    //distance = (duration/2) * 29.1;
    distance = duration*340/20000;
    delay(100);

    if(distance<10){ 
      
      digitalWrite(obstLedPin, HIGH);
      digitalWrite(freeLedPin, LOW);
    }else{
      
      digitalWrite(obstLedPin , LOW);
      digitalWrite(freeLedPin, HIGH);
    }
    
  }
