#include <Servo.h>

#define frontLM 1
#define frontRM 2
#define ledPin 4
#define sweeperServoPin A1
#define forkServoPin A2
#define SWEEP_TIME 3000
#define Grado_1_arriba 5
#define Grado_1_abajo 6
#define Grado_2_arriba 7
#define Grado_2_abajo 8
#define CH1 9
#define CH2 10
#define CH3 11
#define CH4 12


Servo sweeperServo;
Servo forkServo;

int val = 0;
int photocellPin = A3;
const int inputs[4]={Grado_1_arriba,Grado_2_arriba,Grado_1_abajo,Grado_2_abajo};
const int Channels[4]={CH1,CH2,CH3,CH4};


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