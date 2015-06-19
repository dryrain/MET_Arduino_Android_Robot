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
  //Udpread.beginPacket(IPRx, PortRx); //Android Jordi UNI
  //Udpread.beginPacket(Udpread.remoteIP(), Udpread.remotePort());
  Udpread.write(dataTX,sizeof(dataTX));
 // Udpread.write("Hola");
  Udpread.endPacket(); 
  
  
  
}

void sendLaberynthUDP(){
  //Data to send: position,path,wallsFound,solution
  
  int fil1;
  
 
    for( int t=0;t<11;t++){
      fil1=(Laberinto[10][t]);
     
      
    } 
   
    int fil2;
  
 
    for( int t=0;t<11;t++){
      fil2=Laberinto[9][t];
     
      
    } 
    int fil3;
  
 
    for( int t=0;t<11;t++){
      fil3=Laberinto[8][t];
     
      
    } 
    int fil4;
  
 
    for( int t=0;t<11;t++){
      fil1=Laberinto[7][t];
     
      
    } 
    int fil5;
  
 
    for( int t=0;t<11;t++){
      fil1=Laberinto[6][t];
     
      
    }  
  int fil6;
  
 
    for( int t=0;t<11;t++){
      fil1=Laberinto[5][t];
     
      
    } 
   int fil7;
  
 
    for( int t=0;t<11;t++){
      fil2=Laberinto[4][t];
     
      
    } 
    int fil8;
  
 
    for( int t=0;t<11;t++){
      fil3=Laberinto[3][t];
     
      
    } 
    int fil9;
  
 
    for( int t=0;t<11;t++){
      fil1=Laberinto[2][t];
     
      
    } 
    int fil10;
  
 
    for( int t=0;t<11;t++){
      fil1=Laberinto[1][t];
     
      
    }  
  int fil11;
  
 
    for( int t=0;t<11;t++){
      fil1=Laberinto[0][t];
     
      
    } 
  
  
  
  
  String str1=String(fil1);
  String str2=String(fil2);
  String str3=String(fil3);
  String str4=String(fil4);
  String str5=String(fil5);
  String str6=String(fil6);
  String str7=String(fil7);
  String str8=String(fil8);
  String str9=String(fil9);
  String str10=String(fil10);
  String str11=String(fil11);
  
  
 char dataTX[123];
 
  dataTX[0]='L';
  dataTX[1]=str1[0];
  dataTX[2]=str1[1];
  dataTX[3]=str1[2];
  dataTX[4]=str1[3];
  dataTX[5]=str1[4];
  dataTX[6]=str1[5];
  dataTX[7]=str1[6];
  dataTX[8]=str1[7];
  dataTX[9]=str1[8];
  dataTX[10]=str1[9];
  dataTX[11]=str1[10];
 
 
 
  /*
  dataTX[0]='L';
  dataTX[1]=String(Laberinto[0][10]);
  dataTX[2]=String(Laberinto[1][10]);
  dataTX[3]=String(Laberinto[2][10]);
  dataTX[4]=String(Laberinto[3][10]);
  dataTX[5]=String(Laberinto[4][10]);
  dataTX[6]=String(Laberinto[5][10]);
  dataTX[7]=String(Laberinto[6][10]);
  dataTX[8]=String(Laberinto[7][10]);
  dataTX[9]=String(Laberinto[8][10]);
  dataTX[10]=String(Laberinto[9][10]);
  dataTX[11]=String(Laberinto[10][10]);
  /*
  dataTX[12]=String(Laberinto[0][9]);
  dataTX[13]=String(Laberinto[1][9]);
  dataTX[14]=String(Laberinto[2][9]);
  dataTX[15]=String(Laberinto[3][9]);
  dataTX[16]=String(Laberinto[4][9]);
  dataTX[17]=String(Laberinto[5][9]);
  dataTX[18]=String(Laberinto[6][9]);
  dataTX[19]=String(Laberinto[7][9]);
  dataTX[20]=String(Laberinto[8][9]);
  dataTX[21]=String(Laberinto[9][9]);
  dataTX[22]=String(Laberinto[10][9]);
  
   dataTX[23]=String(Laberinto[0][8]);
  dataTX[24]=String(Laberinto[1][8]);
  dataTX[25]=String(Laberinto[2][8]);
  dataTX[26]=String(Laberinto[3][8]);
  dataTX[27]=String(Laberinto[4][8]);
  dataTX[28]=String(Laberinto[5][8]);
  dataTX[29]=String(Laberinto[6][8]);
  dataTX[30]=String(Laberinto[7][8]);
  dataTX[31]=String(Laberinto[8][8]);
  dataTX[32]=String(Laberinto[9][8]);
  dataTX[33]=String(Laberinto[10][8]);
  
  dataTX[34]=String(Laberinto[0][7]);
  dataTX[35]=String(Laberinto[1][7]);
  dataTX[36]=String(Laberinto[2][7]);
  dataTX[37]=String(Laberinto[3][7]);
  dataTX[38]=String(Laberinto[4][7]);
  dataTX[39]=String(Laberinto[5][7]);
  dataTX[40]=String(Laberinto[6][7]);
  dataTX[41]=String(Laberinto[7][7]);
  dataTX[42]=String(Laberinto[8][7]);
  dataTX[43]=String(Laberinto[9][7]);
  dataTX[44]=String(Laberinto[10][7]);
  
  dataTX[45]=String(Laberinto[0][6]);
  dataTX[46]=String(Laberinto[1][6]);
  dataTX[47]=String(Laberinto[2][6]);
  dataTX[48]=String(Laberinto[3][6]);
  dataTX[49]=String(Laberinto[4][6]);
  dataTX[50]=String(Laberinto[5][6]);
  dataTX[51]=String(Laberinto[6][6]);
  dataTX[52]=String(Laberinto[7][6]);
  dataTX[53]=String(Laberinto[8][6]);
  dataTX[54]=String(Laberinto[9][6]);
  dataTX[55]=String(Laberinto[10][6]);
  
  dataTX[56]=String(Laberinto[0][5]);
  dataTX[57]=String(Laberinto[1][5]);
  dataTX[58]=String(Laberinto[2][5]);
  dataTX[59]=String(Laberinto[3][5]);
  dataTX[60]=String(Laberinto[4][5]);
  dataTX[61]=String(Laberinto[5][5]);
  dataTX[62]=String(Laberinto[6][5]);
  dataTX[63]=String(Laberinto[7][5]);
  dataTX[64]=String(Laberinto[8][5]);
  dataTX[65]=String(Laberinto[9][5]);
  dataTX[66]=String(Laberinto[10][5]);
  
  dataTX[67]=String(Laberinto[0][4]);
  dataTX[68]=String(Laberinto[1][4]);
  dataTX[69]=String(Laberinto[2][4]);
  dataTX[70]=String(Laberinto[3][4]);
  dataTX[71]=String(Laberinto[4][4]);
  dataTX[72]=String(Laberinto[5][4]);
  dataTX[73]=String(Laberinto[6][4]);
  dataTX[74]=String(Laberinto[7][4]);
  dataTX[75]=String(Laberinto[8][4]);
  dataTX[76]=String(Laberinto[9][4]);
  dataTX[77]=String(Laberinto[10][4]);
  
  dataTX[78]=String(Laberinto[0][3]);
  dataTX[79]=String(Laberinto[1][3]);
  dataTX[80]=String(Laberinto[2][3]);
  dataTX[81]=String(Laberinto[3][3]);
  dataTX[82]=String(Laberinto[4][3]);
  dataTX[83]=String(Laberinto[5][3]);
  dataTX[84]=String(Laberinto[6][3]);
  dataTX[85]=String(Laberinto[7][3]);
  dataTX[86]=String(Laberinto[8][3]);
  dataTX[87]=String(Laberinto[9][3]);
  dataTX[88]=String(Laberinto[10][3]);
  
  dataTX[89]=String(Laberinto[0][2]);
  dataTX[90]=String(Laberinto[1][2]);
  dataTX[91]=String(Laberinto[2][2]);
  dataTX[92]=String(Laberinto[3][2]);
  dataTX[93]=String(Laberinto[4][2]);
  dataTX[94]=String(Laberinto[5][2]);
  dataTX[95]=String(Laberinto[6][2]);
  dataTX[96]=String(Laberinto[7][2]);
  dataTX[97]=String(Laberinto[8][2]);
  dataTX[98]=String(Laberinto[9][2]);
  dataTX[99]=String(Laberinto[10][2]);
  
  dataTX[100]=String(Laberinto[0][1]);
  dataTX[101]=String(Laberinto[1][1]);
  dataTX[102]=String(Laberinto[2][1]);
  dataTX[103]=String(Laberinto[3][1]);
  dataTX[104]=String(Laberinto[4][1]);
  dataTX[105]=String(Laberinto[5][1]);
  dataTX[106]=String(Laberinto[6][1]);
  dataTX[107]=String(Laberinto[7][1]);
  dataTX[108]=String(Laberinto[8][1]);
  dataTX[109]=String(Laberinto[9][1]);
  dataTX[110]=String(Laberinto[10][1]);
  
  dataTX[111]=String(Laberinto[0][0]);
  dataTX[112]=String(Laberinto[1][0]);
  dataTX[113]=String(Laberinto[2][0]);
  dataTX[114]=String(Laberinto[3][0]);
  dataTX[115]=String(Laberinto[4][0]);
  dataTX[116]=String(Laberinto[5][0]);
  dataTX[117]=String(Laberinto[6][0]);
  dataTX[118]=String(Laberinto[7][0]);
  dataTX[119]=String(Laberinto[8][0]);
  dataTX[120]=String(Laberinto[9][0]);
  dataTX[121]=String(Laberinto[10][0]);
  */
  
  Serial.print("Data Sent: ");
  Serial.println(dataTX);
  
  Udpread.beginPacket(IPSend, sendPort);
  //Udpread.beginPacket(Udpread.remoteIP(), Udpread.remotePort()); //Android Jordi UNI
  Udpread.write(dataTX,sizeof(dataTX));
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

