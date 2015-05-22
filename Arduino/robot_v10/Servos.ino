//ports
const int portServoLeft = 11; //PWM
const int portServoRight = 9;
const int portServoHead = 12;
const int LEFT = 0; // servo/giro izquierda
const int RIGHT = 1; // servo/giro derecha
const int HEAD= 2; // servo 180º

Servo servoLeft;
Servo servoRight;
Servo servoHead;

void menuServoLeft(){
  boolean haveToMove=false;
  int valueSerial=0;

  while ( Serial.available() > 0 ) {             
	//Check if char 'q', if not mount an integer
       valueSerial = Serial.parseInt();
       
       char dataIn = Serial.read();              
       if (valueSerial==0)dataIn='q';
         switch (dataIn){
		case 'q':
			menuSelect=0;
			draw();
                        moverServo(LEFT, 0);                  
		break;
		default:                    	
			
                        haveToMove=true; //move servo if we have a correct integer		
		break;		
	}
  }   
  //Now that we have the speed, move servos  
  if (haveToMove)
   { 
     //90 is 0 --> 0
     //180 is full left --> 90
     //0 is full left --> -90
     if (valueSerial > 90) {
       valueSerial=90;
     }else if (valueSerial < -90) {
       valueSerial=-90;
     }
     moverServo(LEFT, valueSerial);
     haveToMove=false;
     Serial.println(valueSerial);
   }
}  

void menuServoRight(){  
  boolean haveToMove=false;
  int valueSerial=0;
 
  while ( Serial.available() > 0 ) {
       valueSerial = Serial.parseInt();
       char dataIn = Serial.read();
       
       if (valueSerial==0)dataIn='q';
	switch (dataIn){
		case 'q':
			menuSelect=0;
			draw();
                        moverServo(RIGHT, 0);                 
		break;		
		default:	                       				
                        haveToMove=true; //move servo if we have a correct integer		
		break;	
	}
  }   
  //Now that we have the speed, move servos  
  if (haveToMove)
   { 
     //90 is 0 --> 0
     //180 is full right --> 90
     //0 is full left --> -90
     if (valueSerial > 90) {
       valueSerial=90;
     }else if (valueSerial < -90) {
       valueSerial=-90;
     }
     moverServo(RIGHT, valueSerial);
     haveToMove=false;
     Serial.println(valueSerial);
   }
}  

void menuServoHead(){  
  boolean haveToMove=false;
  int value = 0; 
  while ( Serial.available() > 0 ) {
       value = Serial.parseInt();
       char dataIn = Serial.read();
       
       if (value==0)dataIn='q';
     
	switch (dataIn){
		case 'q':
			menuSelect=0;
			draw();
                        moverServo(2,0);                 
		break;		
		default:	                       				
                        haveToMove=true; //move servo if we have a correct integer		
		break;	
	}
  }   
  //Now that we have the speed, move servos  
  if (haveToMove)
   { 
     if (value >90) value=90;
     if (value <-90) value=-90;
     Serial.println(value);
     value=value+90;
     moverServo(2,value);
     haveToMove=false;   
   }  
}

void moverServo(int nservo, int pos){ 
   switch (nservo){
     case LEFT:
         pos=abs(pos-90); //codifica la velocidad de -90 a 90 en 0 a 180º
         servoLeft.write(pos); 
     break;	
     case RIGHT:	
         pos=pos+90; //codifica la velocidad de -90 a 90 en 0 a 180º
         servoRight.write(pos); 
     break;
     default:	  // Gira el servo control de 180º                    				
         servoHead.write(pos);		
     break;	
   }
}

void ini_port_servo_Left(void)
{
  servoLeft.attach(portServoLeft);  
  servoLeft.write(90);
    
}

void ini_port_servo_Right(void)
{
   servoRight.attach(portServoRight);  
   servoRight.write(90);
}

void ini_port_servo_Head(void)
{
   servoHead.attach(portServoHead);
   servoHead.write(90);
}

void reset_port_servo_Left(void)
{
  servoLeft.detach();
}

void reset_port_servo_Right(void)
{
   servoRight.detach();  
}

void reset_port_servo_Head(void)
{
   servoHead.detach();
}




