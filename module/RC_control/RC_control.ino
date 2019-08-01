/*
It's finished by Ethan. 
 */

#include<Servo.h> 

Servo LFServo ; // 6
Servo LBServo ; // 7 
Servo RFServo ; // 8 
Servo RBServo ; // 9 

#define CH1 2   // right and left 
#define CH2 3   // forward and backward 
#define CH3 4 
#define CH4 5 

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

    // contorl Servos 
    LFServo.attach(6);
    LBServo.attach(7);
    RFServo.attach(8);
    RBServo.attach(9);
}

void loop()
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