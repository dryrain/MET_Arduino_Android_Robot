 const int DERCH =  1;           // Constante para identificar el lado derecho.  
 const int IZQ =    0;           // Constante para identificar el lado izquierdo.
 const int RECT =   2;           // Constante para identificar la que hay en la posicion recta.
 bool laberintoSolucionado=false;
 
 unsigned int sensorValues[6];   // Variable donde se guardan los datos recogidos por el sensor de infrarojos.
 int ContadorLinea=0;            // Contador para buscar el final de linea y el final del laberinto.
 int estadoLaberinto=0;          // Variable de maquina de estados de la funcion ModoLaberinto (motor del laberinto).
 int Laberinto[11][11];          // Matriz del laberinto donde se guarda el recorrido.  
 int LaberintoSol[11][11];       // Matriz del laberinto donde se guarda la solucion.
 int LaberintoTotal[11][11];     // Matriz del laberinto enviado a Telefono.
 struct Robot_L{                 // Estructura donde se guarda la posición y la dirección del robot en el laberinto
   int  DirX=1;                  // Vector x de la direccion.
   int  DirY=0;                  // Vector y de la direccion.
   int  PosX=1;                  // Vector x de la posicion.
   int  PosY=1;                  // Vector y de la posicion.
 };
 Robot_L RobL;                  // Declaracion de la estructura para guardar la informacion del robot en el laberinto.
 int  oPosX=1;                  // Posicion anterior.
 int  oPosY=1;                  // Posicion anterior.
 int  sPosX=1;                  // Posicion salida.
 int  sPosY=1;                  // Posicion salida.

int CambiaEstado(int estado){  
// Funcion para cambiar el estado de la maquina de estados de la función ModoLaberinto e reiniciar el contador de linea.
//   entrada: estado(int). Estado al que se desa llegar.
//   salida: (int). Devuelve el estado cambiado.
    ContadorLinea=0;
    estadoLaberinto=estado;
    return estado;
}  

int buscaSalida(unsigned int* sensorV){
// Funcion para encontrar un cruce y detectar si el robot a llegado a la salida.
//   entrada: sensorV (*int). Tabla del laberinto.
//   salida: (int). 0= está buscando la salida o el final del cruce. 1= ha encontrado un cruce. 2= h encontrado la salida. 
       if((sensorV[5]>400)&&(sensorV[0]>400)){  // Condicion para saber si ha encontrado linea o final de laberinto.
          moverServo(DERCH,10);
          moverServo(IZQ,7);
          ContadorLinea=ContadorLinea+1;        // Aumentamos un contador para ver si la línea es muy ancha, lo cual indica que es la salida. 
          if(ContadorLinea==600) return 2;      // Ha encontrado el final del laberinto
          return 0;
          }else{
             if(sensorV[0]<400){
                moverServo(IZQ,0);
             }else{ 
                return 0; 
             } 
             if(sensorV[5]<400){
                moverServo(DERCH,0);
             }else{ 
               return 0;
             } 
             if((sensorV[5]<400)&&(sensorV[0]<400)) return 1;
          }
} 

bool buscarCruce(unsigned int* sensorV){
//Esta funcion sigue en linea recta hasta encontrar un cruce. Utilizaremos los sensores para seguir recto.
// entrada: sensorV (int*). Valor recogido por los sensores infrarrojos.
// salida: (bool). Devuelve True si ha encontrado una interseccion, y False si aun no ha encontrado nada.
          if((sensorV[0]>1500 && sensorV[5]>1500) &&sensorV[2]>1500  ){  
             moverServo(IZQ,0);
             moverServo(DERCH,0);
             return true;  
          }else{
              if(((sensorV[2]>500 && sensorV[3]>500))){
                 moverServo(DERCH,10);
                 moverServo(IZQ,7);
              }else if((sensorV[2]>500 && sensorV[1]>500)){
                 moverServo(DERCH,12);
                 moverServo(IZQ,6);
              }else if((sensorV[2]>1000)){
                 moverServo(DERCH,13);
                 moverServo(IZQ,6);
              }else if((sensorV[3]>500 && sensorV[4]>500)){
                 moverServo(DERCH,9);
                 moverServo(IZQ,11);
              }else if(sensorV[3]>1000){
                 moverServo(DERCH,9);
                 moverServo(IZQ,12);
              }else{
                 moverServo(DERCH,10);
                 moverServo(IZQ,7);
              }
          } 
          return false;
}  

