//Define Ports Acelerometro
MMA8452Q accel;

void menuAccel(){
	updateAccel();
  /*      if(accel.cx<0)
        {
          digitalWrite(29,HIGH);
          Serial.println("Atras");
        }else{
          digitalWrite(29,LOW);
        }*/
        delay(300);
	if (Serial.available()>0){ //We found data!
		char dataIn = Serial.read(); // Get data
		//Serial.print(dataIn);
		
		switch (dataIn){
			case 'q':
			menuSelect=0;
			stopGetAccel();
			draw();
			break;
		}
	}
}
void ini_port_acelerometro(void){ 
  accel.init();
  Lee_aceleracion();
} 
void stopGetAccel() { //Stops getting temps during interrupt time
	getAccel=false;
}

void startGetAccel(){ //Starts getting temps during interrupt time
	getAccel=true;
}

void updateAccel(){
	
	//Get values
	Lee_aceleracion();
	//Print Values
	
}
float Lee_marcha_atras(void){
  Lee_aceleracion();
  return accel.cx;
  
  
  
}
void Lee_aceleracion(void){
   if (accel.available())
  {
    // First, use accel.read() to read the new variables:
    accel.read();
  }
}
void Escribe_aceleracion(void){
  Serial.print("x: ");
  Serial.print(accel.cx, 3);
  Serial.print("\t");
  Serial.print("y: ");
  Serial.print(accel.cy, 3);
  Serial.print("\t");
  Serial.print("z: ");
  Serial.print(accel.cz, 3);
  Serial.println("\t");
}



