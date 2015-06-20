#include <SPI.h> //libreria para interconectar la placa Mega y la shield WIFI.
#include <WiFi.h> //librería para controla la transmisión WIFI.
#include <WiFiUdp.h> //libreria para controlar el tráfico UDP

int status = WL_IDLE_STATUS;           // Estado de la señal radio del WIFI.
WiFiUDP Udpread;                       // Tipo para guardar la trama UDP leida.
//unsigned int readPort = 2390;        // puerto local para leer datos.
unsigned int readPort = 55056; 
char packetBuffer[255];                // Buffer para leer el paquete UDP.
const char IPSend[] = "172.20.10.13";
//const char IPSend[] = "192.168.0.194";
//const char IPSend[] = "192.168.43.228";
const int sendPort = 4560 ;            // puerto remoto para enviar datos al móvil.
const int sendPortLaberinto = 4561 ; 

IPAddress IPRx;
int PortRx;


void ini_WIFI_WPA(char *c_ssid, char *c_pass){  
// Funcion para inicializar la conexion WIFI entre el robot y el telefono movil.
// Entradas:
//  c_ssid: (char *). Nombre del SID WIFI.
//  c_pass: (char *). Contraseña de la red WIFI.

  if (WiFi.status() == WL_NO_SHIELD) { // Comprueba si esta presente la shield WIFI en el robot.
    Serial.println("WiFi shield no esta conectada");
    while (true);
  }
  while ( status != WL_CONNECTED) {  // Realiza la conexión con la red WIFI con el SID y el pass introducidos en la funcion.
    Serial.print("Intentando conectar con la WPA SSID: ");
    Serial.println(c_ssid);
    status = WiFi.begin(c_ssid, c_pass); // Realiza la conexion WP
    delay(10000);                        // Añade un retardo de 10s para comprobar que la conexión es correcta
  }
  Udpread.begin(readPort);               // Inicializa la lectura de tramas UDP en el puerto
  printCurrentNet();                     // Visualiza la SSID, BSSID...
  printWifiData();                       // Visualiza los datos de la shield wifi (IP,MAC)
}

char *readUDP(void){  // Escuchamos el puerto y devolvemos toda la trama en formato char.
// Funcion para devolver la trama recibida en el puerto de escucha.
// Salida:
//  (char *). Devuelve el puntero de la trama recibida. Y si no hay dato devuelve la letra "E".
//
  int packetSize = Udpread.parsePacket();  // Si hay paquete devuelve el tamaño.
  if (packetSize)  //
  {
    Serial.print("From ");
    IPAddress remoteIp = Udpread.remoteIP(); // Lee la IP remota que envia la trama.
    IPRx=remoteIp;
    Serial.print(remoteIp);
    Serial.print(", port ");
    PortRx=Udpread.remotePort();
    Serial.println(PortRx);
    // read the packet into packetBufffer
    int len = Udpread.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0'; //Put a line ending
      Serial.print("Lenght Detected:");
      Serial.println(len);
    }
    return packetBuffer;
  }else{    
    return "E"; //No hay datos en cola.
  }  
}

void sendControlUDP(int temp,int luces,int colision,char manualAuto,int speedValue){
  //Data to send: temp,LEDs,colision,manual/auto,speed,proximity
  //Udpread.beginPacket("192.168.1.34", 4560); //Android Jordi Casa
  
  char dataTX[10];
  /*
  int temp = (int)Lee_temperatura();//Casting from float
  int LEDs = updateLEDs();
  char colision = 'N';    //char colision = getColisionStatus();
  char manualAuto = 'M';  //char manualAuto = getManualAutoStatus();
  int speedValue = 2;     //int speedValue = getSpeedValue();*/
  char proximity = 'N';   //char proximity = getProximityStatus();
  
 
  //Starting TX protocol
  dataTX[0]='C'; //Data Type Control
  
  String str = String(temp);
  dataTX[1]=str[0];
  dataTX[2]=str[1];
  //dataTX[3]=str[2];
  
 // str = String(Luces); 
  //dataTX[4]=str.toCharArray(b,1);
  dataTX[3]=luces;
  str = String(colision);
  dataTX[4]=str[0];
  
  dataTX[5]=manualAuto;
  str = String(speedValue);
  dataTX[6]=str[0];
  dataTX[7]=proximity;
  dataTX[8]='\0';//End
  
  Serial.print("Data Sent: ");
  Serial.println(dataTX);
  
  
  //IPAddress remoteIp = Udpread.remoteIP();
   Serial.print(IPSend);
   Serial.print(", port ");
   //Serial.println(Udpread.remotePort());
   Serial.println(sendPort);
  Udpread.beginPacket(IPSend, sendPort); //Android Jordi UNI
  Udpread.write(dataTX,sizeof(dataTX));
 // Udpread.write("Hola");
  Udpread.endPacket(); 
  
  
  
}

