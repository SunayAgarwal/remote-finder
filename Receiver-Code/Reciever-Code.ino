//reconnecting

#include <WiFi.h>
#include <WiFiUdp.h>

#define BUZZER 2   //D1

const char* ssid = "ESP32AP1";
const char* password = "123456789";
const int UDPport = 1234;

bool buzzerState = HIGH;

WiFiUDP UDP;
uint8_t receivedMessage[] = {0x50, 0x61, 0x63, 0x6B, 0x65, 0x74, 0x72, 0x65, 0x63, 0x65, 0x69, 0x76, 0x65, 0x64};
uint8_t shutUp[] = {0x73, 0x68, 0x75, 0x74, 0x55, 0x70};
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
  pinMode(BUZZER, INPUT);
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

  uint8_t packet[255];
  unsigned long previousMillis = millis();
  int packetSize = UDP.parsePacket();
  while (packetSize == 0) {
    packetSize = UDP.parsePacket();                           //if goes through 20 iterations with no response, send again
    if (millis() - previousMillis > 10000) {
      sendMACAddress();
      previousMillis = millis();
    }
  }
  if (packetSize){
    Serial.print("Received packet! Size: ");
    Serial.println(packetSize);
    int len = UDP.read(packet, 255);
    for(int b=0; b<14; ++b) {
      Serial.print((char)packet[b]);      
    }

    Serial.println();
    String receivedMessage = "Packetreceived";
    char receivedMessageChar[14];
    receivedMessage.toCharArray(receivedMessageChar,15);
    for(int b=0; b<14; ++b) {
      Serial.print(receivedMessageChar[b]);      
    }
    if (memcmp(packet, receivedMessageChar, 14) != 0) {      //send again, use millis i.e. 500 millis since sent
      Serial.println("Stuck");
      while (true){
        delay(10000);
      }
    }
  }
}

void loop() {
  uint8_t packet[255];

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    UDP.begin(UDPport);
    sendMACAddress();
  unsigned long previousMillis = millis();
  int packetSize = UDP.parsePacket();
  while (packetSize == 0) {
    packetSize = UDP.parsePacket();                           //if goes through 20 iterations with no response, send again
    if (millis() - previousMillis > 10000) {
      sendMACAddress();
    }
  }
  if (packetSize){
    Serial.print("Received packet! Size: ");
    Serial.println(packetSize);
    int len = UDP.read(packet, 255);
    for(int b=0; b<14; ++b) {
      Serial.print((char)packet[b]);      
    }

    Serial.println();
    String receivedMessage = "Packetreceived";
    char receivedMessageChar[14];
    receivedMessage.toCharArray(receivedMessageChar,15);
    for(int b=0; b<14; ++b) {
      Serial.print(receivedMessageChar[b]);      
    }
    }
    delay(100);
  }
  
  int packetSize = UDP.parsePacket();
  if (packetSize) {
    Serial.print("Received packet! Size: ");
    Serial.println(packetSize); 
    int len = UDP.read(packet, 255);
    Serial.print("Packet received: ");
    for(int b=0; b<6; ++b) {
      Serial.print(packet[b], HEX);
      // Add ":" as needed
      if (b<5) Serial.print(":");
    }
    Serial.println();
    uint8_t macAddress[6];
    WiFi.macAddress(macAddress);
    if (memcmp(packet, macAddress, 6) == 0 && buzzerState == HIGH) {
      pinMode(BUZZER, OUTPUT);
      buzzerState = LOW;
      digitalWrite(BUZZER, buzzerState);
      Serial.println(buzzerState);
      UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
      UDP.write(receivedMessage, 14);
      UDP.endPacket();
      unsigned long previousMillis = millis();
      while (millis() - previousMillis < 10000){
        //check for shut up packet
        int packetSize = UDP.parsePacket();
  if (packetSize) {
    Serial.print("Received packet! Size: ");
    Serial.println(packetSize); 
    int len = UDP.read(packet, 255);
    Serial.print("Packet received: ");
    for(int b=0; b<6; ++b) {
      Serial.print(packet[b], HEX);
      // Add ":" as needed
      if (b<5) Serial.print(":");
    }
    Serial.println();
    if (memcmp(packet, shutUp, 6) == 0){
      pinMode(BUZZER, INPUT);
      buzzerState = HIGH;
    }
      }
      }     
      pinMode(BUZZER, INPUT);
      buzzerState = HIGH;
    }
    
  }
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
    uint8_t macAddress[6];
    WiFi.macAddress(macAddress);
    for(int b=0; b<6; ++b) {
      Serial.print(macAddress[b], HEX);
      // Add ":" as needed
      if (b<5) Serial.print(":");
    }
    Serial.println();
    UDP.beginPacket(host, UDPport);
    UDP.write(macAddress, 6);
    UDP.endPacket();
}
