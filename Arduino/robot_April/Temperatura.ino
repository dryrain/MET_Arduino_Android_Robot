//Define Ports Tª
const int slaveSelectPin =  40;//revisar 40,41,42
const int clockPin       =  41;
const int dataPin        =  39;
float temperatura=0;

void menuSensorTemps(){
	updateTemps();
	if (Serial.available()>0){ //We found data!
		char dataIn = Serial.read(); // Get data
                
		//Serial.print(dataIn);
		
		switch (dataIn){
			case 'q':
				menuSelect=0;
				stopGetTemps();
				draw();   
			break;
		}	
	}	
}

void ini_port_temperatura(void){
//Ini Ports Temperature
  pinMode(clockPin, OUTPUT);
  pinMode(slaveSelectPin, OUTPUT);
  pinMode(dataPin, INPUT);  
  
}

void stopGetTemps() { //Stops getting temps during interrupt time
	getTemps=false;
}

void startGetTemps(){ //Starts getting temps during interrupt time
	getTemps=true;
}


int updateTemps(){
	
	//Get values
	temperatura=Lee_temperatura();
}
float Lee_temperatura(void){  
        int data=0;
        float temp;
	digitalWrite(clockPin, LOW);       // Start with clock low
        digitalWrite(slaveSelectPin, LOW); // Start transfer by setting CS=LOW
 
        for (int i = 12; i>=0; i-=1) {     // Read the next 12 bits
             digitalWrite(clockPin, LOW);   // Clock pulse
             digitalWrite(clockPin, HIGH);
             data += (digitalRead(dataPin) << i); // Read in value
        }

        digitalWrite(slaveSelectPin, HIGH); // Finish transfer
        temp=data*0.0625;
	return temp;   
}

void Escribe_temp(void){

     Serial.println("Temperatura de ");

     Serial.println(temperatura );
     
     Serial.println("ºC");
     

  
  
  
}
