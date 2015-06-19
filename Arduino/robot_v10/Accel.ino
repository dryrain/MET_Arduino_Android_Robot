//Definicion de los puertos del acelerometro. PIN SCL y SDA, que se definen dentro del tipo MMA8452Q
#include <Wire.h>         // Libreria para controlar comunicaciones I2C.
#include <SFE_MMA8452Q.h> // Libreria del acelerometro.
MMA8452Q accel;           

void ini_port_acelerometro(void){ 
// Funcion para inicializar el acelerometro
  accel.init();
  Lee_aceleracion();
} 

void Lee_aceleracion(void){
// Funcion para leer la aceleracion producida en el robot en los ejes x, y, z.
   if (accel.available()){
    accel.read();
    x=accel.cx;
    y=accel.cy;
    z=accel.cz;
  }  
}

void Escribe_aceleracion(void){
// Funcion para escribir en consola el valor de la aceleración detectado.
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



