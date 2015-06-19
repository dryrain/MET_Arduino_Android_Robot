int velocidad_derecha1=10;
  int velocidad_izquierda1=7;
  int inc=2;
  int inc1=2;
const int L = 0;           // servo/giro izquierda
const int R = 1;
  int EstadoLab=0;

int CambiaEstado(int estado){
    EstadoLab=0;
    return estado;
}  
int buscaSalida(unsigned int* sensorV){
// Funcion para encontrar un cruce y detectar si el robot a llegado a la salida.
//   entrada: sensorV (*int). Tabla del laberinto.
//   salida: (int). 0= está buscando la salida o el final del cruce. 1= ha encontrado un cruce. 2= h encontrado la salida. 
       if((sensorV[5]>200)&&(sensorV[0]>200)){
          moverServo(R,10);
          moverServo(L,7);
          EstadoLab=EstadoLab+1;   // Aumentamos un contador para ver si la línea es muy ancha, lo cual indica que es la salida. 
          if(EstadoLab==400) return 2;  // Ha encontrado el final del laberinto
          return 0;
          }else{
             if(sensorV[0]<200){
                moverServo(L,0);
             }else{ 
                return 0; 
             } 
             if(sensorV[5]<200){
                moverServo(R,0);
             }else{ 
               return 0;
             } 
             if((sensorV[5]<200)&&(sensorV[0]<200)) return 1;
          }
} 
 
 
bool buscarCruce(unsigned int* sensorV){
  //Esta funcion sigue en linea recta hasta encontrar un cruce. Utilizaremos los sensores para seguir recto
  switch(EstadoLab){
     case 0: //Main Menu     
          if((sensorV[0]>1500 && sensorV[5]>1500) &&sensorV[2]>1500  ){  
          moverServo(L,0);
          moverServo(R,0);
          Serial.println("Parado Cruce"); 
          EstadoLab=1;  
          }else{
              if(((sensorV[2]>500 && sensorV[3]>500))){
                 moverServo(R,10);
                 moverServo(L,7);
              }else if((sensorV[2]>500 && sensorV[1]>500)){
                 moverServo(R,12);
                 moverServo(L,6);
              }else if((sensorV[2]>1000)){
                 moverServo(R,13);
                 moverServo(L,6);
          //    }else if((sensorV[1]>1200)){
         //        moverServo(R,13);
         //        moverServo(L,7);
              }else if((sensorV[3]>500 && sensorV[4]>500)){
                 moverServo(R,9);
                 moverServo(L,11);
              }else if(sensorV[3]>1000){
                 moverServo(R,9);
                 moverServo(L,12);
              }else{
                 moverServo(R,10);
                 moverServo(L,7);
              }
          } 
          return false;
      break;
      case 1:
           if((sensorV[5]>200)&&(sensorV[0]>200)){
          moverServo(R,10);
          moverServo(L,7);
          return false;
          }else{
             if(sensorV[0]<200){
                moverServo(L,0);
             }else{ 
                return false; 
             } 
             if(sensorV[5]<200){
                moverServo(R,0);
             }else{ 
               return false;
             } 
             if((sensorV[5]<200)&&(sensorV[0]<200)) return true;
          }
      break;
  }
}  

