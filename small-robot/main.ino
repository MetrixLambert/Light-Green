#define LIGHT_THRE 40 

int photocellPin = 2;    
int ledPin = 13;         
int val = 0;             


void setup() {
  Serial.begin(9600);
}

void loop() {
  val = analogRead(photocellPin);    
  Serial.println(val);

  if(val<=112){
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
}