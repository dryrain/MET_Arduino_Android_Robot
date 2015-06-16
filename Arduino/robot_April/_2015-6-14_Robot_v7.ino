//Autores: Jordi Calduch (gm22240), Jose Maria Torres (ls31267) y Sergi Diaz (ls29433)
//Fecha: 1-6-15
//
// NOTA: Esta deshabilitado el Wifi y el Acelerometro.
//
//
#include <TimerOne.h> //libreria para controla la interrupción

//----------------------------------------------------------------------------------------------------------------------------------

float x,y,z; // Variable para guardar el valor recogido por el sensor de aceleracion.
unsigned int sensorValues[6];

bool flagSendUDPControl = false;
bool flagSendAccelUDP = false;
bool flagLaberinto=false;

//Contadores de segundos en la funcion interrupcion.
int timerCounter=1;      //Contador de segundos, sirve para contabilizar 5 segundos y enviar la temperatura al robot
int timerCounterfreno=0; //Contador de freno, sirve para mantener la luz de freno activa durante 5s.

static int menuSelect=0; //Inicializamos la seleccion del menu en 0

struct WifiRx{
 char dataType;
 int  turnAngle;
 int  speedValue;
 char manualAuto;
 char gestureTrigger;
 char luces;
};
WifiRx dataRX;         // Declaracion de la estructura para guardar la trama recibida desde el movil.
struct EstadoRobot{
 char dataType ='0';   // tipo de de trama
 int temp = 10;        // temperatura del robot
 char luces = 'N';     // Luces delanteras encendidas: Y(yes) o N(no)
 int colision = 0;     // detecta si hay colisión delantera o trasera: 0(no hay), 1(colisión delantera), 2(colisión trasera izq.) y 3(colisión trasera derch.)
 char manualAuto ='M'; // Tipo de modo: M(manual) y A(automático)
 int speedValue =0;    // Velocidad del robot: 0 (parado), 1, 2, 3 (hacia delante) y 4, 5, 6 (hacia atrás)
 int  turnAngle=1;     // Giro del robot: 1 (recto), 2 (der. lento), 3(der. rápido), 4 (izq. lento) y 3(izq. rápido).
 int velocidad_auto=0;
};
EstadoRobot Estado;    // Estructura donde se almacena los datos a enviar al teléfono.
bool cercano=false;    // True: obstaculo cerca, false: no hay obstaculo.
bool girando=false;    // True:indica que está girando. False:indica que ya acabó de girar.

//Variables de memoria velocidad y freno.
int velocidad_anterior=0; // Velocidad del robot: 0 (parado), 1, 2, 3 (hacia delante) y 4, 5, 6 (hacia atrás). //sirve para detectar el frenado en el robot y encender luces de frenado.
bool freno=false;         // True: hay freno activo o se ha bajado la velocidad, False: no se ha bajado la velocidad.

//Bloque wifi para cuadrado, circulo y triangulo.
bool bloqueo_wifi=false;

bool bumper_act=false;   // True: indica que hay colision trasera, False: no hay colisión trasera.
int aux_bumper;// Variable para recoger la colision trasera. 0(no hay), 1(colisión delantera), 2(colisión trasera izq.).
int estadoLaberinto=0;// 1.BuscaCruce 2.BuscaCamino
  int estadoLab=1;

int Laberinto[11][11] ={{2,2,2,2,2,2,2,2,2,2,2},
                        {2,9,0,0,0,0,0,0,0,0,2},
                        {2,0,2,0,2,0,2,0,2,0,2},
                        {2,0,0,0,0,0,0,0,0,0,2},
                        {2,0,2,0,2,0,2,0,2,0,2},
                        {2,0,0,0,0,0,0,0,0,0,2},
                        {2,0,2,0,2,0,2,0,2,0,2},
                        {2,0,0,0,0,0,0,0,0,0,2},
                        {2,0,2,0,2,0,2,0,2,0,2},
                        {2,0,0,0,0,0,0,0,0,0,2},
                        {2,2,2,2,2,2,2,2,2,2,2}};
                        
int LaberintoSol[11][11] ={{2,2,2,2,2,2,2,2,2,2,2},
                           {2,0,0,0,0,0,0,0,0,0,2},
                           {2,0,2,0,2,0,2,0,2,0,2},
                           {2,0,0,0,0,0,0,0,0,0,2},
                           {2,0,2,0,2,0,2,0,2,0,2},
                           {2,0,0,0,0,0,0,0,0,0,2},
                           {2,0,2,0,2,0,2,0,2,0,2},
                           {2,0,0,0,0,0,0,0,0,0,2},
                           {2,0,2,0,2,0,2,0,2,0,2},
                           {2,0,0,0,0,0,0,0,0,0,2},
                           {2,2,2,2,2,2,2,2,2,2,2}};
                       

