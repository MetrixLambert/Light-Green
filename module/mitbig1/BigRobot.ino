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
#define TRACE_THRE  1000  // if value lower than this thre, it must be white  
#define Grado_1_arriba 2
#define Grado_1_abajo 3
#define Grado_2_arriba 4
#define Grado_2_abajo 5
#define CH1 6
#define CH2 7
#define CH3 8
#define CH4 9

Servo servoFrontLeft;
Servo servoFrontRight;
Servo servoBackLeft;
Servo servoBackRight;
Servo servoFlag;

unsigned long time;
int color=1,last,last_color;
int sweeperSpeed = 255;       
int val = 0;
long beginTime ;
float cm; 
float temp; 
const int inputs[4]={Grado_1_arriba,Grado_2_arriba,Grado_1_abajo,Grado_2_abajo};
const int Channels[4]={CH1,CH2,CH3,CH4};
 
void setup() {
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(sweeperMotor, OUTPUT);
  servoFrontLeft.attach(servoPin1);
  servoFrontRight.attach(servoPin2);
  servoBackLeft.attach(servoPin3);
  servoBackRight.attach(servoPin4);  
  for(int x=0;x<4;x++){ 
    pinMode(inputs[x],OUTPUT);
    digitalWrite(inputs[x],LOW);  
  }
  for(int x=0;x<4;x++){
    pinMode(Channels[x],INPUT);
  }      
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

  pinMode(11,OUTPUT);
  digitalWrite(11,HIGH);
  delayMicroseconds(12);
  pinMode(11,INPUT);
  time=micros();

  while(digitalRead(11)) 
  {
    //Serial.println(micros()-time);
  }
  Serial.println(micros()-time);
 
  if((micros()-time - last) > TRACE_THRE) 
    color=-1;     // means black 
  else if((micros()-time - last) < TRACE_THRE) 
    color=1;      // means white 
 
  Serial.print(color),Serial.println(" ");
   
  last=micros()-time;
  last_color=color;
  
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
    unsigned long counter=0,timer=0;
  unsigned long Value_CH1;
  unsigned long Value_CH2;
if((Value_CH1>1400)&&(Value_CH1<1600)){   //Stop
      digitalWrite(Grado_1_arriba,LOW);
      digitalWrite(Grado_1_abajo,LOW);
      digitalWrite(Grado_2_arriba,LOW);
      digitalWrite(Grado_2_abajo,LOW);
 }
    else if(Value_CH1<1400){                  //Primer Grado Abajo
      digitalWrite(Grado_1_arriba,LOW);
      digitalWrite(Grado_1_abajo,HIGH);      
    }
    else if(Value_CH1>1600){                  //Primer Grado Arriba
      digitalWrite(Grado_1_arriba,HIGH);
      digitalWrite(Grado_1_abajo,LOW);     
    }
    else if((Value_CH2>1400)&&(Value_CH2<1600)){   //Stop
      digitalWrite(Grado_1_arriba,LOW);
      digitalWrite(Grado_1_abajo,LOW);
      digitalWrite(Grado_2_arriba,LOW);
      digitalWrite(Grado_2_abajo,LOW);
 }
    else if(Value_CH2<1400){                  //Primer Grado Abajo
      digitalWrite(Grado_2_arriba,LOW);
      digitalWrite(Grado_2_abajo,HIGH);      
    }
    else if(Value_CH2>1600){                  //Primer Grado Arriba
      digitalWrite(Grado_2_arriba,HIGH);
      digitalWrite(Grado_2_abajo,LOW);     
    }
    analogWrite(sweeperMotor, sweeperSpeed);
  }
}