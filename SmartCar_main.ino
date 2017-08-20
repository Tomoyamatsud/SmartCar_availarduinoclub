#include <Servo.h>
#include <MsTimer2.h>

// Definitions Arduino pins connected to input HC-SR04
#define echoPin 2 // Echo Pin
#define trigPin 3 // Trigger Pin

// Definitions Arduino pins connected to input H Bridge
#define IN1 9
#define IN2 10
#define IN3 6
#define IN4 7

// Definitions Arduino pins connected to input ServoMotor
#define servoPin 8

#define INCPOS 1 // サーボの角度加減量
#define DELAYTIME 10 //サーボの処理待ち時間

#define MODE_STOP 0
#define MODE_FOWARD 1
#define MODE_BACK 2
#define MODE_TURN 3

// サーボの初期設定
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int pos = 0;    // variable to store the servo position

// 超音波センサの初期設定
double Duration = 0; //受信した間隔
double Distance = 0; //距離

//センサー動作関数のプロトタイプ宣言
void sen(void);

void flash() {
  sen();
}

void setup()
{

 Serial.begin(9600); // 超音波センサとシリアル通信するためのポート番号
 //サーボの設定
 myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
 pos = 90; // サーボの初期角度
 myservo.write(pos);  // サーボを初期角度にセットする
 
 // Set the output pins
    // ステップモータ
 pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT);
 pinMode(IN3, OUTPUT);
 pinMode(IN4, OUTPUT);
   // 超音波センサ
 pinMode( echoPin, INPUT );
 pinMode( trigPin, OUTPUT );

 //割り込み処理
 MsTimer2::set(500, flash); // 500ms period
 MsTimer2::start();

}
  
void loop()
{
    if( Distance < 10.0 )
    {
     servo();

    }

    else {

 
    }
}

void L298N( int mode )
{
  if( mode == MODE_FOWARD )
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
  }
  else if( mode == MODE_STOP )
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
}

void servo()
{
  for (pos = 90; pos <= 180; pos += INCPOS)
  { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(DELAYTIME);
  }                         // waits 15ms for the servo to reach the position
  
  for (pos = 180; pos >= 0; pos -= INCPOS)
  { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'    
    delay(DELAYTIME);
  }                           // waits 15ms for the servo to reach the position
  
  for (pos = 0; pos <= 90; pos += INCPOS)
  { // goes from 0 degrees to 90 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(DELAYTIME);
  }                           // waits 15ms for the servo to reach the position
}

void sen()
{
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite( trigPin, HIGH ); //超音波を出力
  delayMicroseconds( 10 ); //
  digitalWrite( trigPin, LOW );
  Duration = pulseIn( echoPin, HIGH ); //センサからの入力
  if (Duration > 0)
  {
    Duration = Duration/2; //往復距離を半分にする
    Distance = Duration*340*100/1000000; // 音速を340m/sに設定
    Serial.print("Distance:");
    Serial.print(Distance);
    Serial.println(" cm");
  }
}
