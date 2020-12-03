/*
    This file is part of the Roxie firmware.

    Roxie firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Roxie firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Roxie firmware.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SSD1306_SCREEN_H
#define SSD1306_SCREEN_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "screen_data.h"



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

class SSD1306Screen {
public:
    SSD1306Screen() {};
    SSD1306Screen(Adafruit_SSD1306 display);//ScreenData screen_data);
    void update();

private:
    ScreenData _screen_data;
    //Adafruit_SSD1306 _display(int _screen_width, SCREEN_HEIGHT, &Wire, OLED_RESET);
    int _screen_width = 128;
    int _screen_height = 64;
    // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
    int _oled_reset = -1;
    Adafruit_SSD1306 _display;    

};


#endif //SSD1306_SCREEN_H