/*
It's finished by Ethan. 
 */

#include<Servo.h> 

#define CH1 2   // right and left 
#define CH2 3   // forward and backward 
#define CH3 4 
#define CH4 5 

#define LMOTOR_PIN 11 // can use pwm 
#define RMOTOR_PIN 9 // can use pwm 

unsigned long CH1Time = 0 ; 
unsigned long CH2Time = 0 ; 
unsigned long CH3Time = 0 ; 
unsigned long CH4Time = 0 ; 

void setup()
{
    // for debug 
    Serial.begin(9600);

    //get value of rc controller 
    pinMode(CH1, INPUT) ; 
    pinMode(CH2, INPUT); 
    pinMode(CH3, INPUT) ; 
    pinMode(CH4, INPUT) ; 

    // contorl motors  
    pinMode(LMOTOR_PIN, OUTPUT); 
    pinMode(RMOTOR_PIN, OUTPUT); 

    analogWrite(LMOTOR_PIN,180);
    analogWrite(RMOTOR_PIN, 180);  
}

void loop()
{
    CH1Time = pulseIn(CH1,HIGH) ;
    Serial.print("CH1: ");
    Serial.println(CH1Time);   
    CH2Time = pulseIn(CH2,HIGH) ;
    Serial.print("CH2: "); 
    Serial.println(CH2Time);  
    //CH3Time = pulseIn(CH3,HIGH) ;
    Serial.print("CH3: "); 
    Serial.println(CH3Time); 
    //CH4Time = pulseIn(CH4,HIGH) ;
    Serial.print("CH4: "); 
    Serial.println(CH4Time); 

    if(CH2Time > 1700)
    {
        //right 
        Serial.println("right") ; 

        analogWrite(LMOTOR_PIN, 254); 
        analogWrite(RMOTOR_PIN, 90); 
    }
    else if(CH2Time < 1300)
    {
        //left 
        Serial.println("left") ;

        analogWrite(LMOTOR_PIN, 90); 
        analogWrite(RMOTOR_PIN, 254); 
    }
    else if(CH1Time > 1700)
    {
        //forward 
        Serial.println("forward") ;
         
        analogWrite(LMOTOR_PIN, 254); 
        analogWrite(RMOTOR_PIN, 254); 
    }
    else if(CH1Time < 1300) 
    { 
        //backward 
        Serial.println("backward") ; 
        
        analogWrite(LMOTOR_PIN, 90); 
        analogWrite(RMOTOR_PIN, 90); 
    }
    else
    {
        //stop 
        Serial.println("stop") ;

        analogWrite(LMOTOR_PIN, 180); 
        analogWrite(RMOTOR_PIN, 180); 
    }
    Serial.println("---------"); 
    
    delay(50); 
}
