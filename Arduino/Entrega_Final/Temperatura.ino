//Definicion de los puertos del sensor de temperatura.
const int slaveSelectPin =  40; // PIN Slave.
const int clockPin       =  43; // PIN Clock.
const int dataPin        =  42; // PIN Data.

void ini_port_temperatura(void){
//Inicializa los puertos del sensor de ultrasonido.
  pinMode(clockPin, OUTPUT);       // Establece pin (43) como Clock de salida.
  pinMode(slaveSelectPin, OUTPUT); // Establece pin (40) como Slave de salida.
  pinMode(dataPin, INPUT);         // Establece pin (42) como Data de entrada.
}

float Lee_temperatura(void){ 
// Funcion para leer la temperatura recogido por el sensor de temperatura.
// Salida: (float). Devuelve la temperatura en el ambiente en grados celsius.
        int data=0;
        float temp;
	digitalWrite(clockPin, LOW);             // Inicializa el Clock a valor LOW.
        digitalWrite(slaveSelectPin, LOW);       // Inicializa el CS a valor LOW.
 
        for (int i = 12; i>=0; i-=1) {           // Lee 12 bits.
             digitalWrite(clockPin, LOW);        // Crea un pulso en el Clock.
             digitalWrite(clockPin, HIGH);       // Crea un pulso en el Clock.
             data += (digitalRead(dataPin) << i);// Lee el valor
        }

        digitalWrite(slaveSelectPin, HIGH);      // Finaliza la transferencia. 
        temp=data*0.0625;                        // Convierte la temperatura a escala de grados celsius.
	return temp;                             // Devuelve el valor de temperatura recogido por el sensor.
}

