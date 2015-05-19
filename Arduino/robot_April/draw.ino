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
 Serial.println("1 -- Modo automatico");
 Serial.println("2 -- Reto acelerometro");
 Serial.println("3 -- Laberinto");
}


void drawMenuServo() {
 Serial.println("Introduzca velocidad [-90 : 90]");
} 

void drawMenuServoHead(){
 Serial.println("Introduzca grado de giro [-90 : 90]");
}

