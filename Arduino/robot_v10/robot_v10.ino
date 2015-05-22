#include <Servo.h> //librería de los servomotores
#include <TimerOne.h> //libreria de la interrupción
#include <Wire.h> // Must include Wire library for I2C
#include <SFE_MMA8452Q.h> //libreria del acelerometro
#include <SPI.h> //libreria para interconectar la placa Mega y la shield WIFI
#include <WiFi.h> //librería para controla la transmisión WIFI
#include <WiFiUdp.h> //libreria para controlar el tráfico UDP

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
char *recibido; //puntero donde guardamos el buffer recibido en puerto WIFI.
// Formato de la trama recibida por el puerto
struct  Tramas{
  byte numero; //número de trama.
  byte cabecera; //identifica la acción a realizar (1-avanzar, 2-retroceder...)
  byte datos[8]; //guarda los datos de la acción (ejemplo velociad, dirección...)
  bool activa;   //indica si la trama está en el buffer (1- está en el buffer, 0- ya ha sido leida)
};

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
    
    
   ini_WIFI_WPA("vodafoneED7E","SCCDDUPITP6BBM"); //conectamos wifi
    //Ini Menu
    menuSelect=0;     
    
  sei();
  ini_port_acelerometro(); 
    
  drawMainMenu();
  
}

void loop() {

  struct Tramas Paquete=LeeTrama(); //Leemos el puerto por si hay paquete recibido de la WIFI.
  if (Paquete.activa && !bloqueo_wifi){
    
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
  //Main Switch
  
  switch(menuSelect){
     case 0: //Main Menu     
       mainMenu();
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
 
  switch(menuSelect){     
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
  
  if (timerCounterfreno==5){ // contamos 5 segundos y si el freno esta activado apagamos el led rojo a los cinco segundos.	
    timerCounterfreno=0;
    if(freno_timer==true)
    {
      freno_timer=false;
      digitalWrite(29, LOW);
      digitalWrite(27, LOW);
    }    
  }
}



