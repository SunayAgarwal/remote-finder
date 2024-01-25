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
#include "Adafruit_GFX.h"     // Core graphics library
#include "Adafruit_ST7789.h"  // Hardware-specific library for ST7789
#define TFT_CS A3
#define TFT_RST A5
#define TFT_DC A4

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Rotary encoder defs
#define VCC 3
#define GND 2
#define outputA 6
#define outputB 5
#define buttonIn 4
short aState;
short aLastState;
bool button;
bool buttonLastState;
short up;
short down;
short increment = 2;
uint16_t color[8] = { ST77XX_RED, ST77XX_ORANGE, ST77XX_YELLOW, ST77XX_GREEN, ST77XX_BLUE, ST77XX_MAGENTA, ST77XX_WHITE, ST77XX_BLACK };  // Array of colors for easy access

class Base;

Base* currentMenu;
Base* settings;
Base* devices;
Base* mainMenu;
Base* credits;

class Base {
  public:
    short cursorIndex = 0;
    String title;
    String menuItems[4];
    bool light = false;

    void highlightOption() {
      tft.setTextSize(3);
      tft.setTextColor(color[7], color[6]);
      tft.setCursor(5, 50 + (35 * cursorIndex));
      tft.print(menuItems[cursorIndex]);
    }

    void unHighlightOption() {
      tft.setTextSize(3);
      tft.setTextColor(color[6], color[7]);
      tft.setCursor(5, 50 + (35 * cursorIndex));
      tft.print(menuItems[cursorIndex]);
    }

    void printMenu() {
      currentMenu = this;
      tft.fillScreen(color[7]);
      tft.setTextSize(3);
      tft.setTextColor(color[6]);
      tft.setCursor(160 - (title.length() * 9), 5);
      tft.print(title);
      tft.drawLine(0, 35, 320, 35, color[6]);
      for (short i = 0; i < 4; ++i) {
        tft.setCursor(5, 50 + (35 * i));
        tft.print(menuItems[i]);
      }
      highlightOption();
    }

    void cursorDown() {
      unHighlightOption();
      ++cursorIndex;
      if (cursorIndex > 3) {
        --cursorIndex;
      }
      highlightOption();
    }

    void cursorUp() {
      unHighlightOption();
      --cursorIndex;
      if (cursorIndex < 0) {
        ++cursorIndex;
      }
      highlightOption();
    }

    void selectOption() {
      unHighlightOption();
      highlightOption();
      // this is the best way to do this because idk
      if (menuItems[cursorIndex] == "Devices") {
        devices->printMenu();
      } else if (menuItems[cursorIndex] == "Settings") {
        settings->printMenu();
      } else if (menuItems[cursorIndex] == "<< Back") {
        if (title == "Devices" || title == "Settings" || title == "Credits") {
          mainMenu->printMenu();
        }
      } else if (menuItems[cursorIndex] == "Light Mode" || menuItems[cursorIndex] == "Dark Mode") {
        if (light == false) { 
          color[6] = ST77XX_BLACK;
          color[7] = ST77XX_WHITE;
          light = true;
          menuItems[2] = "Dark Mode";
        } else if (light == true) {
          color[7] = ST77XX_BLACK;
          color[6] = ST77XX_WHITE;
          light = false;
          menuItems[2] = "Light Mode";
        }
        printMenu();
      } else if (menuItems[cursorIndex] == "Credits") {
        credits->printMenu();
      }
    }

    Base(String tt, String option1, String option2, String option3, String option4) {
      title = tt;
      menuItems[0] = option1;
      menuItems[1] = option2;
      menuItems[2] = option3;
      menuItems[3] = option4;
    }

    Base(bool idk) {
      devices = new Base("Devices", "<< Back", "one", "two", "three");
      settings = new Base("Settings", "<< Back", "Add a Device", "Light Mode", "Speaker Toggle");
      mainMenu = new Base("Main Menu", "Devices", "Settings", "Roll Call", "Credits");
      credits = new Base("Credits", "<< Back", "Emmett L.M.", "Joshua Curtis", "Sunay Agarwal");
      currentMenu = mainMenu;
    }
};

Base base = Base(true);

void setup(void) {
  pinMode(VCC, OUTPUT);
  pinMode(GND, OUTPUT);
  digitalWrite(VCC, HIGH);
  digitalWrite(GND, LOW);
  pinMode(buttonIn, INPUT_PULLUP);
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  aLastState = digitalRead(outputA);

  tft.init(240, 320);
  tft.setTextWrap(true);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  currentMenu->printMenu();
}
void loop() {
  aState = digitalRead(outputA);
  button = digitalRead(buttonIn);
  if (aState != aLastState) {
    if (digitalRead(outputB) != aState) {
      if (down == increment) {
        currentMenu->cursorDown();
        down = 1;
      } else {
        down = ++down;
        up = 1;
      }
    } else {
      if (up == increment) {
        currentMenu->cursorUp();
        up = 1;
      } else {
        up = ++up;
        down = 1;
      }
    }
  }
  if (button == false && buttonLastState == true) {
    currentMenu->selectOption();
  }
  aLastState = aState;
  buttonLastState = button;
}
