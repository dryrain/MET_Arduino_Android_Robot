#include <Servo.h> //librería de los servomotores

//Definicion de los puertos de los servocontroles.
const int portServoLeft = 6; // PIN motor izquierdo.
const int portServoRight = 9; // PIN motor derecho.
const int portServoHead = 5; // PIN motor de giro del sensor de ultrasonidos.

//Definicion de constantes.
const int LEFT = 0;           // servo/giro izquierda
const int RIGHT = 1;          // servo/giro derecha
const int HEAD= 2;            // servo 180º

//Definicion de tipos.
Servo servoLeft;              // Establece el tipo servo para el motor izquierdo.
Servo servoRight;             // Establece el tipo servo para el motor derecho.
Servo servoHead;              // Establece el tipo servo para el motor de giro del sensor de ultrasonidos.


void ini_port_servo_Left(void){
//Inicializa el motor izquierdo.
  servoLeft.attach(portServoLeft);  
  servoLeft.write(90);    
}

void ini_port_servo_Right(void){
//Inicializa el motor derecho.
   servoRight.attach(portServoRight);  
   servoRight.write(90);
}

void ini_port_servo_Head(void){
//Inicializa el motor de giro del sensor de ultrasonidos.
   servoHead.attach(portServoHead);
   servoHead.write(91);
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

void reset_port_servo_Left(void){
//Inicializa el motor izquierdo.
  servoLeft.detach();
}

void reset_port_servo_Right(void){
//Inicializa el motor derecho.
   servoRight.detach();  
}

void reset_port_servo_Head(void){
//Inicializa el motor de giro del sensor de ultrasonidos.
   servoHead.detach();
}




