#define IN_A1 2
#define IN_B1 3
#define IN_A2 4
#define IN_B2 5

#define Grado_Base_CS 14
#define Grado_Base_CCS 15
#define Grado_1_arriba 16
#define Grado_1_abajo 17
#define Grado_2_arriba 18
#define Grado_2_abajo 19
#define Grado_3_arriba 20
#define Grado_3_abajo 21
#define Gripper_abre 22
#define Gripper_cierra 23 

#define CH1 9
#define CH2 10
#define CH3 11
#define CH4 12
#define CH5 13

const int inputs[14]={IN_A1,IN_B1,IN_A2,IN_B2,Grado_Base_CS,Grado_Base_CCS,Grado_1_arriba,Grado_2_arriba,Grado_3_arriba,Grado_1_abajo,Grado_2_abajo,Grado_3_abajo,Gripper_abre,Gripper_cierra};
const int Channels[5]={CH1,CH2,CH3,CH4,CH5};
const int PWM_1 = 7;
const int PWM_2 = 8;

void setup(){
  for(int x=0;x<14;x++){ 
    pinMode(inputs[x],OUTPUT);
    digitalWrite(inputs[x],LOW);  
  }
  for(int x=0;x<5;x++){
    pinMode(Channels[x],INPUT);
  }
}

void Modo_Boton_ON(unsigned long Value_CH3,unsigned long Value_CH4){
if(((Value_CH3>1400)&&(Value_CH3<1600))&&((Value_CH4>1400)&&(Value_CH4<1600))){   //Stop
      digitalWrite(Grado_2_arriba,LOW);
      digitalWrite(Grado_2_abajo,LOW);
      digitalWrite(Grado_3_arriba,LOW);
      digitalWrite(Grado_3_abajo,LOW);
      digitalWrite(Gripper_abre,LOW);
      digitalWrite(Gripper_cierra,LOW);
    }
    else if(Value_CH4<1400){    //Segundo Grado Arriba  
      if((Value_CH3<1200)||(Value_CH3>1700)){   //Tercer Grado Arriba
        digitalWrite(Grado_3_arriba,HIGH);
        digitalWrite(Grado_3_abajo,LOW);
        digitalWrite(Grado_2_arriba,LOW);
        digitalWrite(Grado_2_abajo,LOW);
      }
      else{
        digitalWrite(Grado_2_arriba,HIGH);
        digitalWrite(Grado_2_abajo,LOW);    
        digitalWrite(Grado_3_arriba,LOW);
        digitalWrite(Grado_3_abajo,LOW);        
      }
    }
    else if(Value_CH4>1600){    //Segundo Grado Abajo
      if((Value_CH3<1200)||(Value_CH3>1700)){
        digitalWrite(Grado_3_arriba,LOW);
        digitalWrite(Grado_3_abajo,HIGH);
        digitalWrite(Grado_2_arriba,LOW);
        digitalWrite(Grado_2_abajo,LOW);
      }
      else{
        digitalWrite(Grado_2_arriba,LOW);
        digitalWrite(Grado_2_abajo,HIGH);  
        digitalWrite(Grado_3_arriba,LOW);
        digitalWrite(Grado_3_abajo,LOW);
      }    
    }
    else if((Value_CH3<1080)&&((Value_CH4>1400)&&(Value_CH4<1600))){
      digitalWrite(Gripper_abre,HIGH);
      digitalWrite(Gripper_cierra,LOW);
      digitalWrite(Grado_2_arriba,LOW);
      digitalWrite(Grado_2_abajo,LOW);
      digitalWrite(Grado_3_arriba,LOW);
      digitalWrite(Grado_3_abajo,LOW);
    }
    else if((Value_CH3>1920)&&((Value_CH4>1400)&&(Value_CH4<1600))){
      digitalWrite(Gripper_abre,LOW);
      digitalWrite(Gripper_cierra,HIGH);
      digitalWrite(Grado_2_arriba,LOW);
      digitalWrite(Grado_2_abajo,LOW);
      digitalWrite(Grado_3_arriba,LOW);
      digitalWrite(Grado_3_abajo,LOW);      
    }
}

