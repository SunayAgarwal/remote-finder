/*
  Bluetooth controlled car (that's the eventual goal here)

  My code is shared under the MIT license.
    In a nutshell, use it for anything but you take full responsibility.

Starting with the built-in ArduinoBLE example "Peripheral-ButtonBLE"
Also see:
https://docs.arduino.cc/tutorials/nano-33-ble/bluetooth

  This example creates a Bluetooth® Low Energy peripheral with service that contains a
  characteristic to control an LED

  You can use a generic Bluetooth® Low Energy central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  See: https://www.arduino.cc/reference/en/libraries/arduinoble/

  Random UUID Generator: https://www.uuidgenerator.net/version4
  example: ea943a1a-2206-4235-970f-ad8127fff9bb

  Characteristics can have a random/custom UUID, or they can use a pre-defined value from the BlueTooth Assigned Numbers list:
  https://btprodspecificationrefs.blob.core.windows.net/assigned-numbers/Assigned%20Number%20Types/Assigned_Numbers.pdf

Examples:
// Bluetooth® Low Energy Battery Level Characteristic
BLEUnsignedCharCharacteristic batteryLevelChar("2A19",  // standard 16-bit characteristic UUID (see assigned numbers document)
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes

According to the BLE Assigned Numbers document:
  joystick is 0x03C3
  boolean is 0x2AE2

*/

#include <ArduinoBLE.h>

BLEService carService("180A"); // create service: "Device Information"

// create direction control characteristic and allow remote device to read and write
BLEByteCharacteristic carControlCharacteristic("2A57", BLEWrite | BLERead); // 2A57 is "Digital Output"

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT); // use the LED as an output

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1) { // blink the built-in LED fast to indicate an issue
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
  }

  BLE.setLocalName("Reciever");
  BLE.setAdvertisedService(carService);

  // add the characteristics to the service
  carService.addCharacteristic(carControlCharacteristic);

  // add the service
  BLE.addService(carService);

  carControlCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth® device active, waiting for connections...");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice controller = BLE.central();

  if (analogRead(A0) < 1023) {
    Serial.println("Buzz");
    delay(3000);
  }

  // if a central is connected to peripheral:
  if (controller) {
    Serial.print("Connected to controller: ");
    // print the controller's MAC address:
    Serial.println(controller.address());
    digitalWrite(LED_BUILTIN, HIGH);  // turn on the LED to indicate the connection

    // while the controller is still connected to peripheral:
    while (controller.connected()) {

      if (carControlCharacteristic.written()) {

        switch (carControlCharacteristic.value()) {
          case 01:
            Serial.println("LEFT");
            digitalWrite (LED_BUILTIN, HIGH);
            delay(1000);
            digitalWrite (LED_BUILTIN, LOW);
            break;
          case 02:
            Serial.println("RIGHT");
            break;
          case 03:
            Serial.println("UP");
            break;
          case 04:
            Serial.println("DOWN");
            break;
          default:  // 0 or invalid control
            Serial.println("STOP");
            break;
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from controller: "));
    Serial.println(controller.address());
    digitalWrite(LED_BUILTIN, LOW);         // when the central disconnects, turn off the LED

  }
}