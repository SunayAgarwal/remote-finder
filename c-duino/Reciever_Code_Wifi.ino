#include <WiFi.h>

const char *ssid = "Karen's";
const char *password = "desserts1";

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  // Check if a message is available
  if (Serial.available() > 0) {
    // Read the message
    char incomingChar = Serial.read();

    // Check if the message is "1"
    if (incomingChar == '1') {
      // Turn on the onboard LED
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("LED turned on");
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      // Turn off the onboard LED for any other message
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("LED turned off");
    }
  }
}
