void mainMenu(){ 
// Funcion para controlar las opciones pulsadas durante el control del robot mediante consola. 
  if (Serial.available()>0){     // Si detecta que hay una tecla pulsada
    char dataIn = Serial.read(); // Lee la tecla pulsada 
     switch (dataIn){   
       case '0': 
         //No se hace nada
        break;
       case '1': // Modo automatico.  
        Estado.dataType='C';
        Estado.manualAuto='A';
        menuSelect=1;   
        break;
       case '2': // Modo reto acelerometro. 
        Estado.dataType='A';
        menuSelect=2;   
        break;
       case '3': // Modo reto laberinto.
        menuSelect=3;
        estadoLaberinto=0;
        flagLaberinto=true;
        
        break;       
     }   
  }
}