int BuscaPared(int posX, int posY, int direcX, int direcY, int lado){
// Funcion para detectar (en la matriz del laberinto) si hay pared o no o si aun no se ha detectado pared.
// entradas:
//   posX: (int). posicion x del robot.
//   posY: (int). posicion y del robot.
//   direcX: (int). vector posicion x del robot.
//   direcY: (int). vector posicion y del robot.
//   lado: (int). 0:recto, 1:izquierda y 2:derecha.
// salida: 2: hay pared, 0: primera vez que se detecta (hay que revisar la pared. 1: no hay pared. 3:camino de salida mas corto según crokis realizado por robot

Serial.print("xpos");
Serial.println(posX);
Serial.print("ypos");
Serial.println(posY);
Serial.print("direcX");
Serial.println(direcX);
Serial.print("direcY");
Serial.println(direcY);
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

void VisualizaMatriz(void)
{
  Serial.println("MAtriz"); 
  int i,j;
  for(j=10;j>=0;j--){
    Serial.println(" ");
    for(i=0;i<11;i++){
      Serial.print(" ");
      Serial.print(Laberinto[j][i]);
    }
  }
}
void VisualizaMatrizSol(void)
{
  Serial.println("MAtriz Solucion"); 
  int i,j;
  for(j=10;j>=0;j--){
    Serial.println(" ");
    for(i=0;i<11;i++){
      Serial.print(" ");
      Serial.print(LaberintoSol[j][i]);
    }
  }
}

  
void ActualizaSolLab(void){
  Robot_L RobLSol;         // Declaracion de la estructura para guardar la informacion del robot en el laberinto
  int  oPosXSol=1;    // Posicion anterior.
  int  oPosYSol=1;    // Posicion anterior.
  int Hay_pared_Sol;
  RobLSol.PosX=1;
  RobLSol.PosY=1;
  RobLSol.DirX=1;
  RobLSol.DirY=0;
  ini_Laberinto_Solucion(); //Inicializa la tabla de la solucion al estado inicial.
  do{
                Hay_pared_Sol=BuscaPared(RobLSol.PosX,RobLSol.PosY,RobLSol.DirX,RobLSol.DirY,IZQ);
                if(Hay_pared_Sol==1){
                       oPosXSol=RobLSol.PosX;
                       oPosYSol=RobLSol.PosY;
                       LaberintoSol[oPosYSol][oPosXSol]=3;          //Indicamos que en la posicion actual ya hemos pasado.
                       RobLSol.PosX=RobLSol.PosX-(RobLSol.DirY*2);
                       RobLSol.PosY=RobLSol.PosY+(RobLSol.DirX*2);
                       LaberintoSol[RobLSol.PosY][RobLSol.PosX]=9;  //Indicamos la nueva posicion a donde se dirige el robot.
                       RobLSol.DirX=(RobLSol.PosX-oPosXSol)/2;
                       RobLSol.DirY=(RobLSol.PosY-oPosYSol)/2;
                  //     estadoLaberinto=CambiaEstado(12);
                       VisualizaMatrizSol();
                 }else{
                       Hay_pared_Sol=BuscaPared(RobLSol.PosX,RobLSol.PosY,RobLSol.DirX,RobLSol.DirY,RECT);
                       if(Hay_pared_Sol==1){
                            oPosXSol=RobLSol.PosX;
                            oPosXSol=RobLSol.PosX;
                            oPosYSol=RobLSol.PosY;
                            LaberintoSol[oPosYSol][oPosXSol]=3;         //Indicamos que en la posicion actual ya hemos pasado.
                            RobLSol.PosX=RobLSol.PosX+(RobLSol.DirX*2);
                            RobLSol.PosY=RobLSol.PosY+(RobLSol.DirY*2);
                            LaberintoSol[RobLSol.PosY][RobLSol.PosX]=9; //Indicamos la nueva posicion a donde se dirige el robot.
                            RobLSol.DirX=(RobLSol.PosX-oPosXSol)/2;
                            RobLSol.DirY=(RobLSol.PosY-oPosYSol)/2;
                         //   estadoLaberinto=CambiaEstado(101);
                            VisualizaMatrizSol();
                         }else{
                            Hay_pared_Sol=BuscaPared(RobLSol.PosX,RobLSol.PosY,RobLSol.DirX,RobLSol.DirY,DERCH);
                            if(Hay_pared_Sol==1){
                                oPosXSol=RobLSol.PosX;
                                oPosXSol=RobLSol.PosX;
                                oPosYSol=RobLSol.PosY;
                                LaberintoSol[oPosYSol][oPosXSol]=3;          //Indicamos que en la posicion actual ya hemos pasado.
                                RobLSol.PosX=RobLSol.PosX+(RobLSol.DirY*2);
                                RobLSol.PosY=RobLSol.PosY-(RobLSol.DirX*2);
                                LaberintoSol[RobLSol.PosY][RobLSol.PosX]=9;  //Indicamos la nueva posicion a donde se dirige el robot.
                                RobLSol.DirX=(RobLSol.PosX-oPosXSol)/2;
                                RobLSol.DirY=(RobLSol.PosY-oPosYSol)/2;
                           //     estadoLaberinto=CambiaEstado(11);
                                VisualizaMatrizSol();
                              }                
                           
                         }
                 }
  
  
  }while((RobLSol.PosX==RobL.PosX)&&(RobLSol.DirY==RobL.DirY));

}

void ini_Laberinto(void){
  RobL.PosX=1;
  RobL.PosY=1;
  RobL.DirX=1;
  RobL.DirY=0;
  
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
  int i,j;
  for(j=0;j<11;j++)
  {
    for(i=0;i<11;i++){
      if(((i==0)||(j==0))||((j==10)||(i==10))){
        LaberintoSol[j][i]=2;       
      }else if(((i%2)==0)&&((j%2)==0)){ 
        LaberintoSol[j][i]=2;        
       }else{
         LaberintoSol[j][i]=0;         
       }
     }    
  }
  LaberintoSol[1][1]=9;
}