int BuscaPared(int posX, int posY, int direcX, int direcY, int lado){
// Funcion para detectar (en la matriz del laberinto) si hay pared o no o si aun no se ha detectado pared.
// entradas:
//   posX: (int). posicion x del robot.
//   posY: (int). posicion y del robot.
//   direcX: (int). vector posicion x del robot.
//   direcY: (int). vector posicion y del robot.
//   lado: (int). 2:recto, 0:izquierda y 1:derecha.
// salida: (int). 2: hay pared, 0: primera vez que se detecta (hay que revisar la pared. 1: no hay pared. 3:camino de salida mas corto según crokis realizado por robot

  switch(lado){
     case 2: // mirar de frente.
          return Laberinto[posY+direcY][posX+direcX];
     break;
     case 0: // mirar de izquierdo.
          return Laberinto[posY+direcX][posX-direcY];
     break;
     case 1: // mirar de derecho.
          return Laberinto[posY-direcX][posX+direcY];
     break;
  } 
}

int BuscaParedTotal(int posX, int posY, int direcX, int direcY, int lado){
// Funcion para detectar (en la matriz de total laberinto) si hay pared o no o si aun no se ha detectado pared o a encontrado ruta calculada.
// entradas:
//   posX: (int). posicion x del robot.
//   posY: (int). posicion y del robot.
//   direcX: (int). vector posicion x del robot.
//   direcY: (int). vector posicion y del robot.
//   lado: (int). 2:recto, 0:izquierda y 1:derecha.
// salida: 2: hay pared, 0: primera vez que se detecta (hay que revisar la pared. 1: no hay pared. 3:camino de salida mas corto según crokis realizado por robot
  switch(lado){
     case 2: // mirar de frente.
          return LaberintoTotal[posY+direcY][posX+direcX];
     break;
     case 0: // mirar de izquierdo.
          return LaberintoTotal[posY+direcX][posX-direcY];
     break;
     case 1: // mirar de derecho.
          return LaberintoTotal[posY-direcX][posX+direcY];
     break;
  } 
}

