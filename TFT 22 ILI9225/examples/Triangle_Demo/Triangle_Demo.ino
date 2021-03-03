// Include application, user and local libraries
#include "SPI.h"
#include "TFT_22_ILI9225.h"
#include "math.h"

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

#define ROTATE_ANGLE 10 // Angle in degrees to rotate the triangle

struct _point
{
    int16_t x;
    int16_t y;
};

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);

// Variables and constants
_point c1, c2, c3, cc;

// Setup
void setup() {
  tft.begin();

  // Define triangle start coordinates
  c1.x = 30;  c1.y = 30;
  c2.x = 120; c2.y = 80;
  c3.x = 80;  c3.y = 130;

  // Determine the rotation point, i.e. the center of the triangle
  cc = getCoordCentroid(c1, c2, c3);
  
  tft.clear();
}

// Loop
void loop() {
  // Calculate the number of steps to rotate the triangle a full rotation
  int16_t steps = (int16_t)(360 / ROTATE_ANGLE);

  // Draw solid triangle
  tft.fillTriangle(30, 190, 80, 150, 130, 210, COLOR_BLUE);

  for (int8_t i = 0; i < steps; i++) {
    // Draw triangle
    tft.drawTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, COLOR_GREEN);
    // Rotate triangle
    rotateTriangle(c1, c2, c3, cc, ROTATE_ANGLE);
    delay(50);
  }
  delay(5000);
  tft.clear();
}

// Get centroid of triangle
_point getCoordCentroid( _point a, _point b, _point c ) {
  _point o;

  o.x = (int16_t)((a.x + b.x + c.x) / 3);
  o.y = (int16_t)((a.y + b.y + c.y) / 3);

  return o;
}

// Rotate triangle around point r
void rotateTriangle( _point &a, _point &b, _point &c, _point r, int16_t deg ) {

  // Convert degrees to radians
  float angle = (float)deg * 1000 / 57296;

  // Rotate each individual point
  a = rotatePoint( r, angle, a);
  b = rotatePoint( r, angle, b);
  c = rotatePoint( r, angle, c);
}

// Rotate each point p around c
_point rotatePoint( _point c, float angle, _point p ) {
  _point r;
  
  // 1. translate point back to origin
  // 2. rotate point
  // 3. translate point back

  r.x = cos(angle) * (p.x - c.x) - sin(angle) * (p.y - c.y) + c.x;
  r.y = sin(angle) * (p.x - c.x) + cos(angle) * (p.y - c.y) + c.y;

  return r;
}