void Modo_Boton_OFF(unsigned long Value_CH3,unsigned long Value_CH4){
 if(((Value_CH3>1400)&&(Value_CH3<1600))&&((Value_CH4>1400)&&(Value_CH4<1600))){   //Stop
      digitalWrite(Grado_Base_CS,LOW);
      digitalWrite(Grado_Base_CCS,LOW);
      digitalWrite(Grado_1_arriba,LOW);
      digitalWrite(Grado_1_abajo,LOW);
    }
    else if(Value_CH4<1400){                  //Primer Grado Abajo
      digitalWrite(Grado_1_arriba,LOW);
      digitalWrite(Grado_1_abajo,HIGH);      
    }
    else if(Value_CH4>1600){                  //Primer Grado Arriba
      digitalWrite(Grado_1_arriba,HIGH);
      digitalWrite(Grado_1_abajo,LOW);      
    }
    else if(Value_CH3<1080){                  //Base Izquierda
      digitalWrite(Grado_Base_CS,LOW);
      digitalWrite(Grado_Base_CCS,HIGH);
    }
    else if(Value_CH3>1920){                  //Base Derecha
      digitalWrite(Grado_Base_CS,HIGH);
      digitalWrite(Grado_Base_CCS,LOW);     
    }  
    else if((Value_CH3<1920)&&(Value_CH3>1080)){
      digitalWrite(Grado_Base_CS,LOW);
      digitalWrite(Grado_Base_CCS,LOW);      
    }
}