void sendLaberynthUDP(){
  //Data to send: position,path,wallsFound,solution
  //int labXpos = 3; //getLabXpos();
  //int labYpos = 5; //getLabYpos();
 // String labPath = "0010100101001010010100101"; //getLabPath();
  //int wallsHit = 3; //getWallsHit();
  //String labSolution = "0010100101111010010100101"; //getLabSolution();
  char charLaberinto[83];
  charLaberinto[0]='L';
  
  //String parsedlaberinto;
  //String parsedlaberinto[122];
  int count=1;
  
  for( int i=9; i>=1; i--){
    for( int t=1;t<10;t++){
      //parsedlaberinto=parsedlaberinto+String(Laberinto[i][t]);
      if (Laberinto[i][t]==0){
        charLaberinto[count]='0';
      }else if (Laberinto[i][t]==1){
        charLaberinto[count]='1';
      }else if (Laberinto[i][t]==2){
        charLaberinto[count]='2';
      }else if (Laberinto[i][t]==3){
        charLaberinto[count]='3';
      }else{
        charLaberinto[count]='9';
        //Serial.println("9 detectado");
      }
      
      //laberinto[count]=char(Laberinto[i][t]);
      count++;
    }  
  }
  //parsedlaberinto[123]='\0';
  //charLaberinto[82]='7';
  charLaberinto[83]='\0';
  
  
  Serial.print("\nData Parsed: ");
  Serial.println(charLaberinto);
  //Starting TX protocol
  //dataTX[0]='L'; //Data Type Control
  //String strlabX = String(labXpos);
  //dataTX[1]=strlabX[0];

  
  //String dataToSend = 'L' + String(labXpos) + String(labYpos) + labPath + String(wallsHit) + labSolution;
  //String dataToSend = 'L' + String(parsedlaberinto);
  
  //char dataTX[124];
  //dataToSend.toCharArray(dataTX,sizeof(dataTX),0);
 
  
  //Serial.print("Data Sent: ");
  //Serial.println(dataTX);
  
  Udpread.beginPacket(IPSend, sendPortLaberinto);
  //Udpread.beginPacket(Udpread.remoteIP(), Udpread.remotePort()); //Android Jordi UNI
  //Udpread.write(dataTX,sizeof(dataTX));
  //char test[] = "Hola Jose!";
  //Udpread.write("Hola Jose!");
  Udpread.write(charLaberinto,sizeof(charLaberinto));
  Udpread.endPacket();   
}

void sendAccelUDP(){
  //Data to send: X,Y,Z
  char dataTX[20];
  
  //Starting TX protocol
  dataTX[0]='A'; //Data Type Control
  
  String strX=String(x);
  
  String strY = String(y);
  String strZ = String(z);
  
  dataTX[1]=strX[0];
  dataTX[2]=strX[1];
  dataTX[3]=strX[2];
  dataTX[4]=strX[3];
  dataTX[5]=strX[4];
  dataTX[6]=' ';
  dataTX[7]=strY[0];
  dataTX[8]=strY[1];
  dataTX[9]=strY[2];
  dataTX[10]=strY[3];
  dataTX[11]=strY[4];
  dataTX[12]=' ';
  dataTX[13]=strZ[0];
  dataTX[14]=strZ[1];
  dataTX[15]=strZ[2];
  dataTX[16]=strZ[3];
  dataTX[17]=strZ[3];
  

  dataTX[18]='\0';//End
  
  Serial.print("Data Sent: ");
  Serial.println(dataTX);
  Serial.print("x: ");
  Serial.print(accel.cx, 3);
  Serial.print("\t");
  Serial.print("y: ");
  Serial.print(accel.cy, 3);
  Serial.print("\t");
  Serial.print("z: ");
  Serial.print(accel.cz, 3);
  Serial.println("\t");
  
  Udpread.beginPacket(IPSend, sendPort);
  //Udpread.beginPacket(Udpread.remoteIP(), Udpread.remotePort()); //Android Jordi UNI
  Udpread.write(dataTX,sizeof(dataTX));
  Udpread.endPacket();
   
}


void EscribePuerto(char *Trama){  // enviamos una trama char al puerto para enviarlo al destino
  //Udpread.beginPacket(Udpread.remoteIP(), Udpread.remotePort());
 // Udpread.beginPacket("192.168.1.34", 4560); //Android Jordi Casa
  //Udpread.beginPacket("170.20.10.12", 55056); //Android Jordi UNI
  Serial.print("To ");
   //IPAddress remoteIp = Udpread.remoteIP();
   IPAddress remoteIp = Udpread.remoteIP();
   Serial.print(remoteIp);
   Serial.print(", port ");
   Serial.println(Udpread.remotePort());
  //Udpread.beginPacket(remoteIp, 55056);
   //Udpread.beginPacket("170.20.10.12", 55056);
  Udpread.beginPacket(Udpread.remoteIP(), Udpread.remotePort());
  Udpread.write(Trama);
  Udpread.endPacket();  
  Serial.print("Data Sent: ");
  Serial.println(Trama);
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

