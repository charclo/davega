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

#ifndef SIMPLE_VERTICAL_SCREEN_H
#define SIMPLE_VERTICAL_SCREEN_H

#include <TFT_22_ILI9225.h>
#include "ili9225_screen.h"
#include "util.h"

typedef enum {
    DEFAULT_SCREEN,
    TEMP_SCREEN,
    SPEED_SCREEN
} t_value_screen;

class VerticalScreen: public ILI9225Screen {
public:
    VerticalScreen() {}
    void draw_basic();
    void update(t_data* data);
    bool process_buttons(t_data *data, bool long_click);
    void heartbeat(uint32_t duration_ms, bool successful_vesc_read);


protected:
    // Have we just reset the screen? Unset by the first update() call.
    bool _just_reset = false;

    // Remember how many cells are currently filled so that we can update the indicators more efficiently.
    uint8_t _battery_cells_filled = 0;

    vesc_comm_fault_code _last_fault_code = FAULT_CODE_NONE;

    void _update_battery_indicator(float battery_percent, bool redraw = false);
};

#endif //SIMPLE_VERTICAL_SCREEN_H
