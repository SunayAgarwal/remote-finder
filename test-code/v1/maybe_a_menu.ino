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
int aState;
int aLastState;
short degValue = 0;
short counter = 0;
uint16_t color[8] = { ST77XX_RED, ST77XX_ORANGE, ST77XX_YELLOW, ST77XX_GREEN, ST77XX_BLUE, ST77XX_MAGENTA, ST77XX_WHITE, ST77XX_BLACK };  // Array of colors for easy access

class Menu {
private:

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
public:
  short cursorIndex = 0;
  String title;
  String menuItems[4];

  void printMenu() {
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

  Menu() {}

  Menu(String tt, String option1, String option2, String option3, String option4) {
    title = tt;
    menuItems[0] = option1;
    menuItems[1] = option2;
    menuItems[2] = option3;
    menuItems[3] = option4;
  }
};

class MainMenu : public Menu{
  public:
    MainMenu(String tt, Menu option1, Menu option2, String option3, String option4) {
      title = tt;
      menuItems[0] = option1.title;
      menuItems[1] = option2.title;
      menuItems[2] = option3;
      menuItems[3] = option4;
    }
};

// Declaring the menus here for now, later will independently store
Menu devices = Menu("Devices", "one", "two", "three", "four");
Menu settings = Menu("Settings", "<< Back", "Add a Device", "Light/Dark Mode", "Speaker Toggle");
MainMenu mainMenu = MainMenu("Main Menu", devices, settings, "Sleep", "About");

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
  mainMenu.printMenu();
}
void loop() {
  aState = digitalRead(outputA);
  if (aState != aLastState) {
    if (digitalRead(outputB) != aState) {
      mainMenu.cursorUp();
    } else {
      mainMenu.cursorDown();
    }
  }
  aLastState = aState;
}