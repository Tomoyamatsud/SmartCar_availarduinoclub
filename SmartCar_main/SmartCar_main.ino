/* Ultrasonic obstacle-avoidance car
4WDRobot_Motor_Basic with Roaming for Danger
Sketch to test out the operation of the Sainsmart 4WD Robot.
4WD Robot by Glenn Mossy
https://github.com/gmossy/Sainsmart-4WD-Robot
gmossy@gmail.com
DC Robotics Group, Arduino Motors Workshop, Dec 6, 2014
Dec 6, 2014, version 1.0     
      
       *THE SUBROUTINES WORK AS FOLLOWS*
motorA(mode, speed)  // % replace A with B to control motor B %
mode is a number 0 -> 3 that determines what the motor will do.
0 = coast/disable the H bridge
1 = turn motor clockwise
2 = turn motor counter clockwise
3 = brake motor
speed is a number 0 -> 100 that represents percentage of motor speed.
0 = off
50 = 50% of full motor speed
100 = 100% of full motor speed
EXAMPLE
Say you need to have motor A turn clo`ckwise at 33% of its full speed.  The subroutine call would be the following...
motorA(1, 33);
*/

#include <Servo.h>      //servo library
//#include "pitches.h"  // used for the speaker output, add pitches.h to a pitches folder under /documents/arduino/libraies.
#define SPEAKER 10      // define a Speaker Pin, if you add a buzzer or speaker, uncomment the pitches.h library, and buzz methods.
 
//Hardware set the pins
// L298D Motor Controller
#define ENA 5  //enable A on pin 5 (must be a pwm pin)   Speed Control
#define ENB 3  //enable B on pin 3 (must be a pwm pin)   Speed Control

#define IN1 6  //IN1 on pin controls Motor A  Motor A Right Motor (view from front)
#define IN2 7  //IN2 on pin controls Motor A
#define IN3 2  //IN3 on pin conrtols Motor B  Motor B Left Motor (view from front)
#define IN4 4  //IN4 on pin controls Motor B 

#define SERIAL_PORT 9600 // 自分が追加

const int rightmotorpin1 = IN1;  //signal output 1 of Dc motor 
const int rightmotorpin2 = IN2;  //signal output 2 of Dc motor 
const int leftmotorpin1  = IN3;  //signal output 3 of Dc motor 
const int leftmotorpin2  = IN4;  //signal output 4 of Dc motor 

int motorSpeed;          //define motor speed parameter which will be mapped as a percentage value

// pins for the HC-SR04 Ultrasonic Distance Sensor
const int EchoPin = 11; // HC-SR04 Ultrasonic signal input
const int TrigPin = 12; // HC-SR04 Ultrasonic signal output
const int HeadServopin = 9; // pin for signal input of headservo

const int button1Pin = 8;  // pushbutton 1 pin

int add= 0;       //used for nodanger loop count 疑問：なぜカウントしてるかわからない
int add1= 0;      //used for nodanger loop count 疑問：なぜカウントしてるかわからない
int roam = 0;     //just listen for serial commands and wait
int motorSpeed = 20;            // Set motorSpeed variable with an initial motor speed % (percentage)  low end is about 20
int currDist = 5000; // distance
boolean running = false;// boolean to flag whether i'm running or not

Servo headservo;        // initialize a servo object

void setup() { 
Serial.begin(SERIAL_PORT); // Enables Serial monitor for debugging purposes
Serial.println("Ready to receive Serial Commands![f, b, r, l, s, t, 1]"); // Tell us I"m ready
Serial.println("My Commands are: ");
Serial.println("f:forward");
Serial.println("b:backward");
Serial.println("r:right");
Serial.println("l:left");
Serial.println("s:stop");
Serial.println("t:toggleRoam");
Serial.println("1:"); //追記

 //signal output port
 //set all of the outputs for the motor driver
  pinMode(IN1, OUTPUT);       // Motor Driver 
  pinMode(IN2, OUTPUT);       // Motor Driver
  pinMode(IN3, OUTPUT);       // Motor Driver
  pinMode(IN4, OUTPUT);       // Motor Driver
  
  // Set up the pushbutton pins to be an input:
  pinMode(button1Pin, INPUT);
  int button1State=LOW;
  
 // intialize_beeps();        // call the intialize_beeps method and play some tones 

// set HC-SR04 ultrasonic Sensor Pins
   pinMode(TrigPin, OUTPUT);  // Set the HC-SR04 Trig as Output
   pinMode(EchoPin, INPUT);   // Set the HC-SR04 Echo as Input

// setup and test the servo:
headservo.attach(HeadServopin);
//start my movable servo head, to verify I work.
headservo.write(20);
delay(500);
headservo.write(180);
delay(500);
headservo.write(90);
delay(500);
}