struct Robot_L{
 int  DirX=1;
 int  DirY=0;
 int  PosX=1;
 int  PosY=1;
};

Robot_L RobL;         // Declaracion de la estructura para guardar la informacion del robot en el laberinto
int  oPosX=1;    // Posicion anterior.
int  oPosY=1;    // Posicion anterior.
int Hay_pared;
const int DERCH =  1;
const int IZQ =    0;
const int RECT =   2;

int Salida_encontrada; // Variable donde se almacena si ha encontrado la salida: 2=Salida encontra, 1=Cruce encontrado y 0=buscando salida o cruce.
 int I,D,C;
 int detectaPared;
 int ModoLaberinto=1;  // 1:busca salida por primera vez, 2:busca salida con el camino mas corto
 
//----------------------------------------------------------------------------------------------------------------------------------
void setup() {
  cli();  //Deshabilita la interrupción para inicializar los parámetros del robot
     Timer1.initialize(1000000);                // inicializa el Timer cada 1s.
     Timer1.attachInterrupt(interruptCallback); // Crea la interrupción del Timer.
     Serial.begin(9600);                        // Inicializa el puerto serie.
  
     ini_port_ultrasound(); // Inicializa el puerto de ultrasonidos.
     ini_port_LEDS();       // Inicializa los puertos de los LEDs de freno y de posición.
     ini_port_infrarojo();  // Inicializa los puertos de los sensores de infrarojos.
     ini_port_temperatura();// Inicializa los puertos del sensor de temperatura.
     ini_port_servo_Right();// Inicializa el puerto del motor de la rueda derecha.
     ini_port_servo_Left(); // Inicializa el puerto del motor de la rueda izquierda.
     ini_port_servo_Head(); // Inicializa el puerto del motor de giro del sensor de ultrasonidos.
     ini_port_BUMPERS();    // Inicializa el puerto de bumpers traseros.
   
  //ini_WIFI_WPA("hola","adiosadios"); //conecta wifi
 //  ini_WIFI_WPA("vodafoneED7E","SCCDDUPITP6BBM"); //conecta wifi
  //ini_WIFI_WPA("Dryrain","sibemolsibemo");
  ini_WIFI_WPA("IphonePA","sable1992");
  
  
     //Ini Menu
     menuSelect=0;     
     char manualoAuto = 'M';        //inicializa el estado inicial como manual
     Estado.manualAuto=manualoAuto; //inicializa el estado inicial como manual
    
  sei(); // Habilita la interrupcion
//  ini_port_acelerometro(); // Inicializa el puerto del sensor de inclinación.
    
  drawMainMenu(); // Dibuja el menu en la consola.
 // Serial.println("coche");
  ini_Laberinto();
  
  Serial.println(Laberinto[1][1]);
  Serial.println(Laberinto[0][1]);
  VisualizaMatriz();
  
 
}
//----------------------------------------------------------------------------------------------------------------------------------
void loop() {
char* data;
data = readUDP();

freno=Freno_Activado(Estado.speedValue,velocidad_anterior); //Se comprueba si el robot a perdido velocidad y activa la señal de freno.

if (data != "E" && !bloqueo_wifi){ //We found something
  
  Serial.print("Data: ");
  Serial.println(data);
  
  
  //Parsing the string -------------------------------
  //1) Type ( C - control, L - Laberynt, A - Accel , X - STOP,M -Menu)
  char dataType = data[0];
  Serial.print("Type: ");
  Serial.println(dataType);
  dataRX.dataType=dataType;
  Estado.dataType=dataType;
  //2)Speed (1char) 1,2,3,4,5,6
  char speedValue = data[1];
  int speedValueInt = speedValue-'0';
  Serial.print("SpeedValue: ");
  Serial.println(speedValueInt);
  dataRX.speedValue=speedValueInt;
  velocidad_anterior=Estado.speedValue;
  Estado.speedValue=speedValueInt;
  //3)Turn angle (1chars) 54123
  char turnAngle= data[2];
  int turnAngleInt = turnAngle-'0';
  Serial.print("Turn: ");
  Serial.println(turnAngleInt);
  dataRX.turnAngle=turnAngleInt;
  Estado.turnAngle=turnAngleInt;
  //4)Manual/Auto (Manual == 'M' , Auto == A)
  char manualOrAuto = data[3];
  Serial.print("Manual/Auto: ");
  Serial.println(manualOrAuto);
  dataRX.manualAuto=manualOrAuto;
 
  
  //5)Gesture triggered (Yes == Y , No == N ,C == Circle, S == Square, T == Triangle)
  char gestureTrigger = data[4];
  Serial.print("Gesture Triggered: ");
  Serial.println(gestureTrigger); 
  dataRX.gestureTrigger=gestureTrigger; 
  
  //5)Lights activate (Yes == Y , No == N)
  char luces = data[5];
  Serial.print("Luces: ");
  Serial.println(luces); 
  dataRX.luces=luces; 
   
  
  //dataRX UPDATED!!!
  
  //END Parsing ---------------------------------------
  
  flagSendUDPControl=true;
  //Do whatever is needed to ...
  switch (dataRX.dataType){ //Reply control data
    case 'C':
        //Will be handled during interrupt
        if (dataRX.manualAuto=='A'){ //The robot is moving on his own
           if(Estado.manualAuto=='M'){// primera vez que se activa el modo
              Estado.manualAuto='A';
              flagSendUDPControl=true; // enviamos trama indicando el cambio a estado automático
           }
           menuSelect=1; //AutoMovement function  
        }else if(dataRX.manualAuto=='M') { //Detect Manual inputs
          menuSelect=4; // activamos el movimiento
          if(Estado.manualAuto=='A'){// primera vez que se activa el modo
             Estado.manualAuto='M';
             flagSendUDPControl=true; // enviamos trama indicando el cambio a estado Manual
          }
          
          //Gesture detected
          if (dataRX.gestureTrigger=='C'){
            Estado.speedValue=3;
            velocidad_anterior=Estado.speedValue;
            flagSendUDPControl=true;
            menuSelect=5;
          }else if (dataRX.gestureTrigger=='S'){
            Estado.speedValue=3;
            velocidad_anterior=Estado.speedValue;
            flagSendUDPControl=true;
            menuSelect=6;
          }else if (dataRX.gestureTrigger=='T'){
            Estado.speedValue=3;
            velocidad_anterior=Estado.speedValue;
            flagSendUDPControl=true;
            menuSelect=7;
          }else if (dataRX.luces!=Estado.luces){ //comprueba si hay cambio de las luces delanteras.
                  Estado.luces=luz_cruce();  //Activa o desactiva la luz delantera.
                  flagSendUDPControl=true;   //enviamos trama indicando el estado de la luz delantera.
                  Estado.luces=dataRX.luces; //Actualizamos el estado actual de las luce.          
          }          
       }
   break;
  
    case 'L': // Reply with laberynth data
         sendLaberynthUDP();
         //menuSelect=3;
   break;
    case 'A': //Reply with accel data
    
         sendAccelUDP();
         menuSelect=2;
   break; 
   case 'X': //Reply with accel data
    mover(0,1);
    menuSelect=0;
    dataType='M';
   break; 
   case 'M': //Reply with accel data
    menuSelect=0;
    dataType='M';
   break; 
  }
  //-------------------------- 
}

if (Estado.dataType=='C'){      // Si el robot esta en funcion de control  
//
// Revisa los sensores de colision.
//
  if(Lee_ultrasonidos()<20){    // Si detecta un objeto a menos de 20cm se activa la señal de colision delantera.
     if(cercano){               // Si el objeto es cercano, primera vez que ve la colision.
       flagSendUDPControl=true; // enviamos trama indicando la posible colisión delantera 
       Estado.colision=1;       // se activa el estado de colision en las variables de estado generales del robot. 
       cercano=false;           // desactivamos el valor de cercano para que no vuelva a entrar en esta condicion y no envie infinitos mensajes al robot. 
     }  
       girando=true;            // se activa la señal de giro para que el robot sepa que esta girando para esquivar el obstaculo
  }else{             
     if(!cercano){              // si el objeto no es cercano, quiere decir que esta a menos de 20cm.
       flagSendUDPControl=true; // enviamos trama indicando la posible colisión delantera   
       Estado.colision=0;       // se activa el estado de colision en las variables de estado generales del robot. 
       cercano=true;            // activamos el valor de cercano para que no vuelva a entrar en esta condicion y no envie infinitos mensajes al robot. 
     }            
  }
  aux_bumper=Lee_bumpers();        // Lee si hay colision trasera.
  if(aux_bumper!=0){               // evalua si hay colision. 
    if(bumper_act==false){         // comprueba si es el primer golpe y envía la trama de colision al telf. (si se mantiene pulsado el bumper no enviara infinitas tramas.
      flagSendUDPControl=true;     // enviamos trama indicando la colisión delantera. 
      bumper_act=true;             // Activamos el estado de colision.
      Estado.colision=aux_bumper;  // Actualizamos el estado de colision en la estructura de Estado general del robot.
    }      
  }else{                           // evalua si no hay colision.
    if(bumper_act==true){          // comprueba si es la primera vez que se desactiva el bumper y envía la trama de no colision al telf. (si no se mantiene pulsado el bumper no enviara infinitas tramas.
      bumper_act=false;            // desactivamos el estado de colision.
      flagSendUDPControl=true;     // enviamos trama indicando la no colision trasera.
      Estado.colision=aux_bumper;  // Actualizamos el estado de colision en la estructura de Estado general del robot.   
    }    
  }  
//Modo automático.
  if(Estado.manualAuto=='A'){
    Estado.speedValue=3;
  }
  
}

if(flagSendUDPControl){ // Si esta activado el flag envia la trama de control al movil.
  sendControlUDP(Estado.temp,Estado.luces,Estado.colision,Estado.manualAuto,Estado.speedValue);
  flagSendUDPControl=false;
}
if(flagSendAccelUDP){ // Si esta activado el flag envia la trama del retro de aceleracion al movil.
  sendAccelUDP();
  flagSendAccelUDP=false;
}

if(flagLaberinto){ // Si esta activado el flag envia la trama del retro de aceleracion al movil.
  sendLaberynthUDP();
  flagLaberinto=false;
}


mainMenu(); //MENU FOR SERIAL INPUT!

//Main Switch for menu interaction
  switch(menuSelect){
     case 0: //Main Menu     
       mainMenu(); //MENU FOR SERIAL INPUT!
      break;
     case 1: // modo automático
		if(girando){
                  if(gira(1,3,90)){
                    girando=false;
                  }
                }else{
                  mover(3,1);
                }   
      break;
     case 2: // modo acelerometro     
                 Lee_aceleracion();
                 Escribe_aceleracion();          
                 if(x>0){
                   if(x<0.08&&y>-0.08&&y<0.08){
                     mover(0,1);
                   }else{
                       if(y>0.05){
                         
                           mover(3,9);
                        }
                        if(y<0.05){                           
                            mover(3,8);
                        }
                    }
                 }else{
                  if(y>0.05){
                       mover(3,9);
                    }
                    if(y<0.05){                                           
                        mover(3,8);
                   }
                 }
      break;
     case 3: // modo laberinto
               Lee_Infrarojos();
		//Escribe_deteccion_negro(); 
              
         if(flagLaberinto==false){
               if(estadoLaberinto==0){  // INICIO POR PRIMERA VEZ EL LABERINTO.
                     Salida_encontrada=buscaSalida(sensorValues);
                     if(Salida_encontrada==1){
                      // flagLaberinto=true; 
                       estadoLaberinto=CambiaEstado(3);                       
                     }else if(Salida_encontrada==2){        // Ha encontrado la salida.
                      Laberinto[RobL.PosY][RobL.PosX]=4;    //indicamos la posición como la salida.
                      LaberintoSol[RobL.PosY][RobL.PosX]=4; //indicamos la posición como la salida.
                      adelante(0);                          // se frena el robot al encontrar la salida.
                      Serial.println("Salidaaaaaaaaaaaaaaaaaa Encontrada"); 
                      VisualizaMatriz();
                      VisualizaMatrizSol();
                      estadoLaberinto=CambiaEstado(200); // estado siguiente para saber si se ha de seguir el camino
                    
                     }                                            
               }
               if(estadoLaberinto==1){  // Busca la intersección y se para.
                     if( buscarCruce(sensorValues)==true){
                        estadoLaberinto=CambiaEstado(0);
                    //  flagLaberinto=true; 
                     }  
               } 
               if(estadoLaberinto==2){  // Sin uso
                     //compruebaSalida();
                  //   estadoLaberinto=CambiaEstado(3);
               }
               if(estadoLaberinto==3){  // Comprueba si hay pared derecha.
                     Serial.println(BuscaPared(RobL.PosX,RobL.PosY,RobL.DirX,RobL.DirY,DERCH));
                     Hay_pared=BuscaPared(RobL.PosX,RobL.PosY,RobL.DirX,RobL.DirY,DERCH);
                     Serial.println("111111111111111111111111111111111111_comprueba si hay parede derecha");
                     if(Hay_pared==0){
                       // detectar pared  
                      //Mover cabeza
                    //  moverServo(2,0);
               //       if(wait1s()){
                          if(detectaDerecha()){
                                 Serial.println("aaaaaaaaaaaaaaaaaaaaa_el senson detecta pared derecha");
                                 Laberinto[RobL.PosY-RobL.DirX][RobL.PosX+RobL.DirY]=2;
                                 estadoLaberinto=CambiaEstado(4);
                                 VisualizaMatriz();
                          }else{
                                Laberinto[RobL.PosY-RobL.DirX][RobL.PosX+RobL.DirY]=1;   // Indicamos en la matriz que no hay pared.                         
                                Serial.println("aaaaaaaaaaaaaaaaaaaaa_el senson no detecta pared derecha");
                            
                          }
            //          }
                            
                                              
                      
    
                        //   estadoLaberinto=6;
                         // flagLaberinto=true; 
                         
                     }else if(Hay_pared==1){
                        Serial.println("No hay pared derecha"); 
                        Laberinto[RobL.PosY-RobL.DirX][RobL.PosX+RobL.DirY]=1;   // Indicamos en la matriz que no hay pared.                         
                     }else{ // como hay pared va al estado para comprobar la pared frontal.
                       estadoLaberinto=CambiaEstado(4);
                       Serial.println("hay pared derecha"); 
                     }
                     if(Laberinto[RobL.PosY-RobL.DirX][RobL.PosX+RobL.DirY]==1){
                            if(Hay_pared==1){ //comprueba que no pasemos por un camino ya utilizado si es asi recalcula la solucion optima, si no indica que la casilla es solucion
                              ActualizaSolLab();
                            }else{
                              LaberintoSol[RobL.PosX][RobL.PosY]=3; 
                              Laberinto[oPosY-RobL.DirX][oPosX+RobL.DirY]==3;
                            }
                            oPosX=RobL.PosX;
                            oPosY=RobL.PosY;
                            Laberinto[oPosY][oPosX]=1;  //Indicamos que en la posicion actual ya hemos pasado.
                            RobL.PosX=RobL.PosX+(RobL.DirY*2);
                            RobL.PosY=RobL.PosY-(RobL.DirX*2);
                            Laberinto[RobL.PosY][RobL.PosX]=9; //Indicamos la nueva posicion a donde se dirige el robot.
                            RobL.DirX=(RobL.PosX-oPosX)/2;
                            RobL.DirY=(RobL.PosY-oPosY)/2;
                            estadoLaberinto=CambiaEstado(11);
                            if(Hay_pared==1){ //comprueba que no pasemos por un camino ya utilizado si es asi recalcula la solucion optima, si no indica que la casilla es solucion
                              ActualizaSolLab();
                            }else{
                              LaberintoSol[oPosY][oPosX]=3; 
                              Laberinto[oPosY-RobL.DirX][oPosX+RobL.DirY]==1;
                            }
                            VisualizaMatriz();
                      }
                    // flagLaberinto=true;
               }
               if(estadoLaberinto==4){ // Comprueba si hay pared en frente
                     Serial.println(BuscaPared(RobL.PosX,RobL.PosY,RobL.DirX,RobL.DirY,RECT));
                     Hay_pared=BuscaPared(RobL.PosX,RobL.PosY,RobL.DirX,RobL.DirY,RECT);
                     Serial.println("111111111111111111111111111111111111_RECT_comprobar si hay pared en frente");
                     if(Hay_pared==0){
                       // detectar pared
                      
                  //    moverServo(2,90);
              //        if(wait1s()){
                        if(detectaCentro()){
                               Serial.println("aaaaaaaaaaaaaaaaaaaaa_RECT_el detector comprueba que hay pared en fretne");
                               Laberinto[RobL.PosY+RobL.DirY][RobL.PosX+RobL.DirX]=2;
                               estadoLaberinto=CambiaEstado(5);
                        }else{
                              Laberinto[RobL.PosY+RobL.DirY][RobL.PosX+RobL.DirX]=1;   // Indicamos en la matriz que no hay pared.                         
                              Serial.println("aaaaaaaaaaaaaaaaaaaaa_RECT_el detector comprueba que no hay pared en fretne");
                       
                        }
                //      }
                        
                     }else if(Hay_pared==1){
                        Serial.println("No hay pared frontal"); 
                        Laberinto[RobL.PosY+RobL.DirY][RobL.PosX-RobL.DirX]=1;   // Indicamos en la matriz que no hay pared.                         
                     }else{ // como hay pared va al estado para comprobar la pared frontal.
                       estadoLaberinto=CambiaEstado(5);
                       Serial.println("hay pared frontal"); 
                     }
                     if(Laberinto[RobL.PosY+RobL.DirY][RobL.PosX+RobL.DirX]==1){
                            oPosX=RobL.PosX;
                            oPosY=RobL.PosY;
                            Laberinto[oPosY][oPosX]=1;  //Indicamos que en la posicion actual ya hemos pasado.
                            RobL.PosX=RobL.PosX+(RobL.DirX*2);
                            RobL.PosY=RobL.PosY+(RobL.DirY*2);
                            Laberinto[RobL.PosY][RobL.PosX]=9; //Indicamos la nueva posicion a donde se dirige el robot.
                            RobL.DirX=(RobL.PosX-oPosX)/2;
                            RobL.DirY=(RobL.PosY-oPosY)/2;
                            estadoLaberinto=CambiaEstado(1);
                            if(Hay_pared==1){ //comprueba que no pasemos por un camino ya utilizado si es asi recalcula la solucion optima, si no indica que la casilla es solucion
                              ActualizaSolLab();
                            }else{
                              LaberintoSol[oPosY][oPosX]=3; 
                            }
                            VisualizaMatriz();
                      }
                    // flagLaberinto=true;
                  
              }
             if(estadoLaberinto==5){ // Comprueba si hay pared en izquierda
                     Serial.println(BuscaPared(RobL.PosX,RobL.PosY,RobL.DirX,RobL.DirY,IZQ));
                     Hay_pared=BuscaPared(RobL.PosX,RobL.PosY,RobL.DirX,RobL.DirY,IZQ);
                     Serial.println("111111111111111111111111111111111111_IZQ_comprobar si hay pared en frente");
                     if(Hay_pared==0){
                       // detectar pared
                      
                //      moverServo(2,180);
               //       if(wait1s()){
                        if(detectaIzquierda()){
                               Serial.println("aaaaaaaaaaaaaaaaaaaaa_IZQ_el detector comprueba que hay pared en fretne");
                               Laberinto[RobL.PosY+RobL.DirX][RobL.PosX-RobL.DirY]=2;
                               
                               //Giro 180º y estado 1.
                        }else{
                              Laberinto[RobL.PosY+RobL.DirX][RobL.PosX-RobL.DirY]=1;   // Indicamos en la matriz que no hay pared.                         
                              Serial.println("aaaaaaaaaaaaaaaaaaaaa_RECT_el detector comprueba que no hay pared en fretne");
                               
                        }
               //       }
                        
                     }else if(Hay_pared==1){
                        Serial.println("No hay pared frontal"); 
                        Laberinto[RobL.PosY+RobL.DirX][RobL.PosX-RobL.DirY]=1;   // Indicamos en la matriz que no hay pared.                         
                     }else{ // como hay pared va al estado para comprobar la pared frontal.
                       estadoLaberinto=CambiaEstado(13);
                      
                 //      Giro 180º y estado 1
                       Serial.println("hay pared frontal"); 
                     }
                     if(Laberinto[RobL.PosY+RobL.DirX][RobL.PosX-RobL.DirY]==1){
                            oPosX=RobL.PosX;
                            oPosY=RobL.PosY;
                            Laberinto[oPosY][oPosX]=1;  //Indicamos que en la posicion actual ya hemos pasado.
                            RobL.PosX=RobL.PosX-(RobL.DirY*2);
                            RobL.PosY=RobL.PosY+(RobL.DirX*2);
                            Laberinto[RobL.PosY][RobL.PosX]=9; //Indicamos la nueva posicion a donde se dirige el robot.
                            RobL.DirX=(RobL.PosX-oPosX)/2;
                            RobL.DirY=(RobL.PosY-oPosY)/2;
                            estadoLaberinto=CambiaEstado(12);
                            if(Hay_pared==1){ //comprueba que no pasemos por un camino ya utilizado si es asi recalcula la solucion optima, si no indica que la casilla es solucion
                              ActualizaSolLab();
                            }else{
                              LaberintoSol[oPosY][oPosX]=3; 
                            }
                            VisualizaMatriz();
                      }else if(Laberinto[RobL.PosY+RobL.DirX][RobL.PosX-RobL.DirY]==2){
                            oPosX=RobL.PosX;
                            oPosY=RobL.PosY;
                            Laberinto[oPosY][oPosX]=1;  //Indicamos que en la posicion actual ya hemos pasado.
                            RobL.DirX=RobL.DirX*(-1);
                            RobL.DirY=RobL.DirY*(-1);
                            RobL.PosX=RobL.PosX+(RobL.DirX*2);
                            RobL.PosY=RobL.PosY+(RobL.DirY*2);
                            Laberinto[RobL.PosY][RobL.PosX]=9; //Indicamos la nueva posicion a donde se dirige el robot.
                            
                            estadoLaberinto=CambiaEstado(13);
                            VisualizaMatriz();
                        Serial.println("girar 180º");
                      }
                //    // flagLaberinto=true;
                  
              } 
               
               
               if(estadoLaberinto==11){ // Gira a la derercha y sigue enfrente.
                        if(gira(DERCH,3,90)){
                          if(ModoLaberinto==1){ // si está buscando la salida por primera vez.
                             estadoLaberinto=1;
                          }else{                // si está siguiendo el camino mas corto.
                             estadoLaberinto=101;
                          }
                        }             
               } 
               if(estadoLaberinto==12){ // Gira a la izquierda y sigue enfrente.
                        if(gira(IZQ,3,90)){
                          if(ModoLaberinto==1){ // si está buscando la salida por primera vez.
                             estadoLaberinto=1;
                          }else{                // si está siguiendo el camino mas corto.
                             estadoLaberinto=101;
                          }
                        }             
               } 
               if(estadoLaberinto==13){ // Gira a la derercha y sigue enfrente.
                        if(gira(IZQ,3,180)){
                          estadoLaberinto=1;
                        }             
               } 
               
               
               if(estadoLaberinto==44){ //Miramos el servo
                    if (wait1s()){
                       if( detectaIzquierda()) Serial.println("I"); I=1;
                       estadoLaberinto=5;
                       // flagLaberinto=true; 
                    }
              }
             if(estadoLaberinto==4){ //Miramos el servo
                    if (wait1s()){
                        if( detectaCentro()) Serial.println("D"); C=1;
                         estadoLaberinto=1;
                          flagLaberinto=true; 
                    }
              }
              if(estadoLaberinto==55){ //Miramos el servo
                    if (wait1s()){
                        if( detectaDerecha()) Serial.println("D"); D=1;
                        
                         estadoLaberinto=6;
                         // flagLaberinto=true; 
                     }
              }
              
              
              if(estadoLaberinto==100){  // INICIO POR PRIMERA VEZ EL LABERINTO.
                     Salida_encontrada=buscaSalida(sensorValues);
                     if(Salida_encontrada==1){
                      // flagLaberinto=true; 
                       estadoLaberinto=CambiaEstado(102);                       
                     }else if(Salida_encontrada==2){
                      Serial.println("Salidaaaaaaaaaaaaaaaaaa Encontrada"); 
                      adelante(0);
                     }                   
               }
               if(estadoLaberinto==101){  // Busca la intersección y se para.
                     if( buscarCruce(sensorValues)==true){
                        estadoLaberinto=CambiaEstado(100);                  
                     }  
               } 
               if(estadoLaberinto==102){  // Comprueba si hay camino rapido en dirección izquierda
                     Hay_pared=BuscaPared(RobL.PosX,RobL.PosY,RobL.DirX,RobL.DirY,IZQ);
                     if(Hay_pared==3){
                       oPosX=RobL.PosX;
                       oPosY=RobL.PosY;
                       Laberinto[oPosY][oPosX]=3;          //Indicamos que en la posicion actual ya hemos pasado.
                       RobL.PosX=RobL.PosX-(RobL.DirY*2);
                       RobL.PosY=RobL.PosY+(RobL.DirX*2);
                       Laberinto[RobL.PosY][RobL.PosX]=9;  //Indicamos la nueva posicion a donde se dirige el robot.
                       RobL.DirX=(RobL.PosX-oPosX)/2;
                       RobL.DirY=(RobL.PosY-oPosY)/2;
                       estadoLaberinto=CambiaEstado(12);
                       VisualizaMatriz();
                     }else{
                       estadoLaberinto=CambiaEstado(103);
                     }
               }
               if(estadoLaberinto==103){  // Comprueba si hay camino rapido en dirección frontal
                     Hay_pared=BuscaPared(RobL.PosX,RobL.PosY,RobL.DirX,RobL.DirY,RECT);
                     if(Hay_pared==3){
                            oPosX=RobL.PosX;
                            oPosX=RobL.PosX;
                            oPosY=RobL.PosY;
                            Laberinto[oPosY][oPosX]=3;         //Indicamos que en la posicion actual ya hemos pasado.
                            RobL.PosX=RobL.PosX+(RobL.DirX*2);
                            RobL.PosY=RobL.PosY+(RobL.DirY*2);
                            Laberinto[RobL.PosY][RobL.PosX]=9; //Indicamos la nueva posicion a donde se dirige el robot.
                            RobL.DirX=(RobL.PosX-oPosX)/2;
                            RobL.DirY=(RobL.PosY-oPosY)/2;
                            estadoLaberinto=CambiaEstado(101);
                            VisualizaMatriz();
                     }else{
                       estadoLaberinto=CambiaEstado(104);
                     }
               }
               if(estadoLaberinto==104){  // Comprueba si hay camino rapido en direccion derecha.
                     Hay_pared=BuscaPared(RobL.PosX,RobL.PosY,RobL.DirX,RobL.DirY,DERCH);
                     if(Hay_pared==3){
                            oPosX=RobL.PosX;
                            oPosX=RobL.PosX;
                            oPosY=RobL.PosY;
                            Laberinto[oPosY][oPosX]=3;          //Indicamos que en la posicion actual ya hemos pasado.
                            RobL.PosX=RobL.PosX+(RobL.DirY*2);
                            RobL.PosY=RobL.PosY-(RobL.DirX*2);
                            Laberinto[RobL.PosY][RobL.PosX]=9;  //Indicamos la nueva posicion a donde se dirige el robot.
                            RobL.DirX=(RobL.PosX-oPosX)/2;
                            RobL.DirY=(RobL.PosY-oPosY)/2;
                            estadoLaberinto=CambiaEstado(11);
                            VisualizaMatriz();
                     }else{
                      // estadoLaberinto=CambiaEstado(104); // es imposible esta condicion porque siempre sera 3.
                     }
               }
    
    
    
               
         }
            
              
              

      break;
     case 4: // Modo manual           
                mover(Estado.speedValue,Estado.turnAngle); 
      break;
     case 5: // Mover en forma de  circulo            
                if(circunferencia()){ 
                   menuSelect=0;   // si ha acabado la acción vuelve al menú principal       
                   bloqueo_wifi=false;
                   Estado.speedValue=0;                   
                   flagSendUDPControl=true;
                }else{
                  bloqueo_wifi=true;
                }   
      break;
     case 6: // Mover en forma de  cuadrado             
                if(cuadrado(3,20)){
                  menuSelect=0;   // si ha acabado la acción vuelve al menú principal
                  Estado.speedValue=0;
                  flagSendUDPControl=true;
                  bloqueo_wifi=false;
                }else{
                  bloqueo_wifi=true;
                }
      break;
     case 7: // Mover en forma de triángulo             
                if(triangulo(3,20)){ 
                  menuSelect=0;  // si ha acabado la acción vuelve al menú principal        
                  Estado.speedValue=0;
                  flagSendUDPControl=true;
                  bloqueo_wifi=false;
                }else{
                  bloqueo_wifi=true;
                }
      break; 
      case 8:       
                Escribe_deteccion_negro();
      break; 
    }       
}

