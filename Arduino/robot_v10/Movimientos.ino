int estado_triangulo=0; //maquina de estados del triangulo
int estado_cuadrado=0; //maquina de estados del cuadrado
int estado_adel=0; // maquina de estados de la función adelante_dist.
int estado_atras=0; // maquina de estados de la función atras_dist.
int estado_giro=0; // maquina de estado de giro
int estado_circ=0; // maquina de estado de circulo

unsigned long timer_1; // temporizado adelante y atras
unsigned long timer_4; // temporizado giro.
unsigned long timer_6; // temporizado circulo.

const int mLOW = 1; // Velocidad baja
const int mMEDIUM = 2; // Velocidad media
const int mHIGH = 3; // Velocidad alta

const int mLEFT = 0; // servo/giro izquierda
const int mRIGHT = 1; // servo/giro derecha

//Parar ambos servos 360º
void parar(void){
  moverServo(mLEFT,0);
  moverServo(mRIGHT,0); 

}

//Mover los dos motores a la vez para ir hacia adelante.
void adelante(int velocidad)
{
    moverServo(mLEFT,velocidad);
    moverServo(mRIGHT,velocidad); 
}

//Mover los 2 motores hacia adelante a una velocida y a cierta distancia
//entradas:
// velociad: 0-STOP, 2-MEDIA VELOCIDAD y 3-ALTA VELOCIDAD
// distancia: distancia a recorre en cm. 
//salida:
// True: ha llegado a la distancia. False. Aun no ha llegado.
bool adelante_dist(int velocidad,int distancia) 
{
  float distancia_cont;
  unsigned long timer_2=millis();
    
  switch (estado_adel){
     case 0:  
        timer_1=millis();
        estado_adel=1;       
        return false;
     break;
     case 1:        
        if(velocidad==0 or distancia==0){ // si la velocidad es 0 se para el robot.
           parar();   
           estado_adel=0;        
           return true;  
        }else{          
          if(velocidad==mHIGH){             
             distancia_cont=float(distancia)*(float(3100)/float(50));
             moverServo(mLEFT,90);
             moverServo(mRIGHT,90);                     
          }else if (velocidad==mMEDIUM){            
             distancia_cont=float(distancia)*(float(7400)/float(50));
             moverServo(mLEFT,11);
             moverServo(mRIGHT,14); 
           }
          if(timer_2>(timer_1+distancia_cont)){
            parar(); // despues del recorrido paramos el robot 
            estado_adel=0;
            return true;
          }else{
            return false;
          }          
        }
     break;     
  }
}

//Mover los dos motores a la vez para ir hacia atras.
void atras(int velocidad)
{
  moverServo(mLEFT,-velocidad);
  moverServo(mRIGHT,-velocidad);

}

//Mover los 2 motores hacia detras a una velocida y a cierta distancia
//entradas:
// velociad: 0-STOP, 2-MEDIA VELOCIDAD y 3-ALTA VELOCIDAD
// distancia: distancia a recorre en cm. 
//salida:
// True: ha llegado a la distancia. False. Aun no ha llegado.
bool atras_dist(int velocidad,int distancia) //Distancia en cm.
{
  float distancia_cont;
  unsigned long timer_2=millis();
  switch (estado_atras){ //Maquina de estados
     case 0:  
        timer_1=millis();
        estado_atras=1;
        return false;
     break;
     case 1:        
        if(velocidad==0 or distancia==0){ // si la velocidad es 0 o la distancia es 0 se para el robot.
           parar();           
           return true;  
        }else{          
          if(velocidad==mHIGH){
             distancia_cont=float(distancia)*(float(3100)/float(50));
             moverServo(mLEFT,-90);
             moverServo(mRIGHT,-90); 
          }else if (velocidad==mMEDIUM){
             distancia_cont=float(distancia)*(float(7400)/float(50));
             moverServo(mLEFT,-14);
             moverServo(mRIGHT,-11); 
          }
          if(timer_2>(timer_1+distancia_cont)){
            parar(); // despues del recorrido paramos el robot 
            estado_atras=0;
            return true;
          }else{
            return false;
          }          
        }
     break;     
  }  
}

