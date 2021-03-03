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

#include "ssd1306_screen.h"

void SSD1306Screen::init(ScreenData screen_data){
    _screen_data = screen_data;
    pinMode(LED_BUILTIN, OUTPUT);

    _display = Adafruit_SSD1306(_screen_width, _screen_height, &Wire, _oled_reset);
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        for (;;)
        {
            pinMode(LED_BUILTIN, OUTPUT);
            digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
            delay(1000);                     // wait for a second
            digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
            delay(1000);
        }
    }

    _display.clearDisplay();
    _display.setTextColor(SSD1306_WHITE);
}

void SSD1306Screen::update()
{
    _display.clearDisplay();
    // _display.drawCircle(10, 20, 10, SSD1306_WHITE);

    _display.setCursor(10, 10);
    _display.println("Voltage: " + String(_screen_data.get_voltage()) + "V");
    _display.setCursor(10, 20);
    _display.println("Trip km: " + String(_screen_data.get_trip_km()) + "km");
    _display.setCursor(10, 30);
    _display.println("Speed: " + String(_screen_data.get_speed()) + "kmph");

    _display.display();

}