#define Grado_1_arriba 2
#define Grado_1_abajo 3
#define Grado_2_arriba 4
#define Grado_2_abajo 5

#define CH1 6
#define CH2 7
#define CH3 8
#define CH4 9

const int inputs[4]={Grado_1_arriba,Grado_2_arriba,Grado_1_abajo,Grado_2_abajo};
const int Channels[4]={CH1,CH2,CH3,CH4};

void setup()
{
  for(int x=0;x<4;x++){ 
    pinMode(inputs[x],OUTPUT);
    digitalWrite(inputs[x],LOW);  
  }
  for(int x=0;x<4;x++){
    pinMode(Channels[x],INPUT);
  }
}

void loop(){
  unsigned long counter=0,timer=0;
  unsigned long Value_CH1;
  unsigned long Value_CH2;
if((Value_CH1>1400)&&(Value_CH1<1600)){   //Stop
      digitalWrite(Grado_1_arriba,LOW);
      digitalWrite(Grado_1_abajo,LOW);
      digitalWrite(Grado_2_arriba,LOW);
      digitalWrite(Grado_2_abajo,LOW);
 }
    else if(Value_CH1<1400){                  //Primer Grado Abajo
      digitalWrite(Grado_1_arriba,LOW);
      digitalWrite(Grado_1_abajo,HIGH);      
    }
    else if(Value_CH1>1600){                  //Primer Grado Arriba
      digitalWrite(Grado_1_arriba,HIGH);
      digitalWrite(Grado_1_abajo,LOW);     
    }
    else if((Value_CH2>1400)&&(Value_CH2<1600)){   //Stop
      digitalWrite(Grado_1_arriba,LOW);
      digitalWrite(Grado_1_abajo,LOW);
      digitalWrite(Grado_2_arriba,LOW);
      digitalWrite(Grado_2_abajo,LOW);
 }
    else if(Value_CH2<1400){                  //Primer Grado Abajo
      digitalWrite(Grado_2_arriba,LOW);
      digitalWrite(Grado_2_abajo,HIGH);      
    }
    else if(Value_CH2>1600){                  //Primer Grado Arriba
      digitalWrite(Grado_2_arriba,HIGH);
      digitalWrite(Grado_2_abajo,LOW);     
    }
}