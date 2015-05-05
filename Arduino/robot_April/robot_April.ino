#include <Servo.h> //librería de los servomotores
#include <TimerOne.h> //libreria de la interrupción
#include <Wire.h> // Must include Wire library for I2C
#include <SFE_MMA8452Q.h> //libreria del acelerometro
#include <SPI.h> //libreria para interconectar la placa Mega y la shield WIFI
#include <WiFi.h> //librería para controla la transmisión WIFI
#include <WiFiUdp.h> //libreria para controlar el tráfico UDP

#define SERIAL_ACTIVE
#define WIFI_ACTIVE
#define TESTING_AREA

//#define ENTREGA_4
#define ENTREGA_5


//const IPAddress IPSend (170,20,10,13);
const char IPSend[] = "172.20.10.13";
//const int sendPort = 55056;
const int sendPort = 4560;
const char* WIFIName = "IphonePA";
const char* WIFIPass ="sable1992";

//byte IP[4]={170,20,10,12};
IPAddress IPRx;
int PortRx;

bool flagSendUDPControl = false;

//Counter for Interrupts
int timerCounter=1;
int timerCounterfreno=0;
static int menuSelect=0;

long distancia_obj; // variable para almacenar la distancia al objeto obtenida del ultrasonido.

//Flags for interrupt time
boolean getTemps = false;
boolean getAccel = false;
boolean getUltrasound = false;
boolean getInfrared = false;

//Variables WIFI
//char *recibido; //puntero donde guardamos el buffer recibido en puerto WIFI.
// Formato de la trama recibida por el puerto
struct  Tramas{
  byte numero; //número de trama.
  byte cabecera; //identifica la acción a realizar (1-avanzar, 2-retroceder...)
  byte datos[8]; //guarda los datos de la acción (ejemplo velociad, dirección...)
  bool activa;   //indica si la trama está en el buffer (1- está en el buffer, 0- ya ha sido leida)
};

struct WifiRx{
 char dataType;
 int  turnAngle;
 int  speedValue;
 char manualAuto;
 char gestureTrigger;
};
WifiRx dataRX; //Declare a struct used to hold received data

//Variables de memoria de direccion, distancia, velocidad y angulo
int aux_distancia=20; //distancia en cm
int aux_velocidad=3;  //0-stop, 2-media y 3-alta
int aux_angulo=45;    //angulo en grados
int aux_direccion=0;  // 0- Izquierda y 1-derecha
float aux_temperatura; // temeperatura recogida del sensor
String auxiliar; // variable para convertir la temperatura en char*
char envio[20]; // variable para convertir la temperatura en char*

//Variable de freno.
bool freno=false;
bool freno_timer=false;
//Bloque wifi para cuadrado, circulo y triangulo.
bool bloqueo_wifi=false;

void setup() {
  cli();  
   Timer1.initialize(1000000);
   Timer1.attachInterrupt(interruptCallback);
   Serial.begin(9600);
  
    ini_port_ultrasound();
    ini_port_LEDS();
    ini_port_infrarojo();
    ini_port_temperatura();
    ini_port_servo_Right();
    ini_port_servo_Left();
    ini_port_servo_Head();
    
    
   //ini_WIFI_WPA("vodafoneED7E","SCCDDUPITP6BBM"); //conectamos wifi
   //ini_WIFI_WPA("Dryrain","sibemolsibemo");
   ini_WIFI_WPA("IphonePA","sable1992");
    //Ini Menu
    menuSelect=0;     
    
  sei();
  //ini_port_acelerometro(); 
    
  drawMainMenu();
  
}