void loop(){
unsigned long counter=0,timer=0;
unsigned long Value_CH1 = pulseIn(Channels[0],HIGH);
unsigned long Value_CH2 = pulseIn(Channels[1],HIGH);
unsigned long Value_CH3 = pulseIn(Channels[2],HIGH);
unsigned long Value_CH4 = pulseIn(Channels[3],HIGH);
unsigned long Value_CH5 = pulseIn(Channels[4],HIGH);
unsigned int Error_1 = 1305 - Value_CH1;
unsigned int Error_2 = 1705 - Value_CH1;
unsigned int Error_3 = 1305 - Value_CH2;
unsigned int Error_4 = 1705 - Value_CH2;
  Error_2 *= -1;
  Error_4 *= -1;
  if(Error_1>255) Error_1 = 255;
  if(Error_2>255) Error_2 = 255;
  if(Error_3>255) Error_3 = 255;
  if(Error_4>255) Error_4 = 255;
  if(((Value_CH1<900)||(Value_CH1>2020))||((Value_CH2<900)||(Value_CH2>2020))){ 
    analogWrite(PWM_1,0);
    analogWrite(PWM_2,0);
  }
  if(Value_CH1<1306){               //Adelante    
   if((Value_CH1<1306)&&(Value_CH2<1105)){       //Delante_Derecha
    analogWrite(PWM_1,255);
    analogWrite(PWM_2,255);
    digitalWrite(inputs[0],LOW); 
    digitalWrite(inputs[1],LOW);
    digitalWrite(inputs[2],HIGH);
    digitalWrite(inputs[3],LOW);    
  }
  else if((Value_CH1<1306)&&(1905<=Value_CH2)){     //Delante_Izquierda
    analogWrite(PWM_1,255); 
    analogWrite(PWM_2,255);    
    digitalWrite(inputs[0],HIGH); 
    digitalWrite(inputs[1],LOW);
    digitalWrite(inputs[2],LOW);
    digitalWrite(inputs[3],LOW);    
  }
  else{
    analogWrite(PWM_1,Error_1);
    analogWrite(PWM_2,Error_1);
    digitalWrite(inputs[0],HIGH);
    digitalWrite(inputs[1],LOW);
    digitalWrite(inputs[2],HIGH); 
    digitalWrite(inputs[3],LOW);
  }
  }
  
  else if((Value_CH1>1305)&&(Value_CH1<=1704)&&((1305<Value_CH2)&&(Value_CH2<=1704))){  //Stop
    analogWrite(PWM_1,0);
    analogWrite(PWM_2,0);
    digitalWrite(inputs[0],LOW); 
    digitalWrite(inputs[1],LOW);
    digitalWrite(inputs[2],LOW);
    digitalWrite(inputs[3],LOW); 
  }
  else if(Value_CH1>1704){                //Reversa
    if((Value_CH1>1704)&&(Value_CH2<1106)){        //Reversa_Derecha
    analogWrite(PWM_1,255);
    analogWrite(PWM_2,255);
    digitalWrite(inputs[0],LOW); 
    digitalWrite(inputs[1],LOW);
    digitalWrite(inputs[2],LOW);
    digitalWrite(inputs[3],HIGH);    
  }
  else if((Value_CH1>1704)&&(1904<Value_CH2)){       //Reversa_Izquierda
    analogWrite(PWM_1,255);
    analogWrite(PWM_2,255);
    digitalWrite(inputs[0],LOW); 
    digitalWrite(inputs[1],HIGH);
    digitalWrite(inputs[2],LOW);
    digitalWrite(inputs[3],LOW);    
  }
 else {
    analogWrite(PWM_1,Error_2);
    analogWrite(PWM_2,Error_2);
    digitalWrite(inputs[0],LOW);
    digitalWrite(inputs[1],HIGH);
    digitalWrite(inputs[2],LOW); 
    digitalWrite(inputs[3],HIGH);
  }
  }
  
  else if((Value_CH1>=1305)&&(Value_CH1<=1704)&&(Value_CH2<1305)){    //Derecha_Static
    Error_3*=2;
    analogWrite(PWM_2,Error_3);
    digitalWrite(inputs[0],LOW); 
    digitalWrite(inputs[1],LOW);
    digitalWrite(inputs[2],HIGH);
    digitalWrite(inputs[3],LOW);   
  }
  else if((Value_CH1>=1305)&&(Value_CH1<=1704)&&(1704<=Value_CH2)){   //Izquierda_Static    
    Error_4*=2;
    analogWrite(PWM_1,Error_4);
    digitalWrite(inputs[0],HIGH); 
    digitalWrite(inputs[1],LOW);
    digitalWrite(inputs[2],LOW);
    digitalWrite(inputs[3],LOW);    
  }

  /*LA PARTE CHINGONA DEL PROGRAMA*/ //Nel Prro >:v
  if(Value_CH5>1500){
     Modo_Boton_ON(Value_CH3,Value_CH4);
  }
  else{
     Modo_Boton_OFF(Value_CH3,Value_CH4);
  }
  /*
  if(Value_CH3 > 1400 && Value_CH3 < 1600){
    if(Value_CH4 > 1400 && Value_CH4< 1600){
      digitalWrite(Grado_1_arriba, LOW);
      digitalWrite(Grado_1_abajo, LOW);
      digitalWrite(Grado_2_arriba, LOW);
      digitalWrite(Grado_2_abajo, LOW);
      digitalWrite(Gripper_abre,LOW);
      digitalWrite(Gripper_cierra,LOW);
    }
    
    else if(Value_CH4 < 1199){
      digitalWrite(Grado_1_arriba, HIGH);
      digitalWrite(Grado_1_abajo, LOW);
    }

    else if(Value_CH4 > 1899){
      digitalWrite(Grado_1_arriba, LOW);
      digitalWrite(Grado_1_abajo, HIGH);
    }
  }

  
  else if(Value_CH3 < 1399){
    if(Value_CH4 > 1400 && Value_CH4 < 1600){
      digitalWrite(Grado_2_arriba, LOW);
      digitalWrite(Grado_2_abajo, LOW);
     
      if(Value_CH3 < 1080){
        digitalWrite(Gripper_abre, HIGH);
        digitalWrite(Gripper_cierra, LOW);
      }
    }
    
    else if(Value_CH4 < 1200){
      digitalWrite(Grado_2_arriba, HIGH);
      digitalWrite(Grado_2_abajo, LOW);
      digitalWrite(Gripper_abre,LOW);
      digitalWrite(Gripper_cierra,LOW);
    }

    else if(Value_CH4 > 1700){
      digitalWrite(Grado_2_arriba, LOW);
      digitalWrite(Grado_2_abajo, HIGH);
      digitalWrite(Gripper_abre,LOW);
      digitalWrite(Gripper_cierra,LOW);
    }
  }

 else if(Value_CH3 > 1601){
    if(Value_CH4 > 1400 && Value_CH4 < 1600){
      digitalWrite(Grado_2_arriba, LOW);
      digitalWrite(Grado_2_abajo, LOW);

      if(Value_CH3 > 1920){
        digitalWrite(Gripper_abre,LOW);
        digitalWrite(Gripper_cierra,HIGH);
      }
    }
    
    else if(Value_CH4 < 1200){
      digitalWrite(Grado_2_arriba, HIGH);
      digitalWrite(Grado_2_abajo, LOW);
      digitalWrite(Gripper_abre,LOW);
      digitalWrite(Gripper_cierra,LOW);
    }

    else if(Value_CH4 > 1700){
      digitalWrite(Grado_2_arriba, LOW);
      digitalWrite(Grado_2_abajo, HIGH);
      digitalWrite(Gripper_abre,LOW);
      digitalWrite(Gripper_cierra,LOW);
    }
  }
*/
}