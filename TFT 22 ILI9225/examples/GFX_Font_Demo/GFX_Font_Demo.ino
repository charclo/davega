// Include application, user and local libraries
#include "SPI.h"
#include "TFT_22_ILI9225.h"

// Include font definition files
// NOTE: These files may not have all characters defined! Check the GFXfont def
// params 3 + 4, e.g. 0x20 = 32 = space to 0x7E = 126 = ~

#include <../fonts/FreeSans12pt7b.h>
#include <../fonts/FreeSans24pt7b.h>

#if defined (ARDUINO_ARCH_STM32F1)
#define TFT_RST PA1
#define TFT_RS  PA2
#define TFT_CS  PA0 // SS
#define TFT_SDI PA7 // MOSI
#define TFT_CLK PA5 // SCK
#define TFT_LED 0 // 0 if wired to +5V directly
#elif defined(ESP8266)
#define TFT_RST 4   // D2
#define TFT_RS  5   // D1
#define TFT_CLK 14  // D5 SCK
//#define TFT_SDO 12  // D6 MISO
#define TFT_SDI 13  // D7 MOSI
#define TFT_CS  15  // D8 SS
#define TFT_LED 2   // D4     set 0 if wired to +5V directly -> D3=0 is not possible !!
#elif defined(ESP32)
#define TFT_RST 26  // IO 26
#define TFT_RS  25  // IO 25
#define TFT_CLK 14  // HSPI-SCK
//#define TFT_SDO 12  // HSPI-MISO
#define TFT_SDI 13  // HSPI-MOSI
#define TFT_CS  15  // HSPI-SS0
#define TFT_LED 0 // 0 if wired to +5V directly
#else
#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 3   // 0 if wired to +5V directly
#endif

#define TFT_BRIGHTNESS 200 // Initial brightness of TFT backlight (optional)

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);
// Use software SPI (slower)
//TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED, TFT_BRIGHTNESS);

// Variables and constants
int16_t x=0, y=0, w, h;

// Setup
void setup() {
  tft.begin();
  tft.clear();

  // Draw first string in big font
  String s1 = "6789";
  tft.setGFXFont(&FreeSans24pt7b); // Set current font
  tft.getGFXTextExtent(s1, x, y, &w, &h); // Get string extents
  y = h; // Set y position to string height
  tft.drawGFXText(x, y, s1, COLOR_RED); // Print string

  // Draw second string in smaller font
  tft.setGFXFont(&FreeSans12pt7b);  // Change font
  String s2 = "Hello"; // Create string object
  tft.getGFXTextExtent(s2, x, y, &w, &h); // Get string extents
  y += h + 10; // Set y position to string height plus shift down 10 pixels
  tft.drawGFXText(x, y, s2, COLOR_BLUE); // Print string
  
  // Draw third string in same font
  String s3 = "World!"; // Create string object
  y += h + 2; // Set y position to previous string height plus shift down 2 pixels
  tft.drawGFXText(x, y, s3, COLOR_GREEN); // Print string
}

// Loop
void loop() {
}
