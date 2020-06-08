/*  Arduino DC Motor Control - PWM | H-Bridge | L298N
         Project - Arduino Robot Car Control
    by Romaric Tsopnang, www.TheiTronics.com
*/
//    The direction of the car's movement
//  enA   enB   in1   in2   in3   in4   Description  
//  HIGH  HIGH  HIGH  LOW   LOW   HIGH  Car is runing forward
//  HIGH  HIGH  LOW   HIGH  HIGH  LOW   Car is runing back
//  HIGH  HIGH  LOW   HIGH  LOW   HIGH  Car is turning left
//  HIGH  HIGH  HIGH  LOW   HIGH  LOW   Car is turning right
//  HIGH  HIGH  LOW   LOW   LOW   LOW   Car is stoped
//  HIGH  HIGH  HIGH  HIGH  HIGH  HIGH  Car is stoped
//  LOW   LOW   N/A   N/A   N/A   N/A   Car is stoped
#define enA 9
#define in1 4
#define in2 5
#define enB 10
#define in3 6
#define in4 7
#define manuAutoPB 8
#define autoPin 11 //fake addressing, just to keep a number
#define manuPin 13

//Système anti-collision
#define trigPin1 3
#define echoPin1 2
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
  digitalWrite(enA,HIGH); 
  digitalWrite(enB,HIGH); 
  digitalWrite(in1,HIGH); //set in1 hight level
  digitalWrite(in2,LOW);  //set in2 low level
  digitalWrite(in3,LOW);  //set in3 low level
  digitalWrite(in4,HIGH); //set in4 hight level
  Serial.println("Forward");//send message to serial monitor
}
void forward_pro(){ 
  digitalWrite(enA,HIGH); 
  digitalWrite(enB,HIGH); 
  digitalWrite(in1,LOW); //set in1 hight level
  digitalWrite(in2,LOW);  //set in2 low level
  digitalWrite(in3,LOW);  //set in3 low level
  digitalWrite(in4,HIGH); //set in4 hight level
  Serial.println("Forward");//send message to serial monitor
}

void back(){
  digitalWrite(enA,HIGH); 
  digitalWrite(enB,HIGH);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  Serial.println("Back");
}
void back_pro(){
  digitalWrite(enA,HIGH); 
  digitalWrite(enB,HIGH);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  Serial.println("Back");
}

void left(){
  analogWrite(enA,carSpeed); 
  analogWrite(enB,carSpeed);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH); 
  Serial.println("Left");}

void right() {
  analogWrite(enA,carSpeed); 
  analogWrite(enB,carSpeed);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  Serial.println("Left");
  }


  void stopme(){
  digitalWrite(enA,LOW);
  digitalWrite(enB,LOW);
    digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  Serial.println("Stop!");
}


void setup() {
  pinMode(enA, OUTPUT); // PWM signal for right montor
  pinMode(enB, OUTPUT); // PWM signal for left montor
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  //initialising Manual/Auto Button as input 
  pinMode(manuAutoPB, INPUT);
  //initialising Manual/Auto LED state  
  pinMode(manuPin, OUTPUT);
  pinMode(autoPin, OUTPUT);

  //Setting State LED to their default state
  digitalWrite(autoPin, LOW);
  digitalWrite(manuPin, HIGH);
  Serial.begin(9600);//open serial and set the baudrate

  //Système anti-collision
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(obstLedPin, OUTPUT);
  pinMode(freeLedPin, OUTPUT);
  
  stopme();
  while(!Serial){
    ;}
}
void loop() {

  //Système anti-collision
  SonarSensor(trigPin1, echoPin1);
  UltraSensor = distance;
  //Serial.println(UltraSensor);

  // Read button - Debounce
  if (digitalRead(manuAutoPB) == true) {
    pressed = !pressed;
  }
  while (digitalRead(manuAutoPB) == true);
  delay(20);
  // If button is pressed - change Manual/Auto State
  if (pressed == true  & ManuAutoState == 0) {
    digitalWrite(autoPin, LOW);
    digitalWrite(manuPin, HIGH);
    buttonState = false;
    ManuAutoState = 1;
    delay(20);
    
    //stopme();
  }

  
  // If button is pressed - change Manual/Auto State
  if (pressed == false & ManuAutoState == 1) {
    digitalWrite(autoPin, HIGH);
    digitalWrite(manuPin, LOW);    
    buttonState = true;
    
    ManuAutoState = 0;
    delay(20);
  }
  /*
  Serial.println("The state is: ");
  Serial.println(buttonState);
*/

  if (buttonState == false && UltraSensor > distance_securite) { //Manual Mode
    
    int xAxis = analogRead(A0); // Read Joysticks X-axis
    int yAxis = analogRead(A1); // Read Joysticks Y-axis
    // Y-axis used for forward and backward control
    if (yAxis < 470) {
      // Set Motor A backward
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      // Set Motor B backward
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
      motorSpeedA = map(yAxis, 470, 0, 0, 255);
      motorSpeedB = map(yAxis, 470, 0, 0, 255);
    }
    else if (yAxis > 550) {
      // Set Motor A forward
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      // Set Motor B forward
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
      motorSpeedA = map(yAxis, 550, 1023, 0, 255);
      motorSpeedB = map(yAxis, 550, 1023, 0, 255);
    }
    // If joystick stays in middle the motors are not moving
    else {
      motorSpeedA = 0;
      motorSpeedB = 0;
    }
    // X-axis used for left and right control
    if (xAxis < 470) {
      // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 255 value
      int xMapped = map(xAxis, 470, 0, 0, 255);
      // Move to left - decrease left motor speed, increase right motor speed
      motorSpeedA = motorSpeedA - xMapped;
      motorSpeedB = motorSpeedB + xMapped;
      // Confine the range from 0 to 255
      if (motorSpeedA < 0) {
        motorSpeedA = 0;
      }
      if (motorSpeedB > 255) {
        motorSpeedB = 255;
      }
    }
    if (xAxis > 550) {
      // Convert the increasing X-axis readings from 550 to 1023 into 0 to 255 value
      int xMapped = map(xAxis, 550, 1023, 0, 255);
      // Move right - decrease right motor speed, increase left motor speed
      motorSpeedA = motorSpeedA + xMapped;
      motorSpeedB = motorSpeedB - xMapped;
      // Confine the range from 0 to 255
      if (motorSpeedA > 255) {
        motorSpeedA = 255;
      }
      if (motorSpeedB < 0) {
        motorSpeedB = 0;
      }
    }
    // Prevent buzzing at low speeds (Adjust according to your motors. My motors couldn't start moving if PWM value was below value of 70)
    if (motorSpeedA < 70) {
      motorSpeedA = 0;
    }
    if (motorSpeedB < 70) {
      motorSpeedB = 0;
    }
    analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
    analogWrite(enB, motorSpeedB); // Send PWM signal to motor B       

  }


    
  // If button is pressed - change Manual/Auto State
  if (buttonState == true && UltraSensor > distance_securite) { // Auto Mode
    //digitalWrite(autoPin, HIGH);
    //digitalWrite(manuPin, LOW);
    //stopme();
    
  if(Serial.available()){
      getstr = Serial.read();
      switch(getstr){
        /*case 'd': forward();delay(100);forward();left(); break;
        case 'g': back();  delay(100); back();left();  break;*/
        case 'd': forward_pro(); break;
        case 'g': back_pro(); break;
        case 'a': left(); break;
        case 'r': right();  break;
        case 's': stopme();  break;
        default:  break;}
  }

  }
  
    if(UltraSensor > distance_securite){ //on avance
      
    }else{ //on arrete
      stopme();
    }
  
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

