#include <WiFi.h>

#define BUZZER 3

const char* ssid = "ESP32AP1";
const char* password = "123456789";
const char* serverIP = "192.168.4.1"; // IP address of the server

bool buzzerState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  Serial.println("Connected to WiFi");
  Serial.println("MAC Address: " + WiFi.macAddress());

  // Send MAC address to server
  sendMACAddress();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    WiFi.reconnect();
    delay(100);
  }
  while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        if (request == WiFi.macAddress() && buzzerState == LOW) {
          buzzerState = HIGH;
          digitalWrite(BUZZER, buzzerState);
          Serial.println(buzzerState);
        }
        if (request == WiFi.macAddress() && buzzerState == HIGH) {
          buzzerState = LOW;
          digitalWrite(BUZZER, buzzerState);
          Serial.println(buzzerState);
        }
    }
  }
}

void sendMACAddress() {
  WiFiClient client;

  if (client.connect(serverIP, 80)) {
    Serial.println("Connected to server");
    client.println(WiFi.macAddress());
    Serial.println("MAC address sent to server");
  } else {
    Serial.println("Connection to server failed");
  }
}
