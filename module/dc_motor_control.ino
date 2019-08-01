/*
  when the pwm value is 180, motor stops 
  biggest value (fastest) can be used : 90 and 254 (different direction)
  lower than 90 (or value is 255): the motor also stop
*/

const int motorPin = 2 ; 
int pwm = 0 ; 

void setup() {
  // put your setup code here, to run once:
  pinMode(motorPin, OUTPUT); 

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(pwm = 0; pwm<256; pwm++) 
  {
    analogWrite(motorPin, pwm); 
    Serial.println(pwm); 

    delay(100); 
  }

  for(pwm = 255; pwm>=0; pwm--) 
  {
    analogWrite(motorPin, pwm); 
    Serial.println(pwm); 

    delay(100); 
  }
}