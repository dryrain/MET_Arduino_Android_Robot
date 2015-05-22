
void mainMenu(){ 
  
  if (Serial.available()>0){ //We found data!
    char dataIn = Serial.read(); // Get data 
    //Serial.print(dataIn);
   
     switch (dataIn){   
       case 'q':
		//Nothing Happens
         break;      
       case '0': //Main Menu
         //Nothing happens
        break;
       case '1':   
        menuSelect=1;   
        break;
       case '2':   
        menuSelect=2;   
        break;
       case '3': 
        menuSelect=3;   
        break;
       case '4':  
         menuSelect=4;   
        break;
       case '5':
         menuSelect=5;
        break;
       case '6':
         menuSelect=6;
        break;
       case '7':
         menuSelect=7;
        break;       
       case '8':
         menuSelect=8;
       break;    
      case 'a': //adelante
          menuSelect=9;
       break;      
       case 'b': //atras
         
         menuSelect=10;
       break; 
       case 'c': //triangulo         
         menuSelect=11;
       break; 
       case 'd': // cuadrado
        // menuGira();
         menuSelect=12;
       break; 
       case 'e': //circulo
         menuSelect=13;
       break; 
       
     }
     draw();    
  }
}