void loop() 
  {
    int button1State =LOW;  // variables to hold the pushbutton states 追記：何もしていない

    Serial.print(Serial.read()); // 追記
    
    if (Serial.available() > 0)
    {
      int val = Serial.read();  //read serial input commands
 //    buzz();
    switch(val)
    {
    case 'f' : 
      Serial.println("Rolling Forward!");
      moveForward(motorSpeed); 
      delay(5000);
      brake();
      delay(5000);
      break;
      
    case 'l' : 
      Serial.println("Turning Left!");
      body_lturn(motorSpeed);
      delay(2000);
      brake();
      delay(5000);
      break;
      
    case 'r' :   
      Serial.println("Turning Right!");
      body_rturn(motorSpeed);
      delay(2000);
      brake();
      delay(5000);
            break;       
   case 'b' :    
      Serial.println("Turning Backward!");
      moveBackward(motorSpeed);
      delay(5000);
      brake();
      delay(5000);
            break;
   case 's' :      
      Serial.println("Stop!");
      brake();
      delay(5000);
            break;
   case '1' :      
      Serial.println("Rev the Motor Forward");
      Serial.println("Rolling Forward!");
      for (int motorSpeed=20; motorSpeed < 100; motorSpeed++)   // lets increment the motorSpeed
      {
      moveForward(motorSpeed); 
      delay(250);
      }
      brake();
      Serial.println("Rev the Motor Backward");
      Serial.println("Rolling Backward!");
      for (int motorSpeed=100; motorSpeed > 0; motorSpeed--)   // lets increment the motorSpeed
      {
      moveBackward(motorSpeed); 
      delay(250);
      }   
      motorSpeed = 55;  //reset the speed
      break;      
            
   case 't' :      
      Serial.println("toggle Roam Mode"); 
      toggleRoam();
      break;
    }
    Serial.println("I'm Ready to receive Serial Commands![f, b, r, l, s, t, 1]"); // Tell us I"m ready
  }  //Serial.available()に対応する
            
  if(roam == 0){ 
      //just listen for serial commands and wait
      }
  else if(roam == 1){  //If roam active- drive autonomously
      goRoam();          // Go Roaming.  
      }
     
  // Here we'll read the current pushbutton state

    button1State = digitalRead(button1Pin);
  // Now add functions to test if the button is pressed and do something
  if (button1State == LOW)    // if we're not pushing button 1  
  {
  //just listen for serial commands and wait
  }
  else
  {
     Serial.println("You pressed my pushbutton");
  } 
  button1State = LOW;   //reset the pushbutton state
 }
  
//******************   moveForward *******************
void moveForward(int motorSpeed)
{
   // int motorSpeed);  // change the 15 to the Speed variable, and put Speed int the function call command arguments.
    //also add delayTime for example like this:   moveForward(int delayTime, int motorSpeed)
    
    motorA(2, motorSpeed);  //have motor A turn clockwise at % speed, , call motor control method
    motorB(2, motorSpeed);  //have motor B turn clockwise at % speed
    Serial.println("Forward");
}
void moveBackward(int motorSpeed)
{
    motorA(1, motorSpeed);  //have motor A turn counterclockwise 
    motorB(1, motorSpeed);  //have motor B turn counterclockwise
    Serial.println("Backward");
}
void body_rturn(int motorSpeed)
{
   motorA(1, motorSpeed);  //have motor A turn counterclockwise 
   motorB(2, motorSpeed);  //have motor B turn clockwise 
   Serial.println("Right");
}
   void body_lturn(int motorSpeed)
{
   motorA(2, motorSpeed);  //have motor A turn clockwise
   motorB(1, motorSpeed);  //have motor B turn counterclockwise 
   Serial.println("Left");
}

