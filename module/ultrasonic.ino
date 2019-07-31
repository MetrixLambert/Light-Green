#define Trig 2 
#define Echo 3
 
float cm; 
float temp;  
 
void setup() {
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
}
 
void loop() {

  digitalWrite(Trig, LOW); 
  delayMicroseconds(2);    
  digitalWrite(Trig,HIGH); 
  delayMicroseconds(10);    
  digitalWrite(Trig, LOW); 
  
  temp = float(pulseIn(Echo, HIGH)); 
  
  cm = (temp * 17 )/1000; 
 
  Serial.print("Echo =");
  Serial.print(temp);
  Serial.print(" | | Distance = ");
  Serial.print(cm);
  Serial.println("cm");
  delay(100);
}
