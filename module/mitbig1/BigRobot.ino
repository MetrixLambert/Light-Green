#include <Servo.h>

#define Trig 2 
#define Echo 3
#define sweeperMotor 4
#define servoPin1 5
#define servoPin2 6
#define servoPin3 7
#define servoPin4 8
#define photocellPin 9
#define ledPin 10
Servo servoFrontLeft;
Servo servoFrontRight;
Servo servoBackLeft;
Servo servoBackRight;
Servo servoFlag;

int sweeperSpeed = 255;       
int val = 0;

long beginTime ;
 
float cm; 
float temp;  
 
void setup() {
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(sweeperMotor, OUTPUT);
  servoFrontLeft.attach(servoPin1);
  servoFrontRight.attach(servoPin2);
  servoBackLeft.attach(servoPin3);
  servoBackRight.attach(servoPin4);        
}
 
void loop() {
  val = analogRead(photocellPin);    
  Serial.println(val);
  if(val>=100){
    servoFrontLeft.write(270);
    servoFrontRight.write(270);
    servoBackLeft.write(270);
    servoBackRight.write(270);

    beginTime = millis();
  }
  
  digitalWrite(Trig, LOW); 
  delayMicroseconds(2);    
  digitalWrite(Trig,HIGH); 
  delayMicroseconds(10);    
  digitalWrite(Trig, LOW); 
  
  temp = float(pulseIn(Echo, HIGH)); 
  
  cm = (temp * 17 )/1000; 
  if (cm = 168){
    servoFrontLeft.write(0);
    servoFrontRight.write(0);
    servoBackLeft.write(0);
    servoBackRight.write(0);
    servoFlag.write(270);
  }
  Serial.print("Echo =");
  Serial.print(temp);
  Serial.print(" | | Distance = ");
  Serial.print(cm);
  Serial.println("cm");
//  measure(30000);
  if( millis()- beginTime > 30000) 
  {
    // switch RC
  }
  else
  {
    // wait   
  }
  
  //RC code

  analogWrite(sweeperMotor, sweeperSpeed);
}