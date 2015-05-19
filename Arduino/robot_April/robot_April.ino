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


//const IPAddress IPSend (170,20,10,13);
const char IPSend[] = "192.168.0.194";
const int sendPort = 55056;
//const int sendPort = 4560;
//const char* WIFIName = "AndroidJordi";
//const char* WIFIPass ="sergisergi";

const char* WIFIName = "vodafoneED7E";
const char* WIFIPass ="SCCDDUPITP6BBM";

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
  byte datos[9]; //guarda los datos de la acción (ejemplo velociad, dirección...)
  bool activa;   //indica si la trama está en el buffer (1- está en el buffer, 0- ya ha sido leida)
};

struct WifiRx{
 char dataType;
 int  turnAngle;
 int  speedValue;
 char manualAuto;
 char gestureTrigger;
 char luces;
};
WifiRx dataRX; //Declare a struct used to hold received data
struct EstadoRobot{
 char dataType ='C';  // tipo de de trama
 int temp = 10;       // temperatura del robot
 char luces = 'N';      // Luces delanteras encendidas: Y(yes) o N(no)
 int colision = 0; // detecta si hay colisión delantera o trasera: 0(no hay), 1(colisión delantera), 2(colisión trasera izq.) y 3(colisión trasera derch.)
 char manualAuto ='M';// Tipo de modo: M(manual) y A(automático)
 int speedValue =0;   // Velocidad del robot: 0 (parado), 1, 2, 3 (hacia delante) y 4, 5, 6 (hacia atrás)
 int  turnAngle=1;    // Giro del robot: 1 (recto), 2 (der. lento), 3(der. rápido), 4 (izq. lento) y 3(izq. rápido).
 int velocidad_auto=0;
};
EstadoRobot Estado; // Estructura donde se almacena los datos a enviar al teléfono.
bool cercano=false;  //True: obstaculo cerca, false no hay obstaculo
bool girando=false; // True:indica que está girando. False:

//Variables de memoria de direccion, distancia, velocidad y angulo
int aux_distancia=2; //distancia en cm
int aux_velocidad=3;  //0-stop, 2-media y 3-alta
int velocidad=30;
int velocidad2;
int direc=0;
int angulo=300;

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
    
    ini_WIFI_WPA("hola","adiosadios"); //conectamos wifi
  // ini_WIFI_WPA("vodafoneED7E","SCCDDUPITP6BBM"); //conectamos wifi
   //ini_WIFI_WPA("Dryrain","sibemolsibemo");
  // ini_WIFI_WPA("IphonePA","sable1992");
    //Ini Menu
    menuSelect=0;     
    char manualoAuto = 'M';        //inicializamos el estado inicial como manual
    Estado.manualAuto=manualoAuto; //inicializamos el estado inicial como manual
  sei();
  ini_port_acelerometro(); 
    
  drawMainMenu();
  
}

