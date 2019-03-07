#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"

#ifdef ESP8266
   #define STMPE_CS 16
   #define TFT_CS   0
   #define TFT_DC   15
   #define SD_CS    2
#endif
#ifdef ESP32
   #define STMPE_CS 32
   #define TFT_CS   15
   #define TFT_DC   33
   #define SD_CS    14
#endif

#define TFT_RST -1

// Function prototypes
void setup(void);
void loop(void);
void initDisplay(void);
void writeText(const char *);
void writeChar(char);

// Use hardware SPI and the above for CS/DC
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);


void setup() {
  Serial.begin(115200);
  tft.begin();
  initDisplay();
  writeText("Initialized");
}


void loop(void) {
  if (Serial.available() > 0) 
  {
    char incomingByte = Serial.read(); // read the incoming byte:
    writeChar(incomingByte);
  }
}

void initDisplay() {
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);  
  tft.setTextSize(1);
  tft.setRotation(2);         // Portrait with USB at bottom of ESP32 board
}

void writeText(const char *text) {
  tft.println(text);
  Serial.println(text);
}

void writeChar(char byte)
{
  tft.print(byte);
  Serial.print(byte);
}