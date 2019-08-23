#include <Servo.h>

// pin configuration 
#define CH1 2
#define CH2 3
#define CH3 4
#define CH4 5

#define LServo_Pin 6
#define RServo_Pin 7

#define sweeperServoPin 10
#define forkServoPin 8

#define DEBUG_PIN A0 
#define photocellPin A3

// threhold 
#define LIGHT_THRE 25 

//const value 
#define SWEEP_TIME 3000

#define LMotor_UP 254
#define LMotor_DOWN 90
#define LMotor_MID 180 
#define RMotor_UP 254
#define RMotor_DOWN 90
#define RMotor_MID 180 

//status value  
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
Servo LServo; 
Servo RServo; 
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
  
  LServo.attach(LServo_Pin); 
  RServo.attach(RServo_Pin); 

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
   if(CH2Time > 1700 && CH2Time< 3000)
    {
        //right 
        right(90); 
    }
    else if(CH2Time < 1300 && CH2Time != 0)
    {
        //left
        left(90); 
    }
    else if(CH1Time > 1700 && CH2Time < 3000)
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
    if(CH3Time > 1700 && CH3Time < 3000)
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
    if(CH4Time > 1700 && CH3Time < 3000)
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
    if(gameStart == false)  //wait period 
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
      if(millis() - startTime > 29000)
      {
        //rc period start 
        autoPeriod = false ; 
      }
      else
      {
        //motion serial 
      }
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
      if(CH2Time > 1700 && CH2Time < 3000)
      {
        //right 
        right(90); 
      }
      else if(CH2Time < 1300 && CH2Time != 0)
      {
        //left
        left(90); 
      }
      else if(CH1Time > 1700&& CH2Time< 3000)
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
      if(CH3Time > 1700 && CH3Time< 3000)
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
      if(CH4Time > 1700 && CH4Time < 3000)
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
}

//control func 
void forward(int speed)
{
    LServo.write(160);
    RServo.write(160);
} 

void backward(int speed)
{
    LServo.write(20); 
    RServo.write(20); 
}

void right(int speed)
{
    LServo.write(160);
    RServo.write(20); 
}

void left(int speed)
{
    LServo.write(20); 
    RServo.write(160);  
}

void stop(void )
{
    LServo.write(91); 
    RServo.write(91); 
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
