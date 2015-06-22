#include <SPI.h> //libreria para interconectar la placa Mega y la shield WIFI.
#include <WiFi.h> //librería para controla la transmisión WIFI.
#include <WiFiUdp.h> //libreria para controlar el tráfico UDP

int status = WL_IDLE_STATUS;           // Estado de la señal radio del WIFI.
WiFiUDP Udpread;                       // Tipo para guardar la trama UDP leida.
unsigned int readPort = 55056;         // puerto local para leer datos.
char packetBuffer[255];                // Buffer para leer el paquete UDP.
const char IPSend[] = "172.20.10.13";  // IP del movil.
const int sendPort = 4560 ;            // puerto remoto para enviar datos al móvil.
const int sendPortLaberinto = 4561 ;   // puerto laberinto para enviar datos al móvil.
const int sendPortAccel=4562;          // puerto acelerometro para enviar datos al móvil.

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
    IPAddress remoteIp = Udpread.remoteIP(); // Lee la IP remota que envia la trama.
    IPRx=remoteIp;    
    PortRx=Udpread.remotePort();    
    
    int len = Udpread.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = '\0';      
    }
    return packetBuffer;
  }else{    
    return "E"; //No hay datos en cola.
  }  
}

void sendControlUDP(int temp,int luces,int colision,char manualAuto,int speedValue){
//Funcion para enviar la trama de control al movil.
// Entradas: temp(int). Temperatura.  
//           luces(int). Luces.
//           colision(int). Si hay colision.
//           manualAuto. Si hay colision o no.
//           SpeedValue. Velocidad del robot.
  
  char dataTX[10];  
  char proximity = 'N';   //char proximity = getProximityStatus();
  
  dataTX[0]='C'; // Tipo control
  String str = String(temp);
  dataTX[1]=str[0];
  dataTX[2]=str[1];  
  dataTX[3]=luces;
  str = String(colision);
  dataTX[4]=str[0];  
  dataTX[5]=manualAuto;
  str = String(speedValue);
  dataTX[6]=str[0];
  dataTX[7]=proximity;
  dataTX[8]='\0';// Fin de trama
  
  Udpread.beginPacket(IPSend, sendPort); 
  Udpread.write(dataTX,sizeof(dataTX)); 
  Udpread.endPacket(); 
}

void sendLaberynthUDP(char Tipo){
// Funcion para enviar la trama del reto laberinto.
//  entradas: Tipo(char). N: esta buscando la salida. S: ya encontro la salida o esta ejecutando la solucion.
  char charLaberinto[84];
  charLaberinto[0]='L';
  charLaberinto[1]=Tipo;
  
  int count=2;
  
  for( int i=9; i>=1; i--){
    for( int t=1;t<10;t++){
      if (LaberintoTotal[i][t]==0){
        charLaberinto[count]='0';
      }else if (LaberintoTotal[i][t]==1){
        charLaberinto[count]='1';
      }else if (LaberintoTotal[i][t]==2){
        charLaberinto[count]='2';
      }else if (LaberintoTotal[i][t]==3){
        charLaberinto[count]='3';
      }else{
        charLaberinto[count]='9';
      }   
      count++;
    }  
  }  
  charLaberinto[84]='\0';
  Udpread.beginPacket(IPSend, sendPortLaberinto);
  Udpread.write(charLaberinto,sizeof(charLaberinto));
  Udpread.endPacket();   
}

void sendAccelUDP(){
//Funcion para enviar trama del modo acelerometro

 char dataTX[22];
  
  //Starting TX protocol
  dataTX[0]='A'; //Data Type Control
  
  String strX=String(x);
  char signoX;
  String strY = String(y);
  char signoY;
  String strZ = String(z);
  char signoZ;
  
  if(x>0){
  signoX='0';
  }else{
  signoX='1';
  }
  if(y>0){
  signoY='0';
  }else{
  signoY='1';
  }
  if(z>0){
  signoZ='0';
  }else{
  signoZ='1';
  }
  dataTX[1]=signoX;
  dataTX[2]=strX[0];
  dataTX[3]=strX[1];
  dataTX[4]=strX[2];
  dataTX[5]=strX[3];
  dataTX[6]=strX[4];
  dataTX[7]=' ';
  dataTX[8]=signoY;
  dataTX[9]=strY[0];
  dataTX[10]=strY[1];
  dataTX[11]=strY[2];
  dataTX[12]=strY[3];
  dataTX[13]=strY[4];
  dataTX[14]=' ';
  dataTX[15]=signoZ;
  dataTX[16]=strZ[0];
  dataTX[17]=strZ[1];
  dataTX[18]=strZ[2];
  dataTX[19]=strZ[3];
  dataTX[20]=strZ[3];
  dataTX[21]='\0';
  Udpread.beginPacket(IPSend, sendPortAccel);  
  Udpread.write(dataTX,sizeof(dataTX));
  Udpread.endPacket(); 
}

void EscribePuerto(char *Trama){  
// Funcion para ennviar una trama char al puerto para enviarlo al destino
//  entrada y salida: Trama (*char). Trama que contiene los datos a enviar.

  
  IPAddress remoteIp = Udpread.remoteIP();   
  Udpread.beginPacket(Udpread.remoteIP(), Udpread.remotePort());
  Udpread.write(Trama);
  Udpread.endPacket();    
}

void printWifiData() {
// funcion para visualizar la Ip y MAC wifi
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

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
// funcion para visualizar los datos de la red Wifi.
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