void loop() {


char* data;
data = readUDP();
if (data != "E" && !bloqueo_wifi){ //We found something
  
  Serial.print("Data: ");
  Serial.println(data);
  
  
  //Parsing the string -------------------------------
  //1) Type ( C - control, L - Laberynt, A - Accel , X - STOP)
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
  Estado.luces=luces;  
  
  //dataRX UPDATED!!!
  
  //END Parsing ---------------------------------------
  
  
  //Do whatever is needed to ...
  switch (dataRX.dataType){ //Reply control data
    case 'C':
        //Will be handled during interrupt
        if (dataRX.manualAuto=='A'){ //The robot is moving on his own
           //manualOrAuto=Estado.manualAuto;
           if(Estado.manualAuto=='M'){// primera vez que se activa el modo
              Estado.manualAuto='A';
              flagSendUDPControl=true; // enviamos trama indicando el cambio a estado automático
              if(Estado.speedValue!=0){// guardamos la velocidad por del modo manual para utilizar en modo automático
                Estado.velocidad_auto=Estado.speedValue; 
              }else{ //si está parado le asignamos una velocidad de automático de 1.
                Estado.velocidad_auto=1; 
              }
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
            menuSelect=5;
          }else if (dataRX.gestureTrigger=='S'){
            menuSelect=6;
          }else if (dataRX.gestureTrigger=='T'){
            menuSelect=7;
          }else
          {
                
            
           //movimiento
            
       
          }
         
          //Giro
          //Velocidad
          //Freno
         
       }
            
        
        
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

if (dataRX.dataType=='C'){
  if (dataRX.luces!=Estado.luces){ //comprueba si hay cambio de las luces delanteras.
                  Estado.luces=luz_cruce(); //Activa o desactiva la luz delantera
                  flagSendUDPControl=true; // enviamos trama indicando el estado de la luz delantera
  }   
  if(Lee_ultrasonidos()<20){
    
     if(cercano){
       flagSendUDPControl=true; // enviamos trama indicando la posible colisión delantera 
       Estado.colision=1;
       cercano=false;    
       Serial.println("blouqeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");
     }  
       girando=true; 
  }else{
  //--------------->Poner else if con las lecturas de las colisiones traseras y delanteras                
     if(!cercano){
       flagSendUDPControl=true; // enviamos trama indicando la posible colisión delantera   
       Estado.colision=0;
       cercano=true;
     }  
        
  }
}
if(flagSendUDPControl){
  sendControlUDP(Estado.temp,Estado.luces,Estado.colision,Estado.manualAuto,Estado.speedValue);
  flagSendUDPControl=false;
}


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
                  mover(Estado.velocidad_auto,1);
                }
                
                       
      break;
     case 2: // modo acelerometro
		   
      break;
     case 3: // modo laberinto
		      
      break;
     case 4: // Modo manual           
                mover(Estado.speedValue,Estado.turnAngle);
                
      //          menuSelect=0;
                freno=true;    
      break;
     case 5: // Mover en forma de  circulo            
                if(circunferencia()){ 
                   menuSelect=0;   // si ha acabado la acción vuelve al menú principal       
                   freno=true;
                bloqueo_wifi=false;
                }else{
                  bloqueo_wifi=true;
                }   
      break;
     case 6: // Mover en forma de  cuadrado             
                if(cuadrado(aux_velocidad,aux_distancia)){
                  menuSelect=0;   // si ha acabado la acción vuelve al menú principal
                  
                  freno=true;
                  bloqueo_wifi=false;
                }else{
                  bloqueo_wifi=true;
                }
      break;
     case 7: // Mover en forma de triángulo             
                if(triangulo(aux_velocidad,aux_distancia)){ 
                  menuSelect=0;  // si ha acabado la acción vuelve al menú principal        
                  freno=true;
                  bloqueo_wifi=false;
                }else{
                  bloqueo_wifi=true;
                }
      break; 
      case 8: // giro 45º             
                if(gira(1,3,45)){ 
                  menuSelect=1;
                
                }
      break; 
    }       
}


void interruptCallback(){ //Interrupt time
  //1s passed
  timerCounter++; 
  timerCounterfreno++;
 
  
  if (timerCounter==5){ // 5s passed	
    timerCounter=0;   
    //Send info if we are at Control
    if (dataRX.dataType == 'C'){
      Estado.temp = (int)Lee_temperatura();
      flagSendUDPControl=true;
  }
  
  if (freno==true) // si está activo el freno encendemos el led rojo
  {
    timerCounterfreno=0;
    freno_timer=true;
    freno=false;
    digitalWrite(29, HIGH);
    digitalWrite(27, HIGH);
  }  
  
  if (timerCounterfreno==5){ // contamos 5 segundos y si el freno esta activado apagamos el led rojo a los cinco segundos.	
    timerCounterfreno=0;
    //if(freno_timer==true)
    //{
      //freno_timer=false;
      //digitalWrite(29, LOW);
      //digitalWrite(27, LOW);
    //}       
    } 
  }
  

}