bool gira(int direc,int velocidad, int angulo){ //dirección: 0 (Left) 1 (Right). velocidad: 0 (stop) 1 (low) 2 (medium) 3 (High), angulo (0-360º)
   
  float angulo_cont;
  unsigned long timer_3=millis();
  switch (estado_giro){ //Maquina de estados
     case 0:  
        timer_4=millis();
        estado_giro=1;
        return false;
     break;
    case 1:  
    
        switch (velocidad){
           case mHIGH:                
                if(direc==mRIGHT){        
                    angulo_cont=float(angulo)*(float(2900)/float(360)); //convertimos delay en angulo    
                    moverServo(mRIGHT,-90); 
                    moverServo(mLEFT,90);
                }else if (direc==mLEFT){
                    angulo_cont=float(angulo)*(float(2800)/float(360)); //convertimos delay en angulo           
                    moverServo(mRIGHT,90); 
                    moverServo(mLEFT,-90);
                }                
           break;	
           case mMEDIUM:	                
                if(direc==mRIGHT){
                    angulo_cont=float(angulo)*(float(7600)/float(360)); //convertimos delay en angulo
                    moverServo(mRIGHT,-10); 
                    moverServo(mLEFT,10);
                    
                }else if (direc==mLEFT){ 
                    angulo_cont=float(angulo)*(float(7600)/float(360)); //convertimos delay en angulo
                    moverServo(mRIGHT,13); 
                    moverServo(mLEFT,-13);
                }                
           break;
         }
         if(timer_3>(timer_4+angulo_cont)){
            parar(); // despues del recorrido paramos el robot 
            estado_giro=0;
            return true;
          }else{
            return false;
          } 
      break;         
   }
}




bool triangulo(int velocidad,int lado) //lado es la distancia en centrimetros del lado del triangulo
{
  switch (estado_triangulo){
     case 0:
       if(adelante_dist(velocidad,lado)){ 
         estado_triangulo=1;         
         return false;
       }     
     break;
     case 1:
       if(gira(mLEFT,velocidad,120)){ 
         estado_triangulo=2;         
         return false;
       }    
     break;    
     case 2:
       if(adelante_dist(velocidad,lado)){         
         estado_triangulo=3; 
         return false;
       }    
     break;
     case 3:
       if(gira(mLEFT,velocidad,120)){
         estado_triangulo=4;         
         return false;
       }      
     break;
     case 4:
       if(adelante_dist(velocidad,lado)){         
         estado_triangulo=0;         
         return true;
       }   
     break; 
  }
  
}
bool cuadrado(int velocidad, int lado) //lado es la distancia en centrimetros del lado del triangulo
{
  switch (estado_cuadrado){
     case 0:
       if(adelante_dist(velocidad,lado)){ 
         estado_cuadrado=1;         
         return false;
       }     
     break;
     case 1:
       if(gira(mLEFT,velocidad,90)){ 
         estado_cuadrado=2;         
         return false;
       }    
     break;    
     case 2:
       if(adelante_dist(velocidad,lado)){         
         estado_cuadrado=3; 
         return false;
       }    
     break;
     case 3:
       if(gira(mLEFT,velocidad,90)){
         estado_cuadrado=4;         
         return false;
       }      
     break;
     case 4:
       if(adelante_dist(velocidad,lado)){         
         estado_cuadrado=5;         
         return false;
       }
     break;
     case 5:
       if(gira(mLEFT,velocidad,90)){ 
         estado_cuadrado=6;         
         return false;
       }    
     break;    
     case 6:
       if(adelante_dist(velocidad,lado)){         
         estado_cuadrado=0; 
         return true;
       }    
     break;    
  }
  
}

bool circunferencia(void){
  
  unsigned long timer_5=millis();
  switch (estado_circ){
     case 0:  
        timer_6=millis();
        estado_circ=1;       
        return false;
     break;
     case 1:        
          
          moverServo(mLEFT,90);
          moverServo(mRIGHT,15);           
          if(timer_5>(timer_6+14000)){
            parar(); // despues del recorrido paramos el robot 
            estado_circ=0;
            return true;
          }else{
            return false;
          }  
      break;        
    }
   
}



