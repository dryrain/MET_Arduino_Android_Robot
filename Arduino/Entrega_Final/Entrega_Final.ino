//Autores: Jordi Calduch (gm22240), Jose Maria Torres (ls31267) y Sergi Diaz (ls29433)
//Fecha: 20-6-15
//
// 
//
//
#include <TimerOne.h> //libreria para controla la interrupción

//----------------------------------------------------------------------------------------------------------------------------------




bool flagSendUDPControl = false;
bool flagSendAccelUDP = false;


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
WifiRx dataRX;              // Declaracion de la estructura para guardar la trama recibida desde el movil.
struct EstadoRobot{
 char dataType ='0';        // tipo de de trama.
 int temp = 10;             // temperatura del robot.
 char luces = 'N';          // Luces delanteras encendidas: Y(yes) o N(no).
 int colision = 0;          // detecta si hay colisión delantera o trasera: 0(no hay), 1(colisión delantera), 2(colisión trasera izq.) y 3(colisión trasera derch.).
 char manualAuto ='M';      // Tipo de modo: M(manual) y A(automático)
 int speedValue =0;         // Velocidad del robot: 0 (parado), 1, 2, 3 (hacia delante) y 4, 5, 6 (hacia atrás).
 int  turnAngle=1;          // Giro del robot: 1 (recto), 2 (der. lento), 3(der. rápido), 4 (izq. lento) y 3(izq. rápido).
 int velocidad_auto=0;
 char FuncionLaberinto='N'; // Estado del proceso Laberinto: 'N' (busca laberinto), 'S' (sigue la solucion optima).
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

 
//----------------------------------------------------------------------------------------------------------------------------------
void setup() {
  cli();  //Deshabilita la interrupción para inicializar los parámetros del robot
     Timer1.initialize(1000000);                // inicializa el Timer cada 1s.
     Timer1.attachInterrupt(interruptCallback); // Crea la interrupción del Timer.
     Serial.begin(9600);                        // Inicializa el puerto serie.
  
     ini_port_ultrasound();                     // Inicializa el puerto de ultrasonidos.
     ini_port_LEDS();                           // Inicializa los puertos de los LEDs de freno y de posición.
     ini_port_infrarojo();                      // Inicializa los puertos de los sensores de infrarojos.
     ini_port_temperatura();                    // Inicializa los puertos del sensor de temperatura.
     ini_port_servo_Right();                    // Inicializa el puerto del motor de la rueda derecha.
     ini_port_servo_Left();                     // Inicializa el puerto del motor de la rueda izquierda.
     ini_port_servo_Head();                     // Inicializa el puerto del motor de giro del sensor de ultrasonidos.
     ini_port_BUMPERS();                        // Inicializa el puerto de bumpers traseros.
     ini_WIFI_WPA("IphonePA","sable1992");      // Inicializa el sistema Wifi
     menuSelect=0;                              //Ini Menu
     char manualoAuto = 'M';                    //inicializa el estado inicial como manual
     Estado.manualAuto=manualoAuto;             //inicializa el estado inicial como manual
  sei(); // Habilita la interrupcion
  ini_port_acelerometro();                      // Inicializa el puerto del sensor de inclinación.
  drawMainMenu();                               // Dibuja el menu en la consola.
}
//----------------------------------------------------------------------------------------------------------------------------------
void loop() {
char* data;
data = readUDP(); // Lee si hay paquete recibido por el puerto Wifi.

freno=Freno_Activado(Estado.speedValue,velocidad_anterior); //Se comprueba si el robot a perdido velocidad y activa la señal de freno.

//mainMenu(); //MENU FOR SERIAL INPUT!

if (data != "E" && !bloqueo_wifi){ // Si el encuentra paquete y no esta bloqueada la wifi (se bloquea para hacer los gestos).
   
  //Parsing the string -------------------------------
  //1) Type ( C - control, L - Laberynt, A - Accel , X - STOP,M -Menu)
  char dataType = data[0];
  dataRX.dataType=dataType;
  Estado.dataType=dataType;
  //2)Speed (1char) 1,2,3,4,5,6
  char speedValue = data[1];
  int speedValueInt = speedValue-'0';
  dataRX.speedValue=speedValueInt;
  velocidad_anterior=Estado.speedValue;
  Estado.speedValue=speedValueInt;
  //3)Turn angle (1chars) 54123
  char turnAngle= data[2];
  Estado.FuncionLaberinto =data[2];
  int turnAngleInt = turnAngle-'0';
  dataRX.turnAngle=turnAngleInt;
  Estado.turnAngle=turnAngleInt;
  //4)Manual/Auto (Manual == 'M' , Auto == A)
  char manualOrAuto = data[3];
  dataRX.manualAuto=manualOrAuto;
  //5)Gesture triggered (Yes == Y , No == N ,C == Circle, S == Square, T == Triangle)
  char gestureTrigger = data[4];
  dataRX.gestureTrigger=gestureTrigger; 
  //5)Lights activate (Yes == Y , No == N)
  char luces = data[5];
  dataRX.luces=luces; 
  //dataRX UPDATED!!!
  //END Parsing ---------------------------------------
    
  switch (dataRX.dataType){ //Se evalua la trama recibida.
    case 'C':                                       // Trama recibida del modo Control.
        if (dataRX.manualAuto=='A'){                // Modo automatico.
           if(Estado.manualAuto=='M'){
              Estado.manualAuto='A';
              flagSendUDPControl=true;              // Enviamos trama indicando el cambio a estado automático
           }
           menuSelect=1;                            // Cambiamos al modo automatico. 
        }else if(dataRX.manualAuto=='M') {          // Modo manual.
          menuSelect=4;                             // Cambiamos al modo manual.
          if(Estado.manualAuto=='A'){               // Primera vez que se activa el modo.
             Estado.manualAuto='M';
             flagSendUDPControl=true;               // Enviamos trama indicando el cambio a estado Manual.
          }          
          if (dataRX.gestureTrigger=='C'){          // Modo gesto circulo.
            Estado.speedValue=3;              
            velocidad_anterior=Estado.speedValue;
            flagSendUDPControl=true;
            menuSelect=5;                           // Cambiamos al modo circulo
          }else if (dataRX.gestureTrigger=='S'){    // Modo gesto cuadrado.
            Estado.speedValue=3;
            velocidad_anterior=Estado.speedValue;
            flagSendUDPControl=true;
            menuSelect=6;                           // Cambiamos al gesto cuadrado.
          }else if (dataRX.gestureTrigger=='T'){    // Modo gesto triangulo.
            Estado.speedValue=3;
            velocidad_anterior=Estado.speedValue;
            flagSendUDPControl=true;
            menuSelect=7;                           // Cambiamos al gesto triangulo.
          }else if (dataRX.luces!=Estado.luces){    // Comprueba si hay cambio de las luces delanteras.
                  Estado.luces=luz_cruce();         // Activa o desactiva la luz delantera.
                  flagSendUDPControl=true;          // Enviamos trama indicando el estado de la luz delantera.
                  Estado.luces=dataRX.luces;        // Actualizamos el estado actual de las luce.          
          }          
       }
    break;  
    case 'L':                                       // Trama recibida del modo Laberinto.
         menuSelect=3;                              // Activa Modo Laberinto
         CambiaEstado(0);                           // Inicializa la maquina de esados del modo laberinto.
    break;
    case 'A':                                       // Trama recibida del modo acelerometro. 
         sendAccelUDP();
         menuSelect=2;
    break; 
    case 'X':                                       // Trama recibida para saber que estas en el menu principal del telefono.
         mover(0,1);
         menuSelect=0;
         resetHEAD();
         dataType='M';
         flagSendUDPControl=false;
         flagSendAccelUDP=false;
   break; 
  }  
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

mainMenu(); //MENU FOR SERIAL INPUT!

//Main Switch for menu interaction
  switch(menuSelect){
     case 0: //Main Menu     
       //mainMenu(); //MENU FOR SERIAL INPUT!
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
                 
                 Reto_acelerometro();
     break;
     case 3: // Modo laberinto
                 Modo_Laberinto();
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
    }       
}

//----------------------------------------------------------------------------------------------------------------------------------
void interruptCallback(){ //Interrupt time
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
  if (freno==true)                         // si está activo el freno encendemos el led rojo
  {
    timerCounterfreno=0;                   // inicializamos el contador de freno para que cuente 5 segundos.
    freno=false;                           // se desactiva el flag de freno.
    Luz_Freno(true);                       // enciende las luces de freno.
    velocidad_anterior=Estado.speedValue;  // Igualamos la velocidad anterior a la actual para que se refresque el frenado cuando no se envía trama desde movil.
  }  
   
  if (timerCounterfreno==5){               // contamos 5 segundos y si el freno esta activado apagamos el led rojo a los cinco segundos.	
    timerCounterfreno=0;                   // se inicializa el contador cada cinco segundos para que no haya overflow de la variable int por no activar del freno.
    Luz_Freno(false);                      // apaga las luces de freno.        
  } 
  
  if(Estado.dataType=='A'){
     flagSendAccelUDP=true;                // Activa el flag de envío de la aceleracion al movil cada segundo si se esta en el reto de la rampa.
  }
  
}


