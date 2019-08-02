#include <Servo.h>

//Pin Configure 
#define CH1 2   // right and left 
#define CH2 3   // forward and backward 
#define CH3 4   // 
#define CH4 5   //

Servo servoSweeper ;// A7 
Servo servoFlag ;   // 10
Servo LFServo ;     // 6
Servo LBServo ;     // 7 
Servo RFServo ;     // 13 
Servo RBServo ;     // 8

#define DEBUG_PIN A2 

#define lineTracer A6

#define Trig 9 
#define Echo 12

#define binMotor 11   

#define photocellPin A5

//Threhold 
#define LIGHT_THRE 100
#define TRACE_THRE  1000  // if value lower than this thre, it must be white  
#define DIS_THRE 168 // in cm 

// global var 
unsigned long CH1Time = 0 ; 
unsigned long CH2Time = 0 ; 
unsigned long CH3Time = 0 ; 
unsigned long CH4Time = 0 ; 
unsigned long traceTime = 0;

int color=1,last,last_color;

int val = 0;

float cm; 
float temp; 

long beginTime ;
bool gameStatus = 0;
 
//motion control 
void forward(int speed); 
void backward(int speed); 
void left(int speed); 
void right(int speed); 
void stop();

//lind tracing 
bool isOnLine();

//ultraSonic 
int ultraSonicDis();

//flag control 
void flagUp();
void flagUpTo(int pos); 
void flagDown(); 
void flagDownTo(int pos);


void setup() 
{
    Serial.begin(9600);
    
    // ultra sonic 
    pinMode(Trig, OUTPUT);
    pinMode(Echo, INPUT);
    pinMode(binMotor, OUTPUT);
    pinMode(photocellPin, INPUT);    
  
    // get value of rc controller 
    pinMode(CH1, INPUT) ; 
    pinMode(CH2, INPUT); 
    pinMode(CH3, INPUT) ; 
    pinMode(CH4, INPUT) ; 

    // motion Servos 
    LFServo.attach(6);
    LBServo.attach(7);
    RFServo.attach(13);   // 8 
    RBServo.attach(8);

    // flag servo
    servoFlag.attach(10);
    
    // sweeper servo
    servoSweeper.attach(A7);

    // for debug 
    Serial.begin(9600); 
    pinMode(DEBUG_PIN,INPUT); 
    if(analogRead(DEBUG_PIN) < 100)
    {
        gameStatus = 0 ; 
    }
    else
    {
        gameStatus = 1 ; //directly begin remote control 
    }    
}
 
void loop() 
{
    //auto period 
    if(gameStatus == 0)
    {
        //Light sensor
        val = analogRead(photocellPin);    
        Serial.print("LDR value: "), Serial.println(val);
        
        if(val>=LIGHT_THRE)
        {
            Serial.println("the light on!!!!"); 
            
            //record time 
            beginTime = millis();
            
            //foward 
            forward(90);
        }
        else
        {
            stop();  
        }

        //Ultrasonic
        cm = ultraSonicDis(); 
        if (cm >= DIS_THRE)
        {
            stop(); 
            servoFlag.write(180);
        }

    //change game status 
    if(millis() - beginTime >= 29000)   
        gameStatus = 1;

    delay(50); 
}
// remote control time 
else        
{
    //begin rotate 
    servoSweeper.write(180);
  
    //RC control
    CH1Time = pulseIn(CH1,HIGH,2400) ;    //only wait for 2400 ms 
    Serial.print("CH1: ");
    Serial.println(CH1Time);   
    CH2Time = pulseIn(CH2,HIGH,2400) ;
    Serial.print("CH2: "); 
    Serial.println(CH2Time);  
    CH3Time = pulseIn(CH3,HIGH,2400) ;
    Serial.print("CH3: "); 
    Serial.println(CH3Time); 
    CH4Time = pulseIn(CH4,HIGH,2400) ;
    Serial.print("CH4: "); 
    Serial.println(CH4Time);

    //motion control 
   if(CH2Time > 1700)
    {
        //right 
        right(90); 
    }
    else if(CH2Time < 1300 && CH2Time != 0)
    {
        //left
        left(90); 
    }
    else if(CH1Time > 1700)
    {
        //forward 
        forward(90);
    }
    else if(CH1Time < 1300 && CH1Time != 0) 
    { 
        //backward 
        backward(90);
    }
    else
    {
        //stop 
        stop(); 
    }

    //flag control 
    if(CH3Time > 1700)
    {
        //flag up 
        Serial.println("flag up") ; 
        servoFlag.write(180);
    }
    else if(CH3Time < 1300 && CH3Time != 0 ) 
    { 
        //flag down
        Serial.println("flag down") ; 
        servoFlag.write(0);
    }
    else
    {
      // no motion 
    }

    //bin control 
    if(CH4Time > 1700)
    {
        //bin forward
        Serial.println("bin forward") ; 
        analogWrite(binMotor, 254);
    }
    else if(CH4Time < 1300 && CH4Time != 0) 
    { 
        //bin backward
        Serial.println("bin bacward") ; 
        analogWrite(binMotor, 0);
    }
    else 
    {
        // no motion   
    }

    delay(50);
  }
}


// motion control function 
void forward (int speed)
{
    Serial.println("forward") ; 

    LFServo.write(180); 
    LBServo.write(180); 
    RFServo.write(0); 
    RBServo.write(0);
}

void backward (int speed) 
{
    Serial.println("backward") ; 

    LFServo.write(0); 
    LBServo.write(0); 
    RFServo.write(180); 
    RBServo.write(180);
}

void left (int speed)
{
    Serial.println("left") ;

    LFServo.write(0); 
    LBServo.write(0); 
    RFServo.write(0); 
    RBServo.write(0);
}

void right(int speed)
{
    Serial.println("right") ; 

    LFServo.write(180); 
    LBServo.write(180); 
    RFServo.write(180); 
    RBServo.write(180); 
}

void stop()
{
    Serial.println("stop") ;

    LFServo.write(90); 
    LBServo.write(90); 
    RFServo.write(95); 
    RBServo.write(95); 
}

// Line trace function 
bool isOnLine()
{
    //Line Tracer
    pinMode(8,OUTPUT);
    digitalWrite(8,HIGH);
    delayMicroseconds(12);
    pinMode(8,INPUT);
    traceTime=micros();

    while(digitalRead(8)) 
    {
        //Serial.print("Line tracer time = "), Serial.println(micros()-traceTime);
    }     
    traceTime = micros() - traceTime;   
    Serial.println(traceTime);
 
  if((traceTime - last) > TRACE_THRE)  
      color=0;     // means black 
  else if((traceTime - last) < TRACE_THRE) 
      color=1;      // means white 
 
  //Serial.print("is On Line: ");
  //Serial.prinntln(color); 
  
  last=traceTime;

  return color; 
}

// ultra sonic cal Distance 
int ultraSonicDis()
{
    digitalWrite(Trig, LOW); 
    delayMicroseconds(2);    
    digitalWrite(Trig,HIGH); 
    delayMicroseconds(10);    
    digitalWrite(Trig, LOW); 
  
    temp = float(pulseIn(Echo, HIGH)); 
  
    cm = (temp * 17 )/1000; 

    //Serial.print("Echo =");
    //Serial.print(temp);
    //Serial.print(" | | Distance = ");
    //Serial.print(cm);
    //Serial.println("cm");

    return cm; 
}