void ActualizaSolLab(void){
//Funcion para actualizar la matriz a enviar al telefono movil, a partir de la matriz del recorrido y la matriz solucion se construye la matriz total
//  para enviarla al telf y asi poder dibujar los estados descartados y solucion. Utilizamos el algoritmos de mano izq para encontrar el camino mas corto segun
//  solucion encontrada.

  Robot_L RobLSol;         // Declaracion de la estructura para guardar la informacion del robot en el laberinto
  int  oPosXSol=1;         // Posicion anterior.
  int  oPosYSol=1;         // Posicion anterior.
  int Hay_pared_Sol;        
  RobLSol.PosX=1;          // Inicializa la poscicion x del robot.
  RobLSol.PosY=1;          // Inicializa la poscicion y del robot.
  RobLSol.DirX=1;          // Inicializa la direccion x del robot.
  RobLSol.DirY=0;          // Inicializa la direccion y del robot.
  ini_Laberinto_Solucion();//Inicializa la tabla de la solucion al estado inicial.
  do{         
                Hay_pared_Sol=BuscaPared(RobLSol.PosX,RobLSol.PosY,RobLSol.DirX,RobLSol.DirY,IZQ);    // el robot mira a la izq para ver si hay solucion.
                if(Hay_pared_Sol==1){
                       oPosXSol=RobLSol.PosX;
                       oPosYSol=RobLSol.PosY;
                       LaberintoSol[oPosYSol][oPosXSol]=3;          //Indicamos que en la posicion actual ya hemos pasado.
                       LaberintoSol[RobLSol.PosY+(RobLSol.DirX)][RobLSol.PosX-(RobLSol.DirY)]=3;
                       RobLSol.PosX=RobLSol.PosX-(RobLSol.DirY*2);
                       RobLSol.PosY=RobLSol.PosY+(RobLSol.DirX*2);
                       
                       LaberintoSol[RobLSol.PosY][RobLSol.PosX]=9;  //Indicamos la nueva posicion a donde se dirige el robot.
                       RobLSol.DirX=(RobLSol.PosX-oPosXSol)/2;
                       RobLSol.DirY=(RobLSol.PosY-oPosYSol)/2;
                 }else{
                       Hay_pared_Sol=BuscaPared(RobLSol.PosX,RobLSol.PosY,RobLSol.DirX,RobLSol.DirY,RECT); // el robot mira recto para ver si hay solucion.
                       if(Hay_pared_Sol==1){
                            oPosXSol=RobLSol.PosX;
                            oPosXSol=RobLSol.PosX;
                            oPosYSol=RobLSol.PosY;
                            LaberintoSol[oPosYSol][oPosXSol]=3;         //Indicamos que en la posicion actual ya hemos pasado.
                            LaberintoSol[RobLSol.PosY+(RobLSol.DirY)][RobLSol.PosX+(RobLSol.DirX)]=3;
                            RobLSol.PosX=RobLSol.PosX+(RobLSol.DirX*2);
                            RobLSol.PosY=RobLSol.PosY+(RobLSol.DirY*2);
                            LaberintoSol[RobLSol.PosY][RobLSol.PosX]=9; //Indicamos la nueva posicion a donde se dirige el robot.
                            RobLSol.DirX=(RobLSol.PosX-oPosXSol)/2;
                            RobLSol.DirY=(RobLSol.PosY-oPosYSol)/2;
                         }else{
                            Hay_pared_Sol=BuscaPared(RobLSol.PosX,RobLSol.PosY,RobLSol.DirX,RobLSol.DirY,DERCH); // el robot mira a la derech para ver si hay solucion.
                            if(Hay_pared_Sol==1){
                                oPosXSol=RobLSol.PosX;
                                oPosXSol=RobLSol.PosX;
                                oPosYSol=RobLSol.PosY;
                                LaberintoSol[oPosYSol][oPosXSol]=3;          //Indicamos que en la posicion actual ya hemos pasado.
                                LaberintoSol[RobLSol.PosY-(RobLSol.DirX)][RobLSol.PosX+(RobLSol.DirY)]=3;
                                RobLSol.PosX=RobLSol.PosX+(RobLSol.DirY*2);
                                RobLSol.PosY=RobLSol.PosY-(RobLSol.DirX*2);
                                LaberintoSol[RobLSol.PosY][RobLSol.PosX]=9;  //Indicamos la nueva posicion a donde se dirige el robot.
                                RobLSol.DirX=(RobLSol.PosX-oPosXSol)/2;
                                RobLSol.DirY=(RobLSol.PosY-oPosYSol)/2;
                              }                
                           
                         }
                 }
  }while(!((RobLSol.PosX==RobL.PosX)&&(RobLSol.PosY==RobL.PosY)));  // Realiza el bucle hasta llegar a la posicion del robot
}
void ini_posicion_robot(void){
// Funcion para inicializar la posicion del robot al inicio del laberinto.
//
  RobL.PosX=1;
  RobL.PosY=1;
  RobL.DirX=1;
  RobL.DirY=0;
}
void ini_Laberinto(void){ 
// Funcion para inicializar la matriz donde se visualiza el recorrido del robot.
  ini_posicion_robot();
  int i,j;
  for(j=0;j<11;j++)
  {
    for(i=0;i<11;i++){
      if(((i==0)||(j==0))||((j==10)||(i==10))){
        Laberinto[j][i]=2;
        LaberintoSol[j][i]=2;
      }else if(((i%2)==0)&&((j%2)==0)){ 
        Laberinto[j][i]=2;
        LaberintoSol[j][i]=2;
       }else{
         Laberinto[j][i]=0;
         LaberintoSol[j][i]=0;
       }
     }    
  }
  Laberinto[RobL.PosY][RobL.PosX]=9;
  LaberintoSol[1][1]=9;
}

void ini_Laberinto_Solucion(void){
// Funcion para inicializar la matriz donde se visualiza la solucion del robot.
  int i,j;
  for(j=0;j<11;j++)
  {
    for(i=0;i<11;i++){
         LaberintoSol[j][i]=0;   
     }    
  }
  LaberintoSol[1][1]=9;
}
void ini_Laberinto_Total(void){
// Funcion para inicializar la matriz donde se visualiza la solucion, recorrido y descartados del robot. Es la matriz que se envia al telefono.
  int i,j;
  for(j=0;j<11;j++)
  {
    for(i=0;i<11;i++){
         LaberintoTotal[j][i]=0; 
     }    
  }
  LaberintoTotal[1][1]=9;
}

