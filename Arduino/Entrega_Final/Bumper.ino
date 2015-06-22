//PUERTO 22 ES EL BUMPER DERECHO
//PUERTO 24 ES EL BUMPER IZQUIERDO
//Definicion de los puertos de las luces del robot
const int PIN_BUMPER_DERCH =  22; // Bumper dercho;
const int PIN_BUMPER_IZQ   =  24; // Bumper izquierdo;

void ini_port_BUMPERS(void){
//Funcion para inicializar los puertos de los 2 BUMPERS
  pinMode(PIN_BUMPER_DERCH, INPUT); //Inicializamos los puertos como entradas.
  pinMode(PIN_BUMPER_IZQ, INPUT);   //Inicializamos los puertos como entradas.
}

int Lee_bumpers (void){
  //Serial.println(digitalRead(PIN_BUMPER_DERCH));
  if(digitalRead(PIN_BUMPER_DERCH)==HIGH){
    return 3;
  }else{
    if(digitalRead(PIN_BUMPER_IZQ)==HIGH){
      return 2;
    }else{
      return 0;
    }
  }
}
  
  
  
  
  
  
  
  

