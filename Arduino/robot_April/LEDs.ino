
//Define Ports LEDs
const int PIN_LED_1 =  29; // Led rojo trasero izquierdo
const int PIN_LED_2 =  39; // Led blanco delantero derecho
const int PIN_LED_3 =  27; // Led rojo trasero derecho
const int PIN_LED_4 =  37; // Led blanco delantero izquierdo

void menuLEDs(){
	
	if (Serial.available()>0){ //We found data!
		char dataIn = Serial.read(); // Get data
		//Serial.print(dataIn);
		
		switch (dataIn){
			case 'q':
    			  menuSelect=0;
    			  stopGetLEDs();
    			  draw();
			break;
                        case '1':
    			  digitalWrite(PIN_LED_1, !digitalRead(PIN_LED_1));    			      			  
			break;
                        case '2':
    			  digitalWrite(PIN_LED_2, !digitalRead(PIN_LED_2)); 
    			  
			break;
                        case '3':
    			  digitalWrite(PIN_LED_3, !digitalRead(PIN_LED_3)); 
    			  
			break;
                        case '4':
    			  digitalWrite(PIN_LED_4, !digitalRead(PIN_LED_4)); 
    			  
			break;

		}
	}
}
void ini_port_LEDS(void){
//Ini Ports LEDs
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);
}
void stopGetLEDs() { //Stops getting temps during interrupt time
	//getLEDs=false;
        digitalWrite(PIN_LED_1, LOW);
        digitalWrite(PIN_LED_2, LOW);
        digitalWrite(PIN_LED_3, LOW);
        digitalWrite(PIN_LED_4, LOW);
}

void startGetLEDs(){ //Starts getting temps during interrupt time
	//getLEDs=true;    
        Serial.print("Presione 1,2,3 o 4 para activar y desactivar los LEDs.");
}


int updateLEDs(){
  return 4; //TEST
	
	//Get values
	
	//Print Values
	
}


