//Definicion de los puertos de las luces del robot
const int PIN_LED_1 =  29; // Led rojo trasero izquierdo
const int PIN_LED_2 =  39; // Led blanco delantero derecho
const int PIN_LED_3 =  27; // Led rojo trasero derecho
const int PIN_LED_4 =  37; // Led blanco delantero izquierdo

void ini_port_LEDS(void){
//Funcion para inicializar los puertos de los 4 LEDs
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);
}
void stopGetLEDs() { 
//Funcion para apagar todos los LEDs
        digitalWrite(PIN_LED_1, LOW);
        digitalWrite(PIN_LED_2, LOW);
        digitalWrite(PIN_LED_3, LOW);
        digitalWrite(PIN_LED_4, LOW);
}

void startGetLEDs(){ //Starts getting temps during interrupt time
	//getLEDs=true;    
        Serial.print("Presione 1,2,3 o 4 para activar y desactivar los LEDs.");
}

char luz_cruce(){ 
//Funcion para encender y apagar las luces.
//Salida: (char). devuelve el estado final "y" (encendidas) o "n" (apagagas).
  digitalWrite(PIN_LED_2, !digitalRead(PIN_LED_2)); 
  digitalWrite(PIN_LED_4, !digitalRead(PIN_LED_4));
  if(digitalRead(PIN_LED_2)==HIGH)
  {
    return 'Y';
  }else
  {
    return 'N';
  }
}


void Luz_Freno(bool activa){
//Funcion para encender y apagar luz de freno
//Salida: True: enciende luz, false:apaga luz

  if(activa){
    digitalWrite(PIN_LED_1, HIGH);
    digitalWrite(PIN_LED_3, HIGH);
  }else{
    digitalWrite(PIN_LED_1, LOW);
    digitalWrite(PIN_LED_3, LOW);
  }
} 
