int photocellPin = 5;    
int ledPin = 13;         
int val = 0;             


void setup() {
  Serial.begin(9600);
}

void loop() {
  val = analogRead(photocellPin);    
  Serial.println(val);

  delay(500); 
}
