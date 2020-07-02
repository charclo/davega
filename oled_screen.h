#ifndef OLED_SCREEN_H
#define OLED_SCREEN_H

//#include <TFT_22_ILI9225.h>
//#include "davega_ili9225_screen.h"
//#include "davega_simple_screen.h"

class DavegaSimpleVerticalScreen: public DavegaILI9225Screen {
public:
    DavegaSimpleVerticalScreen(t_screen_item primary_item) { _primary_item = primary_item; }
    void reset();
    void update(t_davega_data* data);
    void heartbeat(uint32_t duration_ms, bool successful_vesc_read);

protected:
    // Have we just reset the screen? Unset by the first update() call.
    bool _just_reset = false;

    // Remember how many cells are currently filled so that we can update the indicators more efficiently.
    uint8_t _battery_cells_filled = 0;

    vesc_comm_fault_code _last_fault_code = FAULT_CODE_NONE;

    t_screen_item _primary_item = _primary_item;

    void _update_battery_indicator(float battery_percent, bool redraw = false);
};

#endif //OLED_SCREEN_H
