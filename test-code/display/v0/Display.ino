/*
   Arduino --- TFT
   A3  ---  CS
   A4  ---  DC
   A5  ---  RST
   11  ---  SDA
   13  ---  SCL
   3V  ---  VCC
   GND ---  GND
*/
#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ST7789.h" // Hardware-specific library for ST7789
#define TFT_CS        A3
#define TFT_RST       A5
#define TFT_DC        A4
#define VCC 3
#define GND 2
#define outputA 6
#define outputB 5
#define buttonIn 4
int aState;
int aLastState;
int degValue = 0;
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
void setup(void) {
pinMode(VCC, OUTPUT);
pinMode(GND, OUTPUT);
digitalWrite(VCC, HIGH);
digitalWrite(GND, LOW);
pinMode(buttonIn,INPUT_PULLUP);
  pinMode(outputA,INPUT);
  pinMode(outputB,INPUT);
  aLastState = digitalRead(outputA);

  tft.init(240, 320);
  tft.setTextWrap(true);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(5,5);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(3);
  tft.println("Y    o    ");
  tft.setTextColor(ST77XX_ORANGE);
  tft.println(" o    t   ");
  tft.setTextColor(ST77XX_YELLOW);
  tft.println("  u    h  ");
  tft.setTextColor(ST77XX_GREEN);
  tft.println("   r    e ");
  tft.setTextColor(ST77XX_BLUE);
  tft.println("    m    r");
}
void loop() {
aState = digitalRead(outputA);
  if (aState != aLastState) {
    if (digitalRead(outputB) != aState) {
      degValue++;
    } else {
      degValue--;
    }

    if (degValue = 1){
      
    }
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(5,5);
  tft.println(degValue);
  }
}
void testlines(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x = 0; x < tft.width(); x += 6) {
    tft.drawLine(0, 0, x, tft.height() - 1, color);
    delay(0);
  }
  for (int16_t y = 0; y < tft.height(); y += 6) {
    tft.drawLine(0, 0, tft.width() - 1, y, color);
    delay(0);
  }
}
void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(5, 30);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setCursor(5, 60);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setCursor(5, 110);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(4);
}
void rotateText() {
  for (uint8_t i=0; i<4; i++) {
    tft.fillScreen(ST77XX_BLACK);
    Serial.println(tft.getRotation(), DEC);

    tft.setCursor(0, 30);
    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(1);
    tft.println("Hello World!");
    tft.setTextColor(ST77XX_YELLOW);
    tft.setTextSize(2);
    tft.println("Hello World!");
    tft.setTextColor(ST77XX_GREEN);
    tft.setTextSize(3);
    tft.println("Hello World!");
    tft.setTextColor(ST77XX_BLUE);
    tft.setTextSize(4);
    tft.print(1234.567);

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    tft.setRotation(tft.getRotation()+1);
  }
}