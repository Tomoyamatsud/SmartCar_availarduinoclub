// (c) Michael Schoeffler 2016, http://www.mschoeffler.de
 
#define ENA 5  //enable A on pin 5 (must be a pwm pin)   Speed Control
#define ENB 3  //enable B on pin 3 (must be a pwm pin)   Speed Control

#define IN1 7  //IN1 on pin controls Motor A  Motor A is the left side Motor
#define IN2 6  //IN2 on pin controls Motor A
#define IN3 4  //IN3 on pin conrtols Motor B  Motor B is the Right side Motor
#define IN4 2  //IN4 on pin controls Motor B 

void setup() {
 pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT);
 pinMode(ENA, OUTPUT);
 
 pinMode(IN3, OUTPUT);
 pinMode(IN4, OUTPUT);
 pinMode(ENB, OUTPUT);
 
}
 
void slow() {
 analogWrite(ENA, 128);
 analogWrite(ENB, 128);
}
 
void fast() {
 analogWrite(ENA, 255);
 analogWrite(ENB, 255);
}
 
void loop() { 
 digitalWrite(IN1, LOW);
 digitalWrite(IN2, HIGH);
 digitalWrite(IN3, LOW);
 digitalWrite(IN4, HIGH);
 slow();
 delay(5000);
 fast();
 delay(5000);
 // change of direction
 digitalWrite(IN1, HIGH);
 digitalWrite(IN2, LOW);
 digitalWrite(IN3, HIGH);
 digitalWrite(IN4, LOW);
 slow();
 delay(5000);
 fast();
 delay(5000);
}