void loop() {


  
#ifdef ENTREGA_5 //Wifi protocol Arduino & Android
char* data;
data = readUDP();
if (data != "E"){ //We found something
  
  Serial.print("Data: ");
  Serial.println(data);
  
  //Serial.print("Lenght: ");
  //Serial.println(sizeof data); // DOES NOT WORK
  
//  Serial.println("Printing all: ");
//  Serial.println(data[0]);
//  Serial.println(data[1]);
//  Serial.println(data[2]);
//  Serial.println(data[3]);
//  Serial.println(data[4]);
//  Serial.println(data[5]);
//  Serial.println(data[6]);
  
  
  //Parsing the string -------------------------------
  //1) Type ( C - control, L - Laberynt, A - Accel , X - STOP)
  char dataType = data[0];
  Serial.print("Type: ");
  Serial.println(dataType);
  dataRX.dataType=dataType;
  
  //2)Turn angle (3chars)
  char turnAngle[3] = {data[1],data[2],data[3]};
  int turnAngleInt = atoi(turnAngle);
  Serial.print("Turn: ");
  Serial.println(turnAngleInt);
  dataRX.turnAngle=turnAngleInt;
  
  //3)Speed (1char) 1,2,3
  char speedValue = data[4];
  int speedValueInt = speedValue-'0';
  Serial.print("SpeedValue: ");
  Serial.println(speedValueInt);
  dataRX.speedValue=speedValueInt;
  
  //4)Manual/Auto (Manual == 'M' , Auto == A)
  char manualOrAuto = data[5];
  Serial.print("Manual/Auto: ");
  Serial.println(manualOrAuto);
  dataRX.manualAuto=manualOrAuto;
  
  //5)Gesture triggered (Yes == Y , No == N)
  char gestureTrigger = data[6];
  Serial.print("Gesture Triggered: ");
  Serial.println(gestureTrigger); 
  dataRX.gestureTrigger=gestureTrigger; 
  
  //dataRX UPDATED!!!
  
  //END Parsing ---------------------------------------
  
  
  //Do whatever is needed to ...
  switch (dataRX.dataType){ //Reply control data
    case 'C':
    //Will be handled during interrupt
    
   break;
  
    case 'L': // Reply with laberynth data
    
     sendLaberynthUDP();
   
   break;
  
    case 'A': //Reply with accel data
    
     sendAccelUDP();
   break; 
  }
  
  
  //-------------------------- 
}

if(flagSendUDPControl){
  sendControlUDP();
  flagSendUDPControl=false;
}


#endif //ENTREGA_5


 // char* prova = LeeTramaChar();
  //if (prova!=""){
     //Serial.print("TramaChar:");
    // Serial.println(prova);
 // }
    
    
#ifdef ENTREGA_4
  struct Tramas Paquete=LeeTrama(); //Leemos el puerto por si hay paquete recibido de la WIFI.    
      
  if (Paquete.activa && !bloqueo_wifi){   
      Serial.print("Cabecera:");
      Serial.println(Paquete.cabecera);
      Serial.print("Datos:");
      Serial.println(Paquete.datos[1]);
    
//Menú de la accion recivida por el wifi.
     switch(Paquete.cabecera){ // verificamos la cabecera del paquete recibido para realizar la acción sobre el ROBOT
       case 1: //función Avanzar
          // Ir a función
          
            Serial.print("Avanzar: ");
            Serial.print(Paquete.datos[1]);
            Serial.print("cm a velocidad: ");
            if(Paquete.datos[0]==2){
              Serial.print("Media");
            }else if (Paquete.datos[0]==3){
              Serial.print("Alta");
            }else{Serial.print("ERROR");}
            aux_distancia=Paquete.datos[1]; //Guardamos el dato distancia recibida en el paquete en la variable auxiliar
            aux_velocidad=Paquete.datos[0]; //Guardamos la velocidad recibida de el paquete en la variable auxiliar
            menuSelect=9;  
             
          break;
       case 2: //función Retroceder
  	  // Ir a función  
            Serial.print("Retroceder: ");
            Serial.print(Paquete.datos[1]);
            Serial.print("cm a velocidad: ");
            if(Paquete.datos[0]==2){
              Serial.print("Media");
            }else if (Paquete.datos[0]==3){
              Serial.print("Alta");
            }else{Serial.print("ERROR");}
            aux_distancia=Paquete.datos[1]; //Guardamos el dato distancia recibida en el paquete en la variable auxiliar
            aux_velocidad=Paquete.datos[0]; //Guardamos la velocidad recibida de el paquete en la variable auxiliar
            menuSelect=10;   
          break;
       case 3: //función GrioDerecha
  	  // Ir a función   
            Serial.println("Girar a la derecha al angulo: ");  
            Serial.print(Paquete.datos[1]);
            Serial.print("º y a la velocidad: ");
            if(Paquete.datos[0]==2){
              Serial.print("Media");
            }else if (Paquete.datos[0]==3){
              Serial.print("Alta");
            }else{Serial.print("ERROR");}
            aux_angulo=Paquete.datos[1]; //Guaramos el dato del angulo recibido en el paquete en la variable auxiliar
            aux_velocidad=Paquete.datos[0]; //Guardamos la velocidad recibida de el paquete en la variable auxiliar
            aux_direccion=1; //gira a la derecha
            menuSelect=14;    
          break;
       case 4: //función GrioIzquierda
  	  // Ir a función  
            Serial.println("Girar a la izquierda al angulo: ");  
            Serial.print(Paquete.datos[1]);
            Serial.print("º y a la velocidad: ");
            if(Paquete.datos[0]==2){
              Serial.print("Media");
            }else if (Paquete.datos[0]==3){
              Serial.print("Alta");
            }else{Serial.print("ERROR");}
            aux_angulo=Paquete.datos[1]; //Guaramos el dato del angulo recibido en el paquete en la variable auxiliar
            aux_velocidad=Paquete.datos[0]; //Guardamos la velocidad recibida de el paquete en la variable auxiliar
            aux_direccion=0; //gira a la izquierda
            menuSelect=14;     
          break;
       case 5: //función Redonda     
          // Ir a función
            Serial.println("Redonda"); 
            menuSelect=13;
          break;
       case 6: //función Cuadrado
          // Ir a función
            Serial.println("cuadrado de "); 
            Serial.print(Paquete.datos[1]);
            Serial.print("cm de lado y a la velocidad: ");
            if(Paquete.datos[0]==2){
              Serial.print("Media");
            }else if (Paquete.datos[0]==3){
              Serial.print("Alta");
            }else{Serial.print("ERROR");}
            aux_distancia=Paquete.datos[1]; //Guardamos el dato distancia recibida en el paquete en la variable auxiliar
            aux_velocidad=Paquete.datos[0]; //Guardamos la velocidad recibida de el paquete en la variable auxiliar
            menuSelect=12;  
          break;
       case 7: //función Triangulo
          // Ir a función
          Serial.println("triangulo de "); 
            Serial.print(Paquete.datos[1]);
            Serial.print("cm de lado y a la velocidad: ");
            if(Paquete.datos[0]==2){
              Serial.print("Media");
            }else if (Paquete.datos[0]==3){
              Serial.print("Alta");
            }else{Serial.print("ERROR");}
            aux_distancia=Paquete.datos[1]; //Guardamos el dato distancia recibida en el paquete en la variable auxiliar
            aux_velocidad=Paquete.datos[0]; //Guardamos la velocidad recibida de el paquete en la variable auxiliar
            menuSelect=11;   
          break; 
       case 8: //función Leer temperatura
           // Ir a función
           aux_temperatura=Lee_temperatura();// Lee la temperatura del sensor
           Serial.print("Temperatura: "); 
           Serial.println(String(int(aux_temperatura),DEC));
           auxiliar=String(int(aux_temperatura),DEC); //conviete el integer en String
           auxiliar.toCharArray(envio,5);    //convierte el String en char*       
           EscribePuerto(envio); //envia la temperatura en valor char.
           
       break; 
	 // Se pueden añadir las acciones que se necesiten leer del robot como acelerometro, infrarrojos...
       default:
           Serial.println("No se entiende la trama recibida"); 
         break;       
    }        
  }
  
#endif //ENTREGA_4


//Main Switch for menu interaction
  switch(menuSelect){
     case 0: //Main Menu     
       mainMenu(); //MENU FOR SERIAL INPUT!
      break;
     case 1:
		startGetTemps();
		menuSensorTemps();      
      break;
     case 2:
		startGetAccel();
		menuAccel();       
      break;
     case 3:
		startGetUltra();
		menuUltrasound();       
      break;
     case 4:
		startGetInfra(); 
		menuInfra();      
      break;
     case 5:
                menuServoRight();
      break;
     case 6:       
                menuServoLeft();
      break;
     case 7:
                menuServoHead();
      break; 
     case 8:             
                menuLEDs();
     break; 
     case 9: // Mover hacia adelante
                if(adelante_dist(aux_velocidad,aux_distancia)){
                  menuSelect=0; // si ha acabado la acción vuelve al menú principal
                  
                  freno=true;
                }
     break; 
     case 10: // Mover hacia atras             
                if(atras_dist(aux_velocidad,aux_distancia)){ 
                  menuSelect=0; // si ha acabado la acción vuelve al menú principal
                  
                  freno=true;
                }
     break; 
     case 11: // Mover en forma de triángulo             
                if(triangulo(aux_velocidad,aux_distancia)){ 
                  menuSelect=0;  // si ha acabado la acción vuelve al menú principal        
                  freno=true;
                  bloqueo_wifi=true;
                }else{
                  bloqueo_wifi=false;
                }
     break; 
     case 12: // Mover en forma de  cuadrado             
                if(cuadrado(aux_velocidad,aux_distancia)){
                  menuSelect=0;   // si ha acabado la acción vuelve al menú principal
                  
                  freno=true;
                  bloqueo_wifi=true;
                }else{
                  bloqueo_wifi=false;
                }
                    
     break; 
     case 13: // Mover en forma de  circulo            
                if(circunferencia()){ 
                   menuSelect=0;   // si ha acabado la acción vuelve al menú principal       
                   freno=true;
                bloqueo_wifi=true;
                }else{
                  bloqueo_wifi=false;
                }   
     break; 
     case 14: // gira             
                if(gira(aux_direccion,aux_velocidad,aux_angulo)){
                   menuSelect=0;   // si ha acabado la acción vuelve al menú principal       
                  
                   freno=true;
                }
     break;     
    }       
}


