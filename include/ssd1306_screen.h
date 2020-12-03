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

class SSD1306Screen
{
public:
    SSD1306Screen();
    //void init(); //ScreenData screen_data);
    void update();

private:
    ScreenData _screen_data;
    uint8_t _screen_width = 128;
    uint8_t _screen_height = 64;
    uint8_t _oled_reset = -1;
    Adafruit_SSD1306 _display;
};

#endif //SSD1306_SCREEN_H