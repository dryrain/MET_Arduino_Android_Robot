
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
     }
     draw();    
  }
}








