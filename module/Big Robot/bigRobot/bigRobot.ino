#include <Servo.h>

//Pin Configure 
#define CH1 2   // right and left 
#define CH2 3   // forward and backward 
#define CH3 4   // 
#define CH4 5   //

Servo binServo ;    // 11 
Servo flagServo ;   // 10
Servo LFServo ;     // 6
Servo LBServo ;     // 7 
Servo RFServo ;     // 13 
Servo RBServo ;     // 8

#define DEBUG_PIN A2 

#define lineTracer A6

#define Trig 9 
#define Echo 12

#define sweeperMotor A3  

#define photocellPin A5

//Threhold 
#define LIGHT_THRE 100
#define TRACE_THRE  1000  // if value lower than this thre, it must be white  
#define DIS_THRE 168 // in cm 

//constant var 
#define BIN_ANGLE 5 
#define FLAG_ANGLE 5 

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

int binPos = 0 ; 

long beginTime ;
bool gameOn = false; 
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
int flagPos = 0; 
void flagUp();
void flagUpTo(int pos); 
void flagDown(); 
void flagDownTo(int pos);

//bin control 
void binUp();
void binDown(); 


void setup() 
{
    Serial.begin(9600);
    
    // ultra sonic 
    pinMode(Trig, OUTPUT);
    pinMode(Echo, INPUT);
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
    flagServo.attach(10);
    
    // sweeper servo
    binServo.attach(11);

    // for debug 
    Serial.begin(9600); 
    pinMode(DEBUG_PIN,INPUT); 
    if(analogRead(DEBUG_PIN) < 800)
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
        if(gameOn == false)
        {
            //sense the light 
            val = analogRead(photocellPin);    
            Serial.print("LDR value: "), Serial.println(val);
            if(val>=LIGHT_THRE)
            {
                Serial.println("the light on!!!!"); 
            
                //record time 
                beginTime = millis();
                gameOn = true; 
            
                //foward 
                forward(90);
            }
            else
            {
                stop();  
            }
        }
        else // if game is on 
        {
            //Ultrasonic
            cm = ultraSonicDis(); 
            Serial.print("dis to backward: "); 
            Serial.println(cm); 

            if (cm >= DIS_THRE)
            {
                Serial.println("arrived!! stop!!"); 
                stop(); 
                flagUpTo(180); 

                gameStatus = 1;
            }
        }
      
        //change game status 
        if(gameOn == true && millis() - beginTime >= 29000)   
            gameStatus = 1;

        delay(50); 
}
// remote control time 
else        
{
    //begin rotate 
    //not finished here 
  
    //RC control
    CH1Time = pulseIn(CH1,HIGH) ;    //only wait for 2400 ms 
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
        flagUp(); 
    }
    else if(CH3Time < 1300 && CH3Time != 0 ) 
    { 
        //flag down
        Serial.println("flag down") ; 
        flagDown(); 
    }
    else
    {
      // no motion 
    }

    //bin control 
    if(CH4Time > 1700)
    {
        //bin up
        Serial.println("bin up") ; 
        binUp();
    }
    else if(CH4Time < 1300 && CH4Time != 0) 
    { 
        //bin down
        Serial.println("bin bacward") ; 
        binDown(); 
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

//flag 
void flagUp()
{
    flagPos = flagPos + FLAG_ANGLE; 
    if(flagPos > 180)
        flagPos = 180 ; 
    
    flagServo.write(flagPos); 
}

void flagUpTo(int pos)
{
    if(pos< 0)
        pos = 0 ; 
    else if(pos > 180)
        pos = 180; 

    flagPos = pos ; 
    flagServo.write(flagPos); 
}

void flagDown()
{
    flagPos = flagPos - FLAG_ANGLE; 

    if(flagPos < 0)
        flagPos = 0 ; 
    
    flagServo.write(flagPos); 
}

void flagDownTo(int pos)
{
    if(pos<0)
        pos = 0  ; 
    else if(pos >180)
        pos = 180 ; 

    flagPos = pos ; 
    flagServo.write(flagPos); 
}


//bin control 
void binUp()
{
    binPos = binPos + BIN_ANGLE; 
    if(binPos > 180)
        binPos = 180 ; 

    binServo.write(binPos); 
}

void binDown()
{
    binPos = binPos - BIN_ANGLE ; 
    if(binPos < 0)
        binPos = 0 ; 
    
    binServo.write(binPos); 
}