void brake()
{
    motorA(3, 0);  //brake motor A with 100% braking power, call motor control method
    motorB(3, 0);  //brake motor B with 100% braking power, call motor control method
    Serial.println("Brake");
}

void toggleRoam(){
  // This method chooses to make the robot roam or else use the serial command input.
if(roam == 0){
   roam = 1;
   Serial.println("Activated Roam Mode");
  }
  else{
    roam = 0;
    brake();
    Serial.println("De-activated Roam Mode");
  }
}
void goRoam(){
  Serial.println("Im going roaming");
  moveForward(motorSpeed);    // temporary just go forward for a little while
//  time = millis(); // Sets "time" to current system time count
  currDist = MeasuringDistance(); //measure front distance
  Serial.print("Current Forward Distance: ");
  Serial.println(currDist);                       // Print the current distance
  if(currDist > 35) {
    delay(5000); // delayミリ秒間前進
  }
  else{
    //add=0;
    Serial.println("Forward Blocked- Decide Which Way");
    brake();
    delay(500);
    moveBackward(motorSpeed);            // Move backward with % speed
    delay(500);
    whichway();                  // decide which way to go
  }
}
   
//measure distance, unit “cm”
long MeasuringDistance() {
  // This method reads the HC-SR04 Ultrasonic Sensor and returns the distance measured of an object in front the sensor
      // Calculates the Distance in cm
    float cm;                          //define variables for distance sensor 
    // ((time)*(Speed of sound))/ toward and backward of object) = Width of Echo pulse, in uS (micro second)
    // How to call the function:    long Distance_cm = Distance(Duration);   // Use function to calculate the distance  
long duration;
long adjust = 1.15;                        // Calibration adjustment based on actual measurement test
//pinMode(TrigPin, OUTPUT);
digitalWrite(TrigPin, LOW);
delayMicroseconds(2);
digitalWrite(TrigPin, HIGH);
delayMicroseconds(5);
digitalWrite(TrigPin, LOW);
//pinMode(EchoPin, INPUT);
duration = pulseIn(EchoPin, HIGH);
return duration / 29 / 2 + adjust;        // Actual calculation in centimeters
}

//******************    nodanger   *******************
void nodanger() {
   running = true;//  Yes I'm still running
   if (add1 > 38 ) whichway(); // Robot will stop and check direction every X loops through nodanger then resets in totalhalt (40 is good)
   return;
}
//******************    whichway   *******************
//choose which way to turn when an object is blocking my path in front of my HC-SR04 distance sensor
void whichway() {
//running = true;         
      brake();
//      delay(5000); //テスト、不要と思われるのでコメントアウト
      
headservo.write(160);
delay(900);
int lDist = MeasuringDistance(); // check left distance
Serial.print("checking left: ");
Serial.println(lDist);
headservo.write(20);             // turn the servo right
delay(900);
int rDist = MeasuringDistance(); // check right distance
Serial.print("checking right: ");
Serial.println(rDist);

if(lDist < rDist) {
Serial.println("Decided Right Is Best");
//buzz();  //  buzz 
body_rturn(motorSpeed);
delay(2000);

currDist = MeasuringDistance();  //measure front distance
if(currDist < 45) body_rturn(motorSpeed);  //if front distance still too small- turn again
}
else{
Serial.println("Decided Left Is Best");
 // buzz();  // Make him talk
  body_lturn(motorSpeed);
   brake();       // Stop Me

  currDist = MeasuringDistance(); //measure front distance
  if(currDist < 45) body_lturn(motorSpeed); // if front distance still too small- turn again
   }
headservo.write(90);             // 2017.11.25 テスト用コード追加。サーボを正面に向ける。
return;
} 
//**********************************************************
