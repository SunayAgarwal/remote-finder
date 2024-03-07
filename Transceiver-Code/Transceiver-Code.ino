/*
   Seeeduino Xiao ESP32C3--- TFT
   D1  ---  CS
   D0 ---  DC
   D3  ---  RST
   D4  ---  SDA
   D5  ---  SCL
   3V3  ---  VCC
   GND ---  GND
*/
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <WiFi.h>             // WiFi library for receiver interaction
#include <ToneESP32.h>        // Tone library for speaker use
#define TFT_CS 3
#define TFT_RST 5
#define TFT_DC 2
#define TFT_SDA 6
#define TFT_SCL 7
#define SPK 21

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_SDA, TFT_SCL, TFT_RST);

// Rotary encoder defs
#define outputA 9
#define outputB 8
#define buttonIn 20
short aState;
short aLastState;
bool button;
bool buttonLastState;
short up;
short down;
short increment = 2;
uint16_t color[8] = { ST77XX_RED, ST77XX_ORANGE, ST77XX_YELLOW, ST77XX_GREEN, ST77XX_BLUE, ST77XX_MAGENTA, ST77XX_WHITE, ST77XX_BLACK };  // Array of colors for easy access

short clients;
short prevClients;
WiFiServer server(80);
class Menu;

class Device {
  public:
    String address;
    bool connected = false;
    Menu* menu;
    WiFiClient client;

    void buzz() {
      client.println(address);
    }

    String getMAC() {
      return address;
    }

    void addAddress(String adr) {
      address = adr;
    }

    Device() {}
};

Device dev1;
Device dev2;

Menu* currentMenu;
Menu* settings;
Menu* devices;
Menu* mainMenu;
Menu* credits;

class Menu {

  public:
    short cursorIndex = 0;
    short cursorMax = 3;
    bool speaker = true;
    String title;
    String menuItems[4];


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
      tone(SPK, 5000 * speaker, 5);
      unHighlightOption();
      ++cursorIndex;
      if (cursorIndex > cursorMax) {
        --cursorIndex;
      }
      highlightOption();
    }

    void cursorUp() {
      tone(SPK, 5000 * speaker, 5);
      unHighlightOption();
      --cursorIndex;
      if (cursorIndex < 0) {
        ++cursorIndex;
      }
      highlightOption();
    }

    void selectOption() {
      tone(SPK, 2500 * speaker, 40);
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
        } else if (title == "Device One" || title == "Device Two") {
          devices->printMenu();
        }
      } else if (menuItems[cursorIndex] == "Light Mode" || menuItems[cursorIndex] == "Dark Mode") {
        if (menuItems[cursorIndex] == "Light Mode") { 
          color[6] = ST77XX_BLACK;
          color[7] = ST77XX_WHITE;
          menuItems[2] = "Dark Mode";
        } else if (menuItems[cursorIndex] == "Dark Mode") {
          color[7] = ST77XX_BLACK;
          color[6] = ST77XX_WHITE;
          menuItems[2] = "Light Mode";
        }
        printMenu();
      } else if (menuItems[cursorIndex] == "Credits") {
        credits->printMenu();
      } else if (menuItems[cursorIndex] == "Device One") {
        dev1.menu->printMenu();
      } else if (menuItems[cursorIndex] == "Device Two") {
        dev2.menu->printMenu();
      } else if (menuItems[cursorIndex] == "Play Sound") {
        if (title == "Device One") {
          dev1.buzz();
        } else if (title == "Device Two") {
          dev2.buzz();
        }
      } else if (menuItems[cursorIndex] == "Speaker: On" || menuItems[cursorIndex] == "Speaker: Off") {
        if (speaker) {
          speaker = false;
          menuItems[3] = "Speaker: Off";
        } else if (!speaker) {
          speaker = true;
          menuItems[3] = "Speaker: On ";
          unHighlightOption();
          menuItems[3] = "Speaker: On";
        }
        highlightOption();
      }
    }

    Menu(String tt, String option1, String option2, String option3, String option4) {
      title = tt;
      menuItems[0] = option1;
      menuItems[1] = option2;
      menuItems[2] = option3;
      menuItems[3] = option4;
    }

    Menu(String tt, String option1, String option2, String option3, String option4, int max) {
      title = tt;
      menuItems[0] = option1;
      menuItems[1] = option2;
      menuItems[2] = option3;
      menuItems[3] = option4;
      cursorMax = max;
    }

    Menu(bool idk) {
      devices = new Menu("Devices", "<< Back", "Device One", "Device Two", "", 2);
      settings = new Menu("Settings", "<< Back", "Add a Device", "Light Mode", "Speaker: On");
      mainMenu = new Menu("Main Menu", "Devices", "Settings", "Roll Call", "Credits");
      credits = new Menu("Credits", "<< Back", "Emmett L.M.", "Joshua Curtis", "Sunay Agarwal", 0);
      currentMenu = mainMenu;
      dev1.menu = new Menu("Device One", "<< Back", "Play Sound", "", "", 1);
      dev2.menu = new Menu("Device Two", "<< Back", "Play Sound", "", "", 1);
    }
};

Menu base = Menu(true);

void setup() {
  Serial.begin(115200);
  const char* ssid = "ESP32AP1";
  const char* password = "123456789"; 
  WiFi.softAP(ssid, password, 1, true);
  Serial.print("Access point started. IP address: ");
  Serial.println(WiFi.softAPIP());
  clients = WiFi.softAPgetStationNum(); 
   
  server.begin();
   
  pinMode(SPK, OUTPUT);
  
  pinMode(buttonIn, INPUT_PULLUP);
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  aLastState = digitalRead(outputA);

  tft.init(240, 320);
  delay(500);
  tft.setTextWrap(true);
  delay(100);
  tft.setRotation(3);
  delay(100);
  tft.fillScreen(ST77XX_BLACK);
  delay(100);
  currentMenu->printMenu();
  delay(100);
}


void loop() {
  
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        Serial.print("Received data: ");
        Serial.println(request);
        dev1.addAddress(request);

      }
    }
    Serial.print("Number of connected clients: ");
    Serial.println(WiFi.softAPgetStationNum());
  }

  if (clients < prevClients) {
    client.println("return address pls");
    String address = client.readStringUntil('\r');
  }  
  
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
  if (button == true && buttonLastState == false) {
    currentMenu->selectOption();
  }
  aLastState = aState;
  buttonLastState = button;
  prevClients = clients;
}
