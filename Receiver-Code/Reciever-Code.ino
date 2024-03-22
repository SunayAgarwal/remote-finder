#include <WiFi.h>
#include <WiFiUdp.h>

#define BUZZER 3

const char* ssid = "ESP32AP1";
const char* password = "123456789";
const int UDPport = 1234;

bool buzzerState = LOW;

WiFiUDP UDP;
char packet[255];
String replyOn = "Turning On";
uint8_t replyOnInt = replyOn.toInt();
String replyOff = "Turning Off";
uint8_t replyOffInt = replyOff.toInt();
IPAddress host(192,168,4, 1);

/*
String charString() {
uint8_t mac[6];
WiFi.macAddress(mac);
String macAddress = "";
for (int i = 0; i < 6; i++) {
  macAddress += String(mac[i], HEX);
  if (i < 5) {
    macAddress += ":";
  }
}
return macAddress;
}
*/

uint8_t macAddressInt = WiFi.macAddress().toInt();


String convertToString(char* packet){
String packetString(packet);
return packetString;
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi");
  Serial.println("MAC Address: " + WiFi.macAddress());
  UDP.begin(UDPport);
  // Send MAC address to server
  sendMACAddress();

  int packetSize = UDP.parsePacket();
  while (packetSize == 0) {
    int packetSize = UDP.parsePacket();
    delay(10);                            //if goes through 20 iterations with no response, send again
  }
  if (packetSize){
    int len = UDP.read(packet, 255);
    if (len > 0) {
      packet[len] = '\0';
    }
    Serial.println(packet);
    if (packet != "Packet received") {      //send again, use millis i.e. 500 millis since sent
      Serial.println("Stuck");
      while (true){
        delay(10000);
      }
    }
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    WiFi.reconnect();
    UDP.begin(UDPport);
    sendMACAddress();
    delay(100);
  }
  
  int packetSize = UDP.parsePacket();
  if (packetSize) {
    Serial.print("Received packet! Size: ");
    Serial.println(packetSize); 
    int len = UDP.read(packet, 255);
    if (len > 0) {
      packet[len] = '\0';
    }
    Serial.print("Packet received: ");
    Serial.println(packet);
    String packetString = convertToString(packet);
    if (bool macCompare = packetString.compareTo(WiFi.macAddress()) && buzzerState == LOW) {
      buzzerState = HIGH;
      Serial.println(buzzerState);
      UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
      UDP.write(replyOnInt);
      UDP.endPacket();
    }
    else if (bool macCompare = packetString.compareTo(WiFi.macAddress())) {
      buzzerState = LOW;
      Serial.println(buzzerState);
      UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
      UDP.write(replyOffInt);
      UDP.endPacket();
    }
  }
    digitalWrite(BUZZER, buzzerState);
}

void sendMACAddress() {
    /*
    uint8_t mac[6];
    WiFi.macAddress(mac);
    String macAddress = "";
    for (int i = 0; i < 6; i++) {
      macAddress += String(mac[i], HEX);
      if (i < 5) {
        macAddress += ":";
  }
    }
    */
    unsigned char* macAddress[6];
    strcpy((char*) macAddress, WiFi.macAddress());
    //Serial.println(macAddress);
    UDP.beginPacket(host, UDPport);
    UDP.write(reinterpret_cast<const char*>(macAddress), 6);
    UDP.endPacket();
}
