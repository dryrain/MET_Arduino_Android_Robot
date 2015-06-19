//Variables de estado
int estado_triangulo=0;  //maquina de estados del triangulo.
int estado_cuadrado=0;   //maquina de estados del cuadrado.
int estado_adel=0;       // maquina de estados de la función adelante_dist.
int estado_atras=0;      // maquina de estados de la función atras_dist.
int estado_giro=0;       // maquina de estado de giro.
int estado_circ=0;       // maquina de estado de circulo.
int estado_mov_autom=0;

unsigned long timer_1;   // temporizado adelante y atras
unsigned long timer_4;   // temporizado giro.
unsigned long timer_6;   // temporizado circulo.

const int mLOW = 1;      // Velocidad baja.
const int mMEDIUM = 2;   // Velocidad media.
const int mHIGH = 3;     // Velocidad alta.

const int mLEFT = 0;     // servo/giro izquierda.
const int mRIGHT = 1;    // servo/giro derecha.

void parar(void){
// Para los motores de movimiento.
  moverServo(mLEFT,0);
  moverServo(mRIGHT,0); 
}

void mover(int velocidad, int giro){
// Funcion para mover el robot hacia delante, atras y con giro.
// Entrada: (int). velocidad: Parado (0), Adelante (1, 2 y 3) y atras (4, 5 y 6)
//          (int). giro: sin giro (1), derecha lento (2), derecha rapido(3), izquierda lento (4), izquierda rapido (5),
//                       giro derecha para coger pendiente (8), giro izquierda para coger pendiente(9).
//                       giro derecha para bajar pendiente (10) y giro izquierda para baja pendiente (11).
  int velocidad_derch=0;
  int velocidad_izq=0;
  switch (velocidad){ //Maquina de estados
     case 0:  
        velocidad_derch=0;
        velocidad_izq=0;
     break;
     case 1:  
        velocidad_derch=19;
        velocidad_izq=15;
        
     break;
     case 2:          
        if(giro==1){  
              velocidad_derch=29;
              velocidad_izq=23;
       }else if(giro==2){
              velocidad_derch=20;
              velocidad_izq=23;              
       }else if(giro==3){
              velocidad_derch=14;
              velocidad_izq=23;              
       }else if(giro==4){
              velocidad_derch=29;
              velocidad_izq=19;
       }else if(giro==5){
              velocidad_derch=29;
              velocidad_izq=13;        
       } 
       else if(giro==6){
              velocidad_derch=90;
              velocidad_izq=-90;        
       }
     break;
     case 3:
       if(giro==1){  
              velocidad_derch=90;
              velocidad_izq=90;
       }else if(giro==2){
              velocidad_derch=25;
              velocidad_izq=90;
       }else if(giro==3){
              velocidad_derch=15;
              velocidad_izq=90;
       }else if(giro==4){
              velocidad_derch=90;
              velocidad_izq=25;
       }else if(giro==5){
              velocidad_derch=90;
              velocidad_izq=15; 
       }else if(giro==6){
              velocidad_derch=-40;
              velocidad_izq=90; 
       }else if(giro==7){
              velocidad_derch=90;
              velocidad_izq=-40; 
       } 
       
     break;
     case 4:  
        velocidad_derch=-15;
        velocidad_izq=-20;
     break;
     case 5:  
        velocidad_derch=-25;
        velocidad_izq=-29;
     break;
     case 6:  
        if(giro==1){  
              velocidad_derch=-90;
              velocidad_izq=-90;
       }else if(giro==2){
              velocidad_derch=-90;
              velocidad_izq=-25;
       }else if(giro==3){
              velocidad_derch=-90;
              velocidad_izq=-15;
       }else if(giro==4){
              velocidad_derch=-25;
              velocidad_izq=-90;
       }else if(giro==5){
              velocidad_derch=-15;
              velocidad_izq=-90; 
       }       
     break;
     
   }
   if(giro==8){ //Giro derecha para coger la pendiente
              velocidad_derch=15;
              velocidad_izq=0; 
   }else if(giro==9){ //Giro izquierda para coger la pendiente
              velocidad_derch=0;
              velocidad_izq=15; 
    
  }else if(giro==10){ //Giro derecha bajando pendiente
              velocidad_derch=15;
              velocidad_izq=10; 
  }else if(giro==11){ //Giro izquierad bajando pendiente
              velocidad_derch=10;
              velocidad_izq=15; 
  }  
   
    moverServo(mLEFT,velocidad_izq);     // Mueve motor izquierdo
    moverServo(mRIGHT,velocidad_derch);  // Mueve motor derecho
}
  
void adelante(int velocidad){
// Funcion para mover los dos motores a la vez para ir hacia adelante.
// Entrada: (int) velocidad, Se introduce la velocidad del robot (-90..90)
    moverServo(mLEFT,velocidad);
    moverServo(mRIGHT,velocidad); 
}

bool adelante_dist(int velocidad,int distancia){
//Mover los 2 motores hacia adelante a una velocida y a cierta distancia
//entradas:
// velociad: 0-STOP, 2-MEDIA VELOCIDAD y 3-ALTA VELOCIDAD
// distancia: distancia a recorre en cm. 
//salida:
// True: ha llegado a la distancia. False. Aun no ha llegado.

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

bool gira(int direc,int velocidad, int angulo){
//Mover los 2 motores hacia adelante y atras con un angulo de giro (0º-90º)
//entradas:
// direccion: (int) 0 (Left) 1 (Right).
// velociad: (int) 0-STOP, 2-MEDIA VELOCIDAD y 3-ALTA VELOCIDAD
// angulo: int (0-360º).
//salida:
// True: ha llegado al angulo indicado. False. Aun no ha llegado.
   
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
                    angulo_cont=float(angulo)*(float(2600)/float(360)); //convertimos delay en angulo    2900
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

bool triangulo(int velocidad,int lado){ 
//Funcion para moverse en forma de triangulo.
//entradas:
// velociad: (int) 0-STOP, 2-MEDIA VELOCIDAD y 3-ALTA VELOCIDAD.
// lado: (int) distancia en cm del lado del triangulo.
//salida:
// True: ha realizado la figura. False. Aun no ha finalizado la figura.
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
bool cuadrado(int velocidad, int lado){ 
//Funcion para moverse en forma de cuadrado.
//entradas:
// velociad: (int) 0-STOP, 2-MEDIA VELOCIDAD y 3-ALTA VELOCIDAD.
// lado: (int) distancia en cm del lado del cuadrado.
//salida:
// True: ha realizado la figura. False. Aun no ha finalizado la figura.
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
//Funcion para moverse en forma de circunferencia.
// salida:
// True: ha realizado la figura. False. Aun no ha finalizado la figura.  
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

bool Freno_Activado(int velocidad_actual,int velocidad_anterior){
//Funcion para detectar si se ha bajado la velocidad y se ha activado el freno.
//Entradas: (int). velocidad_actual. Velocidad que actualmente se ha forzado el robot.
//          (int). velocidad_anterior. Velocidad del robot ants de realizar el canvio de velocidad.
//Salidas: True: Si se a bajado la velocidad. False: si no se ha bajado la velocidad.
  
  if(abs(velocidad_actual)<abs(velocidad_anterior)){
    return true;
  }else{
    return false;
  }
}

