/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-uno-r4-led-matrix-displays-number-character
 */

#include "Arduino_LED_Matrix.h"
#include "fonts.h"
ArduinoLEDMatrix matrix;

uint8_t frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

int degValue = 0;
int tens;
int ones;

#define outputA 10
#define outputB 11
#define buttonIn 12
bool button = false;
bool lastButton = false;
int aState;
int aLastState;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  pinMode(buttonIn,INPUT_PULLUP);
  pinMode(outputA,INPUT);
  pinMode(outputB,INPUT);
  Serial.begin(115200);
  matrix.begin();
  Serial.begin(9600);
  aLastState = digitalRead(outputA);
}

void loop() {
  button = digitalRead(buttonIn);
  if (button == false && lastButton == true) {
    degValue += 10;
    degValue = (degValue+100) % 100;
    ones = degValue % 10;
    tens = (degValue - ones) / 10;
    clear_frame();
    add_to_frame(tens, 0);
    add_to_frame(ones, 6);
    display_frame();
  }
  lastButton = button;
  aState = digitalRead(outputA);
  if (aState != aLastState) {
    if (digitalRead(outputB) != aState) {
      degValue++;
    } else {
      degValue--;
    }
    degValue = (degValue+100) % 100;
    ones = degValue % 10;
    tens = (degValue - ones) / 10;
    clear_frame();
    add_to_frame(tens, 0);
    add_to_frame(ones, 6);
    display_frame();
  }
  aLastState = aState;
}

void clear_frame() {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 12; col++) {
      frame[row][col] = 0;
    }
  }
}

void display_frame() {
  matrix.renderBitmap(frame, 8, 12);
}


void add_to_frame(int c, int pos) {
  int index = -1;
  if (c >= 0 && c <= 9)
    index = c - 0;
  else {
    Serial.println("WARNING: unsupported character");
    return;
  }

  for (int row = 0; row < 8; row++) {
    uint32_t temp = fonts[index][row] << (7 - pos);
    for (int col = 0; col < 12; col++) {
      frame[row][col] |= (temp >> (11 - col)) & 1;
    }
  }
}