void interruptCallback(){ //Interrupt time
  //1s passed
  timerCounter++; 
  timerCounterfreno++;
 
  /*switch(menuSelect){     
     case 1:
	if (timerCounter==2){ 
		Escribe_temp();
	}     
      break;
     case 2:
		Escribe_aceleracion();     
      break;
     case 3:
		Escribe_distancia(distancia_obj);       
      break;
     case 4:
		Escribe_deteccion_negro();     
      break;    	 
  }     
  if (timerCounter==2){ // 2s passed	
    timerCounter=0;    
  }
  
  if (freno==true) // si está activo el freno encendemos el led rojo
  {
    timerCounterfreno=0;
    freno_timer=true;
    freno=false;
    digitalWrite(29, HIGH);
    digitalWrite(27, HIGH);
  }  
  */
  if (timerCounterfreno==5){ // contamos 5 segundos y si el freno esta activado apagamos el led rojo a los cinco segundos.	
    timerCounterfreno=0;
    //if(freno_timer==true)
    //{
      //freno_timer=false;
      //digitalWrite(29, LOW);
      //digitalWrite(27, LOW);
    //}    
    
    //Send info if we are at Control
    if (dataRX.dataType == 'C'){
      flagSendUDPControl=true;
      //EscribePuerto("C5104NM2N");
    }
    
    
    
  }
}



