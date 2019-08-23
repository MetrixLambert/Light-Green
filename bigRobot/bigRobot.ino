#include <Servo.h>

//Pin Configure 
#define CH1 2   
#define CH2 1   
#define CH3 4   
#define CH4 5   

#define sweeperServoPin 10   

#define binServoPin 11
#define flagServoPin 3

#define LFServoPin 6 
#define LBServoPin 7 
#define RFServoPin 13 
#define RBServoPin 8    

#define Trig 9 
#define Echo 12

#define DEBUG_PIN A2  
#define photocellPin A5
 
//Threhold 
#define LIGHT_THRE 25
#define TRACE_THRE  1000  // if value lower than this thre, it must be white  
#define DIS_THRE 30 // in cm 

//constant var 
#define BIN_ANGLE 5 
#define FLAG_ANGLE 5 

#define BIN_UP_VAL 21   //31
#define BIN_DOWN_VAL 79

#define ROTATE_SPEED 254

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
 
Servo binServo ;    
Servo flagServo ;   
Servo LFServo ;     
Servo LBServo ;      
Servo RFServo ;      
Servo RBServo ;  
Servo sweeperServo ; 

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
void flagStop();

//bin control 
void binUp();
void binDown(); 


void setup() 
{    
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
    LFServo.attach(LFServoPin);
    LBServo.attach(LBServoPin);
    RFServo.attach(RFServoPin);  
    RBServo.attach(RBServoPin);

    // flag servo
    flagServo.attach(flagServoPin);
    
    // bin servo and sweeper motor 
    binServo.attach(binServoPin);
    binServo.write(BIN_DOWN_VAL); 
    
    sweeperServo.attach(sweeperServoPin); 
    sweeperServo.write(91); 

    // for debug 
    //Serial.begin(115200);
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
                delay(1000); //forward enough 
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
            Serial.print("dis to right: "); 
            Serial.println(cm); 

            if (cm <= DIS_THRE && cm > 0)
            {
                Serial.println("arrived!! stop!!"); 
                
                stop(); 
                flagUpTo(180); 

                // wait for control 
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
       
    CH2Time = pulseIn(CH2,HIGH) ;
     
    CH3Time = pulseIn(CH3,HIGH) ;

    CH4Time = pulseIn(CH4,HIGH) ;
    
    if(CH1Time == 0 && CH3Time == 0)
    {
        Serial.println("RC control not online"); 
    }
    else
    {
        Serial.print("CH1: ");
        Serial.println(CH1Time);
        Serial.print("CH2: "); 
        Serial.println(CH2Time); 
        Serial.print("CH3: "); 
        Serial.println(CH3Time);     
        Serial.print("CH4: "); 
        Serial.println(CH4Time);  
    }
    
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
        Serial.println("------- flag up --------") ; 
        flagUp(); 
    }
    else if(CH3Time < 1300 && CH3Time != 0 ) 
    { 
        //flag down
        Serial.println("------- flag down ------- ") ; 
        flagDown(); 
    }
    else
    {
        // flag stop 
        Serial.println("------ flag stop --------") ;
        flagStop();
    }

    //bin control 
    if(CH4Time > 1700)
    {
        //bin up
        Serial.println("-------- bin up --------- ") ; 
        binUp();
    }
    else if(CH4Time < 1300 && CH4Time != 0) 
    { 
        //bin down
        Serial.println("------- bin bacward -------") ; 
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
    LBServo.write(110); 
    RFServo.write(0); 
    RBServo.write(0);
}

void backward (int speed) 
{
    Serial.println("backward") ; 

    LFServo.write(0); 
    LBServo.write(75); 
    RFServo.write(180); 
    RBServo.write(180);
}

void left (int speed)
{
    Serial.println("left") ;

    LFServo.write(0); 
    LBServo.write(75); 
    RFServo.write(0); 
    RBServo.write(0);
}

void right(int speed)
{
    Serial.println("right") ; 

    LFServo.write(180); 
    LBServo.write(110); 
    RFServo.write(180); 
    RBServo.write(180); 
}

void stop()
{
    Serial.println("stop") ;

    LFServo.write(90); 
    LBServo.write(91); 
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
    flagServo.write(20); 
}


void flagDown()
{
    flagServo.write(160); 
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

void flagDownTo(int pos)
{
    if(pos<0)
        pos = 0  ; 
    else if(pos >180)
        pos = 180 ; 

    flagPos = pos ; 
    flagServo.write(flagPos); 
}

void flagStop()
{
    flagServo.write(91); 
}

//bin control 
void binUp()
{
    binServo.write(BIN_UP_VAL); 

    //stop rotate 
    sweeperServo.write(20); 
}

void binDown()
{
    binServo.write(BIN_DOWN_VAL); 

    //begin rotate 
    sweeperServo.write(91); 
}