//----------------------------------------------------------------------------------------------------------------------------------
void interruptCallback(){ //Interrupt time
  //1s passed
  timerCounter++; 
  timerCounterfreno++;
  if (timerCounter==5){ // 5s passed	
    timerCounter=0;   
    //Send info if we are at Control
    if (Estado.dataType=='C'){
      Estado.temp = (int)Lee_temperatura();
      flagSendUDPControl=true;
    }
  }
  if (freno==true) // si está activo el freno encendemos el led rojo
  {
    timerCounterfreno=0;                   // inicializamos el contador de freno para que cuente 5 segundos.
    freno=false;                           // se desactiva el flag de freno.
    Luz_Freno(true);                       // enciende las luces de freno.
    velocidad_anterior=Estado.speedValue;  // Igualamos la velocidad anterior a la actual para que se refresque el frenado cuando no se envía trama desde movil.
  }  
   
  if (timerCounterfreno==5){ // contamos 5 segundos y si el freno esta activado apagamos el led rojo a los cinco segundos.	
    timerCounterfreno=0;                   // se inicializa el contador cada cinco segundos para que no haya overflow de la variable int por no activar del freno.
    Luz_Freno(false);                      // apaga las luces de freno.        
  } 
  
  if(Estado.dataType=='A'){
     flagSendAccelUDP=true; // Activa el flag de envío de la aceleracion al movil cada segundo si se esta en el reto de la rampa.
  }
  
  if(Estado.dataType=='L'){
    flagLaberinto=true;
  }
}


