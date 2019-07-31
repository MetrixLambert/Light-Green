#define TRACE_THRE  1000  // if value lower than this thre, it must be white  

unsigned long time;
int color=1,last,last_color;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  pinMode(9,OUTPUT);
  digitalWrite(9,HIGH);
  delayMicroseconds(12);
  pinMode(9,INPUT);
  time=micros();

  while(digitalRead(9)) 
  {
    //Serial.println(micros()-time);
  }
  Serial.println(micros()-time);
 
  if((micros()-time - last) > TRACE_THRE) 
    color=-1;     // means black 
  else if((micros()-time - last) < TRACE_THRE) 
    color=1;      // means white 
 
  Serial.print(color),Serial.println(" ");
   
  last=micros()-time;
  last_color=color;
}
