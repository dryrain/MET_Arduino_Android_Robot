#include <QTRSensors.h>

#define NUM_SENSORS   6     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
//Define Ports Infrared
const int PIN_SENSOR_1  = 3;
const int PIN_SENSOR_2  = 4;
const int PIN_SENSOR_3  = 5;
const int PIN_SENSOR_4  = 2;
const int PIN_SENSOR_5  = 6; // antes 7, se cambia el pin porque hay conflicto con el WIFI. El pin 7 es el de handshake
const int PIN_SENSOR_6  = 8;
const int EMITTER_PIN   = 1;     // emitter is controlled by digital pin 2
QTRSensorsRC qtrrc((unsigned char[]) {PIN_SENSOR_1, PIN_SENSOR_2, PIN_SENSOR_3, PIN_SENSOR_4, PIN_SENSOR_5, PIN_SENSOR_6},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
  
unsigned int sensorValues[NUM_SENSORS];


void menuInfra(){
	updateInfra();
        //Lee_Infrarojos();
        //Serial.println("Jey Man");
	if (Serial.available()>0){ //We found data!
		char dataIn = Serial.read(); // Get data
		//Serial.print(dataIn);
		
		switch (dataIn){
			case 'q':
			menuSelect=0;
			stopGetInfra();
			draw();
			break;
		}
	}
}
void ini_port_infrarojo(void){
//Ini Ports Infrared
  pinMode(PIN_SENSOR_1, OUTPUT);
  pinMode(PIN_SENSOR_2, OUTPUT);
  pinMode(PIN_SENSOR_3, OUTPUT);
  pinMode(PIN_SENSOR_4, OUTPUT);
  pinMode(PIN_SENSOR_5, OUTPUT);
  pinMode(PIN_SENSOR_6, OUTPUT);
  pinMode(EMITTER_PIN, OUTPUT);
}
void stopGetInfra() { //Stops getting temps during interrupt time
	getInfrared=false;
}

void startGetInfra(){ //Starts getting temps during interrupt time
	getInfrared=true;
}

void updateInfra(){
	
	//Get values
	Lee_Infrarojos();
	//Print Values
        //Escribe_deteccion_negro();
	
}

void Lee_Infrarojos(void){
  // read raw sensor values
  qtrrc.read(sensorValues);
  // print the sensor values as numbers from 0 to 2500, where 0 means maximum reflectance and
  // 1023 means minimum reflectance
 /* for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
  }
  Serial.println();
  */
  //delay(250);
  
}

void Escribe_deteccion_negro(void){
  
 unsigned char num_negro=0; // variable para detecar cuantos sensores han encontrado negro
 Serial.print("Se detecta negro en los sensores: ");
 for (unsigned char i = 0; i < NUM_SENSORS; i++)
 {
   if(sensorValues[i]>2498){ //hay que comprobar si a partir de 1023 o cuando es 2500
      num_negro++;
      Serial.print(i+1);
      Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
   }
 }
 if(num_negro==0){
   Serial.print("<Ninguno>");
 }
 Serial.print("\n");  
}
