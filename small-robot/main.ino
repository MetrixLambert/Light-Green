#include <Servo.h>

#define frontLM 1
#define frontRM 2
#define ledPin 4
#define sweeperServoPin 5
#define forkServoPin 6
#define SWEEP_TIME 3000


Servo sweeperServo;
Servo forkServo;

int val = 0;
int photocellPin = A3;


void setup() {
  Serial.begin(9600);
  pinMode(frontLM, OUTPUT);
  pinMode(frontRM, OUTPUT);
  sweeperServo.attach(sweeperServoPin);
  forkServo.attach(forkServoPin);
}

void loop() {
  val = analogRead(photocellPin);    
  Serial.println(val);
  if(val>=100){
    digitalWrite(frontLM,HIGH);
    digitalWrite(frontRM,HIGH);
  }

  if(millis() > SWEEP_TIME) 
  {
    sweeperServo.write(270);
  }

  //RC code + fork
  
}