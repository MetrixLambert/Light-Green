#include <Servo.h>

// pin configuration 
#define CH1 2
#define CH2 3
#define CH3 4
#define CH4 5

#define LMotor_Pin 6
#define RMotor_Pin 10

#define sweeperServoPin 7
#define forkServoPin 8

#define DEBUG_PIN A0 
#define photocellPin A3

// const values 
#define LIGHT_THRE 25 
#define SWEEP_TIME 3000

#define LMotor_UP 254
#define LMotor_DOWN 90
#define LMotor_MID 180 
#define RMotor_UP 254
#define RMotor_DOWN 90
#define RMotor_MID 180 

//for game start 
int gameStart = 0 ;
long startTime = 0 ;  
int autoPeriod = 1 ; 

int val = 0;

unsigned long CH1Time = 0 ; 
unsigned long CH2Time = 0 ; 
unsigned long CH3Time = 0 ; 
unsigned long CH4Time = 0 ; 

int sweeperPos = 0 ;                  // need to check !!!!
int forkPos = 0 ; 

// Servo
Servo sweeperServo ;    //  7 
Servo forkServo ;       //  8 

//motion control func 
void forward(int speed); 
void backward(int speed); 
void right(int speed); 
void left(int speed); 
void stop(); 

//fork control 
void forkForward(int speed); 
void forkBackward(int speed); 

//sweeper control 
void sweepForward(int speed); 
void sweepBackward(int speed); 

void setup() {
  //debug 
  Serial.begin(115200);
  
  pinMode(frontLM, OUTPUT);
  pinMode(frontRM, OUTPUT);
  
  sweeperServo.attach(sweeperServoPin);
  forkServo.attach(forkServoPin);

  sweeperServo.write(sweeperPos); 
  forkServo.write(forkPos); 
}

void loop() 
{ 
  if(analogRead(DEBUG_PIN) > 800) //DEBUG 
  {
    //rc control 
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

    //fork control   
    if(CH3Time > 1700)
    {
        //fork forward
        Serial.println("fork forward") ; 
        forkForward(90) ;
    }
    else if(CH3Time < 1300 && CH3Time != 0 ) 
    { 
        //fork downward
        Serial.println("fork downward") ; 
        forkBackward(90) ; 
    }
    else
    {
      // no motion 
    }

    //sweeper control 
    if(CH4Time > 1700)
    {
        //sweeper forward
        Serial.println("sweeper forward") ;
        sweepForward(90) ;
    }
    else if(CH4Time < 1300 && CH4Time != 0) 
    { 
        //sweeper backward 
        Serial.println("bin bacward") ;  
        sweepBackward(90) ; 
    }
    else 
    {
        // no motion   
    }

    delay(50);  

  }
  else
  {
    //wait for game to start 
    if(gameStart == False)  //wait period 
    {
      val = analogRead(photocellPin);  
      Serial.println(val);
    
      if(val>=LIGHT_THRE)
      {
        gameStart = true ; 
        startTime = millis(); 
      }
    }
    else if(autoPeriod == true) //auto period 
    {
      // if into rc control period   
    }
    else //remote control period 
    {
        //rc control 
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

      //what control ?  
      if(CH3Time > 1700)
      {
        //flag up 
        Serial.println("flag up") ; 
        
      }
      else if(CH3Time < 1300 && CH3Time != 0 ) 
      {  
        //flag down
        Serial.println("flag down") ; 
      
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
      }
      else if(CH4Time < 1300 && CH4Time != 0) 
      { 
        //bin down
        Serial.println("bin bacward") ;  
      }
      else 
      {
        // no motion   
      }

      delay(50);  
    }       
  }
  //RC code + fork
  
}

//control func 
void forward(int speed)
{
  digitalWrite(LMotor_Pin, LMotor_UP); 
  digitalWrite(RMotor_Pin, RMotor_UP); 
} 

void backward(int speed)
{
  digitalWrite(LMotor_Pin, LMotor_DOWN); 
  digitalWrite(RMotor_Pin, RMotor_DOWN); 
}

void right(int speed)
{
  digitalWrite(LMotor_Pin, LMotor_UP); 
  digitalWrite(RMotor_Pin, RMotor_DOWN); 
}

void left(int speed)
{
  digitalWrite(LMotor_Pin, LMotor_DOWN); 
  digitalWrite(RMotor_Pin, RMotor_UP); 
}

void stop(void )
{
  digitalWrite(LMotor_Pin, LMotor_MID);
  digitalWrite(RMotor_Pin, RMotor_MID); 
}

//sweeper control 
void sweepForward(int speed)
{
  //sweeperServo.write();
} 

void sweepBackward(int speed)
{
  //sweeperServo.write();
}

//fork control 
void forkForward(int speed)
{
  //forkServo.write(); 
}

void forkBackward(int speed)
{
  //forkServo.write(); 
}
