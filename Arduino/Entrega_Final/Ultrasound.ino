//Definicion de los puertos del sensor de ultrasonidos.
const int trigPin =  49; //PIN Trigger.
const int echoPin =  48; //PIN echo.
int EstadoDistancia=0;
unsigned long timer_ini;
void ini_port_ultrasound(void){ 
//Inicializa los puertos del sensor de ultrasonido.
  pinMode(trigPin, OUTPUT); // Establece pin (49) de trigger como salida.
  pinMode(echoPin, INPUT); // Establece pin (48) echo/ como entrada.
}

long Lee_ultrasonidos (void){
//Funcion que lee la distancia a un objeto.
//Salida: (long). Devuelve la distancia (en cm) al objeto mas cercano.
  long distancia, eco;          // Declara variables.
// Pulso de 10us para inicial el modulo.
  digitalWrite(trigPin, HIGH);  
  delayMicroseconds(10);        // Creamos el trigger para activar la señal de ultrasonidos.
  digitalWrite(trigPin, LOW);   
  eco = pulseIn(echoPin, HIGH); // Devuelve la longitud del pulso del pin Echo en us.
  distancia = eco/58;           // a partir del eco encontramos la distancia.
  delay(50);                    // Espera 50ms para la siguiente medición (Tiempo mínimo recomendado!!)
  return distancia;             // Devuelve la distancia detectada al objeto mas proximo.
}

bool detectaCentro(){
  //Movemos el servo al centro
  moverServo(HEAD,90);
  delay(1000);
  //Lectura de la distancia
  long distancia = Lee_ultrasonidos();
  if (distancia<20) return true;
  else false;
}

bool detectaDerecha(){
  //Movemos el servo al centro
  moverServo(HEAD,0);
  delay(1000);
  
  //Lectura de la distancia
  long distancia = Lee_ultrasonidos();
  if (distancia<20) return true;
  else false;
}

bool detectaIzquierda(){
  //Movemos el servo al centro
  moverServo(HEAD,180);
  delay(1000);
  //Lectura de la distancia
  long distancia = Lee_ultrasonidos();
  if (distancia<20) return true;
  else false;
}


