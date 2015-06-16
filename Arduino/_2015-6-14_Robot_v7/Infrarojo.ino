//Libreria para controlar el sensor de infrarojos.
#include <QTRSensors.h>

#define NUM_SENSORS   6     // nº de sensores usados.
#define TIMEOUT       2500  // Espera 2,5s hasta que la salida de los sensores pasan a LOW.

//Definicion de los puertos de los sensores de infrarojos.
const int PIN_SENSOR_1  = 30;
const int PIN_SENSOR_2  = 31;
const int PIN_SENSOR_3  = 32;
const int PIN_SENSOR_4  = 33;
const int PIN_SENSOR_5  = 34; 
const int PIN_SENSOR_6  = 35;
const int EMITTER_PIN   = 2;   

// Inicializacion de los sensores, utilizando el tipo QTRSensorsRC.
QTRSensorsRC qtrrc((unsigned char[]) {PIN_SENSOR_1, PIN_SENSOR_2, PIN_SENSOR_3, PIN_SENSOR_4, PIN_SENSOR_5, PIN_SENSOR_6},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
  
  //Tabla de enteros donde se guarda los valores recogidos por los sensores.

void ini_port_infrarojo(void){
//Funcion para incializar los puertos de los sensores de infrarojos.
  pinMode(PIN_SENSOR_1, OUTPUT);
  pinMode(PIN_SENSOR_2, OUTPUT);
  pinMode(PIN_SENSOR_3, OUTPUT);
  pinMode(PIN_SENSOR_4, OUTPUT);
  pinMode(PIN_SENSOR_5, OUTPUT);
  pinMode(PIN_SENSOR_6, OUTPUT);
  pinMode(EMITTER_PIN, OUTPUT);
}

void Lee_Infrarojos(void){
// Funcion para leer los datos recibidos por los sensores de infrarojos.
  qtrrc.read(sensorValues);  
}

void Escribe_deteccion_negro(void){
//Funcion para visualizar en consola los valores recogidos por los sensores de infrarojos.  
 unsigned char num_negro=0; // variable para detecar cuantos sensores han encontrado negro
/* Serial.print("Se detecta negro en los sensores: ");
 for (unsigned char i = 0; i < NUM_SENSORS; i++){
   if(sensorValues[i]>2000){ //hay que comprobar si a partir de 1023 o cuando es 2500
      num_negro++;
      Serial.print(i+1);
      Serial.print('\t'); 
   }
 }
 if(num_negro==0){
   Serial.print("<Ninguno>"); // si no detecto negro devuelve el valor <Ninguno>
 }
 Serial.print("\n");  */

 for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
  }
  Serial.println();
}