void Crea_Matriz_Total(void){
// Funcion para construir la matriz que se envia al telf, a partir de la matriz de recorrido y solucion.
int i,j;
  for(j=0;j<11;j++)
  {
    for(i=0;i<11;i++){
         if (Laberinto[j][i]==9){   
             LaberintoTotal[j][i]=9;   
         }else {
           LaberintoTotal[j][i]=LaberintoSol[j][i]+Laberinto[j][i];
           if(LaberintoTotal[j][i]==4) LaberintoTotal[j][i]=3;
         }
     }    
  }
}

void Modo_Laberinto(void){
// Funcion para reslover el reto del laberinto.
// Consta de una maquina de estados con 3 bloques diferenciados.
// Maquina de estado para buscar la salida del laberinto.
  // 0. INICIO POR PRIMERA VEZ EL LABERINTO.
  // 1. BUSCA EL FINAL DE INTERSECCION O LA SALIDA DEL RECORRIDO.
  // 2. BUSCA LA SIGUIENTE INTERSECCION.
  // 3. COMPRUEBA SI HAY PARED A LA DERECHA.
  // 4. COMPRUEBA SI HAY PARED EN FRENTE. 
  // 5. COMPRUEBA SI HAY PARED A LA IZQUIERDA.    
// Maquina de estado para realizar los giros del robot.  
  // 11. GIRA A LA DERECHA Y SIGUE DE FRENTE.
  // 12. GIRA A LA IZQUIERDA Y SIGUE DE FRENTE.
  // 13. GIRA 180º Y SIGUE DE FRENTE.
// Maquina de estados para seguir la solucion propuesta.  
  // 100. BUSCA EL FINAL DE CRUCE O LA SALIDA DEL LABERINTO.
  // 101. BUSCA LA INTERSECCION Y SE PARA.
  // 102. COMPRUEBA SI HAY CAMINO RAPIDO EN LA DIRECCION IZQUIERDA.
  // 103. COMPRUEBA SI HAY CAMINO RAPIDO EN LA DIRECCION FRONTAL. 
  // 104. COMPRUEBA SI HAY CAMINO RAPIDO EN LA DIRECCION DERECHA.
    
               int Salida_encontrada;         // Variable donde se almacena si ha encontrado la salida: 2=Salida encontra, 1=Cruce encontrado y 0=buscando salida o cruce.
               int Hay_pared;                 // Variable donde guarda el valor si hay pared o hay camino.
               Lee_Infrarojos(sensorValues);  // Lee el sensor de infrarojos y devuelve el valor en la variable sensorValues.
               
         
               if(estadoLaberinto==0){                        // INICIO POR PRIMERA VEZ EL LABERINTO.
                  if(Estado.FuncionLaberinto=='N'){           // Entra en la funcion busca salida.
                      laberintoSolucionado=false;
                      ini_Laberinto();
                      ini_Laberinto_Solucion();
                      ini_Laberinto_Total();
                      estadoLaberinto=CambiaEstado(1);
                   }else if(Estado.FuncionLaberinto=='S'){    // Entra en la funcion de sigue recorrido solucionado
                      ini_posicion_robot();                   // Inicializamos el robot en la posicion inicial.
                      LaberintoTotal[RobL.PosY][RobL.PosX]=9; // Dibujamos el robot en la posición inicial.
                      estadoLaberinto=CambiaEstado(100);                      
                   }                                                   
               }               
               if(estadoLaberinto==1){                            // BUSCA EL FINAL DE INTERSECCION O LA SALIDA DEL RECORRIDO.
                     Salida_encontrada=buscaSalida(sensorValues);
                     if(Salida_encontrada==1){                    //Encontrado CRUCE!
                        // flagLaberinto=true; 
                         estadoLaberinto=CambiaEstado(3);
                         ActualizaSolLab();
                         Crea_Matriz_Total();
                         sendLaberynthUDP('N'); 
                     }else if(Salida_encontrada==2){               // Ha encontrado la salida.
                        Laberinto[RobL.PosY][RobL.PosX]=9;         // Indicamos la posición como la salida.
                        LaberintoSol[RobL.PosY][RobL.PosX]=9;      // Indicamos la posición como la salida.
                        adelante(0);                               // Se frena el robot al encontrar la salida.                        
                        ActualizaSolLab();
                        Crea_Matriz_Total();                        
                        sPosX=RobL.PosX;
                        sPosY=RobL.PosY;                        
                        estadoLaberinto=CambiaEstado(200);         // Se va al estado de stand-bye.                
                        sendLaberynthUDP('S');
                        laberintoSolucionado=true;
                     }                                            
               }
               if(estadoLaberinto==2){                             // BUSCA LA SIGUIENTE INTERSECCION.
                     if( buscarCruce(sensorValues)==true){
                        estadoLaberinto=CambiaEstado(1);                    
                     }  
               } 
               if(estadoLaberinto==3){                             // COMPRUEBA SI HAY PARED A LA DERECHA.                    
                     Hay_pared=BuscaPared(RobL.PosX,RobL.PosY,RobL.DirX,RobL.DirY,DERCH);             
                     if(Hay_pared==0){   
                          if(detectaDerecha()){               
                                 Laberinto[RobL.PosY-RobL.DirX][RobL.PosX+RobL.DirY]=2;
                                 estadoLaberinto=CambiaEstado(4);                            
                          }else{
                                Laberinto[RobL.PosY-RobL.DirX][RobL.PosX+RobL.DirY]=1;   // Indicamos en la matriz que no hay pared. 
                          }
                     }else if(Hay_pared==1){              
                        Laberinto[RobL.PosY-RobL.DirX][RobL.PosX+RobL.DirY]=1;           // Indicamos en la matriz que no hay pared.                         
                     }else{                                                              // Como hay pared va al estado para comprobar la pared frontal.
                       estadoLaberinto=CambiaEstado(4);             
                     }
                     if(Laberinto[RobL.PosY-RobL.DirX][RobL.PosX+RobL.DirY]==1){         
                            oPosX=RobL.PosX;
                            oPosY=RobL.PosY;
                            Laberinto[oPosY][oPosX]=1;                                   // Indicamos que en la posicion actual ya hemos pasado.
                            RobL.PosX=RobL.PosX+(RobL.DirY*2);
                            RobL.PosY=RobL.PosY-(RobL.DirX*2);
                            Laberinto[RobL.PosY][RobL.PosX]=9;                           // Indicamos la nueva posicion a donde se dirige el robot.
                            RobL.DirX=(RobL.PosX-oPosX)/2;
                            RobL.DirY=(RobL.PosY-oPosY)/2;
                            estadoLaberinto=CambiaEstado(11);
                            if(Hay_pared==1){                                            // Comprueba que no pasemos por un camino ya utilizado si es asi recalcula la solucion optima, si no indica que la casilla es solucion
                              ActualizaSolLab();
                            }else{               
                              Laberinto[oPosY-RobL.DirX][oPosX+RobL.DirY]==1;
                            }                        
                      }                    
               }
               if(estadoLaberinto==4){                                                   // COMPRUEBA SI HAY PARED EN FRENTE.          
                     Hay_pared=BuscaPared(RobL.PosX,RobL.PosY,RobL.DirX,RobL.DirY,RECT);
                        if(Hay_pared==0){                
                        if(detectaCentro()){            
                               Laberinto[RobL.PosY+RobL.DirY][RobL.PosX+RobL.DirX]=2;
                               estadoLaberinto=CambiaEstado(5);
                        }else{
                              Laberinto[RobL.PosY+RobL.DirY][RobL.PosX+RobL.DirX]=1;    // Indicamos en la matriz que no hay pared.                         
                        }
                     }else if(Hay_pared==1){           
                        Laberinto[RobL.PosY+RobL.DirY][RobL.PosX-RobL.DirX]=1;          // Indicamos en la matriz que no hay pared.                         
                     }else{                                                             // como hay pared va al estado para comprobar la pared frontal.
                       estadoLaberinto=CambiaEstado(5);           
                     }
                     if(Laberinto[RobL.PosY+RobL.DirY][RobL.PosX+RobL.DirX]==1){
                            oPosX=RobL.PosX;
                            oPosY=RobL.PosY;
                            Laberinto[oPosY][oPosX]=1;                                   // Indicamos que en la posicion actual ya hemos pasado.
                            RobL.PosX=RobL.PosX+(RobL.DirX*2);
                            RobL.PosY=RobL.PosY+(RobL.DirY*2);
                            Laberinto[RobL.PosY][RobL.PosX]=9;                           //Indicamos la nueva posicion a donde se dirige el robot.
                            RobL.DirX=(RobL.PosX-oPosX)/2;
                            RobL.DirY=(RobL.PosY-oPosY)/2;
                            estadoLaberinto=CambiaEstado(2);          
                      }                    
              }
             if(estadoLaberinto==5){                                                     // COMPRUEBA SI HAY PARED A LA IZQUIERDA.        
                     Hay_pared=BuscaPared(RobL.PosX,RobL.PosY,RobL.DirX,RobL.DirY,IZQ);
                     if(Hay_pared==0){
                       if(detectaIzquierda()){                               
                               Laberinto[RobL.PosY+RobL.DirX][RobL.PosX-RobL.DirY]=2;   
                        }else{
                              Laberinto[RobL.PosY+RobL.DirX][RobL.PosX-RobL.DirY]=1;     // Indicamos en la matriz que no hay pared.   
                        }
                     }else if(Hay_pared==1){           
                        Laberinto[RobL.PosY+RobL.DirX][RobL.PosX-RobL.DirY]=1;           // Indicamos en la matriz que no hay pared.                         
                     }else{                                                              // Como hay pared va al estado para comprobar la pared frontal.
                       estadoLaberinto=CambiaEstado(13); 
                     }
                     if(Laberinto[RobL.PosY+RobL.DirX][RobL.PosX-RobL.DirY]==1){
                            oPosX=RobL.PosX;
                            oPosY=RobL.PosY;
                            Laberinto[oPosY][oPosX]=1;                                   //Indicamos que en la posicion actual ya hemos pasado.
                            RobL.PosX=RobL.PosX-(RobL.DirY*2);
                            RobL.PosY=RobL.PosY+(RobL.DirX*2);
                            Laberinto[RobL.PosY][RobL.PosX]=9;                           //Indicamos la nueva posicion a donde se dirige el robot.
                            RobL.DirX=(RobL.PosX-oPosX)/2;
                            RobL.DirY=(RobL.PosY-oPosY)/2;
                            estadoLaberinto=CambiaEstado(12);           
                      }else if(Laberinto[RobL.PosY+RobL.DirX][RobL.PosX-RobL.DirY]==2){
                            oPosX=RobL.PosX;
                            oPosY=RobL.PosY;
                            Laberinto[oPosY][oPosX]=1;                                   //Indicamos que en la posicion actual ya hemos pasado.
                            RobL.DirX=RobL.DirX*(-1);
                            RobL.DirY=RobL.DirY*(-1);
                            RobL.PosX=RobL.PosX+(RobL.DirX*2);
                            RobL.PosY=RobL.PosY+(RobL.DirY*2);
                            Laberinto[RobL.PosY][RobL.PosX]=9;                           //Indicamos la nueva posicion a donde se dirige el robot.                            
                            estadoLaberinto=CambiaEstado(13); 
                      }  
               } 
               if(estadoLaberinto==11){                                                  // GIRA A LA DERECHA Y SIGUE DE FRENTE.
                        if(gira(DERCH,3,90)){
                          if(Estado.FuncionLaberinto=='N'){                              // Si esta buscando la salida por primera vez.
                             estadoLaberinto=CambiaEstado(2);
                          }else{                                                         // Si esta siguiendo el camino mas corto.
                             estadoLaberinto=CambiaEstado(101);
                          }
                        }             
               } 
               if(estadoLaberinto==12){                                                  // GIRA A LA IZQUIERDA Y SIGUE DE FRENTE.
                        if(gira(IZQ,3,90)){
                          if(Estado.FuncionLaberinto=='N'){                              // Si está buscando la salida por primera vez.
                             estadoLaberinto=CambiaEstado(2);
                          }else{                                                         // Si está siguiendo el camino mas corto.
                             estadoLaberinto=CambiaEstado(101);
                          }
                        }             
               } 
               if(estadoLaberinto==13){                                                  // GIRA 180º Y SIGUE DE FRENTE.
                        if(gira(IZQ,3,180)){
                          estadoLaberinto=CambiaEstado(2);
                        }             
               } 
               if(estadoLaberinto==100){                                                 // BUSCA EL FINAL DE CRUCE O LA SALIDA DEL LABERINTO.
                    
                     Salida_encontrada=buscaSalida(sensorValues);
                     if(Salida_encontrada==1){                                           //Encontrado CRUCE!                        
                         estadoLaberinto=CambiaEstado(102);                       
                         sendLaberynthUDP('S'); 
                     }else if(Salida_encontrada==2){                                     // Ha encontrado la salida.
                        LaberintoTotal[RobL.PosY][RobL.PosX]=9;                          // Indicamos la posición como la salida.                     
                        adelante(0);                                                     // Se frena el robot al encontrar la salida.                        
                        sPosX=RobL.PosX;
                        sPosY=RobL.PosY;               
                        estadoLaberinto=CambiaEstado(200);                               // Estado siguiente para saber si se ha de seguir el camino                  
                        sendLaberynthUDP('S');
                     }       
               }
               if(estadoLaberinto==101){                                                 // BUSCA LA INTERSECCION Y SE PARA
                     if( buscarCruce(sensorValues)==true){
                        estadoLaberinto=CambiaEstado(100);                  
                     } 
               } 
               if(estadoLaberinto==102){                                                 // COMPRUEBA SI HAY CAMINO RAPIDO EN LA DIRECCION IZQUIERDA.
                     Hay_pared=BuscaParedTotal(RobL.PosX,RobL.PosY,RobL.DirX,RobL.DirY,IZQ);
                     if(Hay_pared==3){
                       oPosX=RobL.PosX;
                       oPosY=RobL.PosY;
                       LaberintoTotal[oPosY][oPosX]=3;                                   //Indicamos que en la posicion actual ya hemos pasado.
                       RobL.PosX=RobL.PosX-(RobL.DirY*2);
                       RobL.PosY=RobL.PosY+(RobL.DirX*2);
                       LaberintoTotal[RobL.PosY][RobL.PosX]=9;                           //Indicamos la nueva posicion a donde se dirige el robot.
                       RobL.DirX=(RobL.PosX-oPosX)/2;
                       RobL.DirY=(RobL.PosY-oPosY)/2;
                       estadoLaberinto=CambiaEstado(12);          
                     }else{
                       estadoLaberinto=CambiaEstado(103);
                     }
               }
               if(estadoLaberinto==103){                                                 // COMPRUEBA SI HAY CAMINO RAPIDO EN LA DIRECCION FRONTAL.
                     Hay_pared=BuscaParedTotal(RobL.PosX,RobL.PosY,RobL.DirX,RobL.DirY,RECT);                     
                     if(Hay_pared==3){
                            oPosX=RobL.PosX;
                            oPosY=RobL.PosY;                            
                            LaberintoTotal[oPosY][oPosX]=3;                              //Indicamos que en la posicion actual ya hemos pasado.
                            RobL.PosX=RobL.PosX+(RobL.DirX*2);
                            RobL.PosY=RobL.PosY+(RobL.DirY*2);                            
                            LaberintoTotal[RobL.PosY][RobL.PosX]=9;                      //Indicamos la nueva posicion a donde se dirige el robot.
                            RobL.DirX=(RobL.PosX-oPosX)/2;
                            RobL.DirY=(RobL.PosY-oPosY)/2;
                            estadoLaberinto=CambiaEstado(101);                            
                     }else{
                       estadoLaberinto=CambiaEstado(104);
                     }
               }
               if(estadoLaberinto==104){                                                 // COMPRUEBA SI HAY CAMINO RAPIDO EN LA DIRECCION DERECHA.
                     Hay_pared=BuscaParedTotal(RobL.PosX,RobL.PosY,RobL.DirX,RobL.DirY,DERCH);                      
                     if(Hay_pared==3){
                            oPosX=RobL.PosX;
                            oPosX=RobL.PosX;
                            oPosY=RobL.PosY;                            
                            LaberintoTotal[oPosY][oPosX]=3;                             //Indicamos que en la posicion actual ya hemos pasado.
                            RobL.PosX=RobL.PosX+(RobL.DirY*2);
                            RobL.PosY=RobL.PosY-(RobL.DirX*2);                            
                            LaberintoTotal[RobL.PosY][RobL.PosX]=9;                     //Indicamos la nueva posicion a donde se dirige el robot.
                            RobL.DirX=(RobL.PosX-oPosX)/2;
                            RobL.DirY=(RobL.PosY-oPosY)/2;
                            estadoLaberinto=CambiaEstado(11);
                     }
               } 
}
