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

#ifndef BUTTON_INTERRUPT_H
#define BUTTON_INTERRUPT_H

#include <Arduino.h>

class Button {
public:
    Button(uint8_t button_pin);

    /**
     * @brief Update the button status. This function must be called every loop iteration.
     * 
     * @return true if button was pressed
     */
    bool update_button();

    /**
     * @brief This function should be called by an ISR every time a state change for this button occurs.
     * 
     */
    void button_changed();

    /**
     * @brief Check to see if the button was clicked. The _clicked flag is reset every time this function is called.
     * 
     * @return true 
     * @return false 
     */
    bool get_clicked();

    /**
     * @brief Check to see if a long click occurred. The _long_click flag is reset every time this function is called.
     * 
     * @return true 
     * @return false 
     */
    bool get_long_click();

private:
    uint8_t _button_pin;
    unsigned long _fallen_time;
    unsigned long _risen_time;
    volatile bool _fallen_flag = false;
    volatile bool _risen_flag = false;
    bool _clicked = false;
    bool _long_click = false;

};

#endif //BUTTON_INTERRUPT_H