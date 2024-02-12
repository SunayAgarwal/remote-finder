// Upload as Adafruit QT Py ESP32-C3
#include <WiFi.h>

const char *ssid = "network_name";
const char *password = "network_password";
#define BUZZER 4 // Buzzer on pin D2

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, HIGH);
}

void loop() {
  // Check if a message is available
  if (Serial.available() > 0) {
    // Read the message
    char incomingChar = Serial.read();

    // Check if the message is "1"
    if (incomingChar == '1') {
      // Turn on the onboard LED
      digitalWrite(BUZZER, HIGH);
      Serial.println("LED turned on");
      delay(1000);
      digitalWrite(BUZZER, LOW);
    } else {
      // Turn off the onboard LED for any other message
      digitalWrite(BUZZER, LOW);
      Serial.println("LED turned off");
    }
  }
}
