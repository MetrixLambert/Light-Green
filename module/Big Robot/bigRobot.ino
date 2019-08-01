#include <Servo.h>

#define Trig 6 
#define Echo 7
#define sweeperMotor 4
#define photocellPin A5
#define TRACE_THRE  1000  // if value lower than this thre, it must be white  
#define CH1 2   // right and left 
#define CH2 3   // forward and backward 
#define CH3 4 
#define CH4 5 
#define lineTracer 8 

Servo servoFrontLeft;
Servo servoFrontRight;
Servo servoBackLeft;
Servo servoBackRight;
Servo servoFlag;

Servo LFServo ; // 6
Servo LBServo ; // 7 
Servo RFServo ; // 8 
Servo RBServo ; // 9 

unsigned long CH1Time = 0 ; 
unsigned long CH2Time = 0 ; 
unsigned long CH3Time = 0 ; 
unsigned long CH4Time = 0 ; 
unsigned long time;
int color=1,last,last_color;
int sweeperSpeed = 255;       
int val = 0;
long beginTime ;
float cm; 
float temp; 
bool gameStatus = 0;
 
void setup() {
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(sweeperMotor, OUTPUT);
  pinMode(photocellPin, INPUT);    
  
  //get value of rc controller 
    pinMode(CH1, INPUT) ; 
    pinMode(CH2, INPUT); 
    pinMode(CH3, INPUT) ; 
    pinMode(CH4, INPUT) ; 

    // contorl Servos 
    LFServo.attach(6);
    LBServo.attach(7);
    RFServo.attach(8);
    RBServo.attach(9);
}
 
void loop() {
  if(gameStatus == 0)
{
  //Light sensor
  
  val = analogRead(photocellPin);    
  Serial.print("LDR value: "), Serial.println(val);
  if(val>=100){
    servoFrontLeft.write(270);
    servoFrontRight.write(270);
    servoBackLeft.write(270);
    servoBackRight.write(270);

    beginTime = millis();
  }

  //Line Tracer
  
  pinMode(8,OUTPUT);
  digitalWrite(8,HIGH);
  delayMicroseconds(12);
  pinMode(8,INPUT);
  time=micros();

  while(digitalRead(8)) 
  {
    Serial.print("Line tracer time = "), Serial.println(micros()-time);
  }
  Serial.println(micros()-time);
 
  if((micros()-time - last) > TRACE_THRE) 
    color=-1;     // means black 
  else if((micros()-time - last) < TRACE_THRE) 
    color=1;      // means white 
 
  Serial.print(color),Serial.println(" ");
   
  last=micros()-time;
  last_color=color;

  //Ultrasonic
  
  digitalWrite(Trig, LOW); 
  delayMicroseconds(2);    
  digitalWrite(Trig,HIGH); 
  delayMicroseconds(10);    
  digitalWrite(Trig, LOW); 
  
  temp = float(pulseIn(Echo, HIGH)); 
  
  cm = (temp * 17 )/1000; 
  if (cm >= 168){
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
  gameStatus = 1;
}
else{
  
  //RC control
  
  if( millis()- beginTime > 30000) 
  {
    CH1Time = pulseIn(CH1,HIGH) ;
    Serial.print("CH1: ");
    Serial.println(CH1Time);   
    CH2Time = pulseIn(CH2,HIGH) ;
    Serial.print("CH2: "); 
    Serial.println(CH2Time);  
    CH3Time = pulseIn(CH3,HIGH) ;
    Serial.print("CH3: "); 
    Serial.println(CH3Time); 
    CH4Time = pulseIn(CH4,HIGH) ;
    Serial.print("CH4: "); 
    Serial.println(CH4Time);
   if(CH2Time > 1700)
    {
        //forward 
        Serial.println("right") ; 

        LFServo.write(180); 
        LBServo.write(180); 
        RFServo.write(180); 
        RBServo.write(180); 
    }
    else if(CH2Time < 1300)
    {
        //backward 
        Serial.println("left") ;

        LFServo.write(0); 
        LBServo.write(0); 
        RFServo.write(0); 
        RBServo.write(0);
    }
    else if(CH1Time > 1700)
    {
        //right 
        Serial.println("forward") ; 

        LFServo.write(180); 
        LBServo.write(180); 
        RFServo.write(0); 
        RBServo.write(0);
    }
    else if(CH1Time < 1300) 
    { 
        //left 
        Serial.println("backward") ; 

        LFServo.write(0); 
        LBServo.write(0); 
        RFServo.write(180); 
        RBServo.write(180);
    }
    else
    {
        //stop 
        Serial.println("stop") ;

        LFServo.write(90); 
        LBServo.write(90); 
        RFServo.write(95); 
        RBServo.write(95); 
    }
    
    delay(50);
  }
}
}