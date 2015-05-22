void draw(){
 
 if (menuSelect==0){
	drawMainMenu(); 
 }  
 if (menuSelect==1){
	 //drawMenuTemps(); Not Used!
 }
 if (menuSelect==2){
	//drawMenuAccel(); Not Used!
 }
 if (menuSelect==3){
	 //drawMenuUltra(); Not Used!
 }
 if (menuSelect==4){
	 //drawMenuInfra(); Not Used!
 }
 if (menuSelect==5){
	 drawMenuServo();
 }
 if (menuSelect==6){
	 drawMenuServo();
 }
 if (menuSelect==7){
	 drawMenuServoHead();
 }
 if (menuSelect==8){
    startGetLEDs();
   }
}



void drawMainMenu(){
 Serial.println("Main Menu:");
 Serial.println("1 -- Sensor de Temperatura");
 Serial.println("2 -- Sensor de Aceleracion");
 Serial.println("3 -- Sensor de Ultrasonidos");
 Serial.println("4 -- Sensor de Infrarojos");
 Serial.println("5 -- Motor Derecho");
 Serial.println("6 -- Motor Izquierdo");
 Serial.println("7 -- Motor 180 grados");
 Serial.println("8 -- LED's");
 Serial.println("a -- Adelante");
 Serial.println("b -- Atras");
 Serial.println("c -- Triangulo");
 Serial.println("d -- Cuadrado");
 Serial.println("e -- Circulo");

}


void drawMenuServo() {
 Serial.println("Introduzca velocidad [-90 : 90]");
} 

void drawMenuServoHead(){
 Serial.println("Introduzca grado de giro [-90 : 90]");
}

