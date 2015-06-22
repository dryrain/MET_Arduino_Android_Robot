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
        ini_posicion_robot();
        ini_Laberinto();
        ini_Laberinto_Solucion();
        ini_Laberinto_Total();
        estadoLaberinto=CambiaEstado(1);  
        Estado.dataType='L';
        Estado.FuncionLaberinto='N';
        break;   
       
       case '4': // Modo reto laberinto.
        if (laberintoSolucionado){
          menuSelect=3;
          ini_posicion_robot();                   // Inicializamos el robot en la posicion inicial.
          LaberintoTotal[RobL.PosY][RobL.PosX]=9; // Dibujamos el robot en la posición inicial.            
          estadoLaberinto=CambiaEstado(100);  
          Estado.dataType='L';
          Estado.FuncionLaberinto='S';
        }else{
          Serial.println("Opcion no valida: No se ha solucionado el laberinto");
        }
        
        break;  
     
     case 'q': // Modo reto acelerometro. 
         mover(0,1);
         menuSelect=0;
         resetHEAD();        
         flagSendUDPControl=false;
         flagSendAccelUDP=false; 
        break;   
     }   
  }
}








