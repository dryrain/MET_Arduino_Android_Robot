//Define Ports Ultrasounds
const int trigPin =  25;
const int echoPin =  24;

void menuUltrasound(){
	updateUltra();
	if (Serial.available()>0){ //We found data!
		char dataIn = Serial.read(); // Get data
		//Serial.print(dataIn);
		
		switch (dataIn){
			case 'q':
			menuSelect=0;
			stopGetUltra();
			draw();
			break;
		}
	}
}
void ini_port_ultrasound(void){
//Ini Ports Ultrasound
  pinMode(trigPin, OUTPUT); // Establece pin (51) de trigger como salida
  pinMode(echoPin, INPUT); // Establece pin (53) echo/ como entrada
}
void stopGetUltra() { //Stops getting temps during interrupt time
	getUltrasound=false;
}

void startGetUltra(){ //Starts getting temps during interrupt time
	getUltrasound=true;
        digitalWrite(trigPin, LOW); // Pone el pin a un estado logico bajo
}


void updateUltra(){
	
	//Get values
	distancia_obj=Lee_ultrasonidos();
	//Print Values
        //Escribe_distancia(distancia_obj);
	
}

long Lee_ultrasonidos (void){
  long distancia, eco; // Declara variables
  // Pulso de 10us para inicial el modulo
  digitalWrite(trigPin, HIGH); //  
  delayMicroseconds(10);       // Creamos el trigger para activar la señal de ultrasonidos
  digitalWrite(trigPin, LOW);  // 
  eco = pulseIn(echoPin, HIGH); //Devuelve la longitud del pulso del pin Echo en us
  distancia = eco/58; // a partir del eco encontramos la distancia.
  delay(50); // Espera 50ms para la siguiente medición (Tiempo mínimo recomendado!!)
  return distancia;
}

void Escribe_distancia(long distancia){
  
 // Imprime valores por el puerto serie:
  Serial.print("Distancia: "); 
  Serial.print(distancia); 
  Serial.print("cm");
  Serial.println();   
}
