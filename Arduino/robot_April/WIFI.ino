int status = WL_IDLE_STATUS;     // the Wifi radio's status
WiFiUDP Udpread;

//unsigned int readPort = 2390;      // puerto local para leer datos
unsigned int readPort = 55056; 
char packetBuffer[255]; //buffer to hold incoming packet

void ini_WIFI_WPA(char *c_ssid, char *c_pass){  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield no esta conectada");
    // don't continue:
    while (true);
  }
  while ( status != WL_CONNECTED) {
    Serial.print("Intentando conectar con la WPA SSID: ");
    Serial.println(c_ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(c_ssid, c_pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  Udpread.begin(readPort); //inicializamos la lectura de tramas UDP en el puerto
  printCurrentNet(); //Visualizamos la SSID, BSSID...
  printWifiData(); // visualizamos los datos de la shield wifi (IP,MAC)
}

struct Tramas LeeTrama(void) //Leemos el puerto y retornamos una estructura con los datos del paquete recibido
{  
  char *recibidos;
  struct Tramas Paquete1;
  recibidos=EscuchaPuerto(); //Leemos la trama recibida en formato char.
  if(recibidos[0]!='\0'){ 
     Paquete1.numero=recibidos[0];
     Paquete1.cabecera=recibidos[1];
     for(byte i=0;i<8;i++){
       Paquete1.datos[i]=recibidos[i+2];
     }
     Paquete1.activa=true;        
  }else{Paquete1.activa=false;}  
  
  //Serial.print("Cabecera:");
  //Serial.println(Paquete1.cabecera);
  //Serial.print("Datos:");
  //Serial.println(Paquete1.datos[1]);
  return Paquete1;
}

char* readWifiProtocol(void)
{
  char *recibidos;
  recibidos=EscuchaPuerto();
    
    //Starting the reading protocol
    //----
    
  return recibidos;
}


char *EscuchaPuerto(){  // Escuchamos el puerto y devolvemos toda la trama en formato char.
  // if there's data available, read a packet
  int packetSize = Udpread.parsePacket();
  if (packetSize)
  {
    // read the packet into packetBufffer
    int len = Udpread.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = '\0'; //Put a line ending
    
    Serial.print("PacketBuffer: ");
    Serial.println(packetBuffer);
    
  }else{
    packetBuffer[0]='\0';
  }  
 return packetBuffer;
}

char *readUDP(void){  // Escuchamos el puerto y devolvemos toda la trama en formato char.
  // if there's data available, read a packet
  int packetSize = Udpread.parsePacket();
  if (packetSize)
  {
    Serial.print("From ");
    IPAddress remoteIp = Udpread.remoteIP();
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
    //Serial.print("PacketBuffer: ");
    //Serial.println(packetBuffer);
    return packetBuffer;
  }else{
    //packetBuffer[0]='\0';
    return "E"; //No data
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
  //Udpread.beginPacket(IPRx, PortRx); //Android Jordi UNI
  //Udpread.beginPacket(Udpread.remoteIP(), Udpread.remotePort());
  //Udpread.write(dataTX,sizeof(dataTX));
  Udpread.write("Hola");
  Udpread.endPacket(); 
  
  
  
}

void sendLaberynthUDP(){
  //Data to send: position,path,wallsFound,solution
  
  
  int labXpos = 3; //getLabXpos();
  int labYpos = 5; //getLabYpos();
  String labPath = "0010100101001010010100101"; //getLabPath();
  int wallsHit = 3; //getWallsHit();
  String labSolution = "0010100101111010010100101"; //getLabSolution();
  
  //Starting TX protocol
  //dataTX[0]='L'; //Data Type Control
  String strlabX = String(labXpos);
  //dataTX[1]=strlabX[0];

  
  String dataToSend = 'L' + String(labXpos) + String(labYpos) + labPath + String(wallsHit) + labSolution;
  
  char dataTX[54];
  dataToSend.toCharArray(dataTX,sizeof(dataTX),0);
 
  
  Serial.print("Data Sent: ");
  Serial.println(dataTX);
  
  Udpread.beginPacket(IPSend, sendPort);
  //Udpread.beginPacket(Udpread.remoteIP(), Udpread.remotePort()); //Android Jordi UNI
  Udpread.write(dataTX,sizeof(dataTX));
  Udpread.endPacket();   
}

void sendAccelUDP(){
  //Data to send: X,Y,Z
  char dataTX[11];
  
  int Xpos = 100;//getPositionX();
  int Ypos = 110;//getPositionY();
  int Zpos = 111;//getPositionZ();
  
  //Starting TX protocol
  dataTX[0]='A'; //Data Type Control
  
  String strX = String(Xpos);
  String strY = String(Ypos);
  String strZ = String(Zpos);
  dataTX[1]=strX[0];
  dataTX[2]=strX[1];
  dataTX[3]=strX[2];
  dataTX[4]=strY[0];
  dataTX[5]=strY[1];
  dataTX[6]=strY[2];
  dataTX[7]=strZ[0];
  dataTX[8]=strZ[1];
  dataTX[9]=strZ[2];

  dataTX[10]='\0';//End
  
  Serial.print("Data Sent: ");
  Serial.println(dataTX);
  
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

