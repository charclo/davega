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

#include "roxie_config.h"
#include "eeprom_backup.h"
#include "util.h"
#include "screen.h"
#include "vesc_comm.h"
#include "button_interrupt.h"
#include "vertical_screen.h"
//#include "screen_data.h"
//#include "ssd1306_screen.h"

#ifdef FOCBOX_UNITY
#include "vesc_comm_unity.h"
VescCommUnity vesc_comm = VescCommUnity();
#else
#include "vesc_comm_standard.h"
VescCommStandard vesc_comm = VescCommStandard();
#endif

VerticalScreen vertical_screen;
t_screen_config screen_config;
t_data data;                  // struct containing data received from vesc
t_session_data session_data;  // data from this session
int32_t startup_trip_meters;  // trip meters that were stored in the EEPROM on startup
int32_t startup_total_meters; // total meters that were stored in the EEPROM on startup
int32_t last_total_meters_stored;
int32_t last_stopped;
int32_t last_rpm;

Button button1 = Button(BUTTON_1_PIN);
Button button2 = Button(BUTTON_2_PIN);
Button button3 = Button(BUTTON_3_PIN);

//SSD1306Screen ssd1306_screen;
//ScreenData test;

//HardwareSerial Serial3(PB11, PB10);

void setup()
{
    // delay(1000); // Only for testing the EEPROM

    // Initialize communication with computer for debugging and with vesc
    //Serial3.begin(115200);
    vesc_comm.init(115200);

    attachInterrupt(digitalPinToInterrupt(BUTTON_1_PIN), button1_changed, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BUTTON_2_PIN), button2_changed, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BUTTON_3_PIN), button3_changed, CHANGE);

    screen_config = {make_fw_version(FW_VERSION, REVISION_ID), IMPERIAL_UNITS, USE_FAHRENHEIT,
                     SHOW_AVG_CELL_VOLTAGE, BATTERY_S, SCREEN_ORIENTATION};
    vertical_screen = VerticalScreen();

    // Initialize EEPROM and/or read stored data from it.
    if (!eeprom_is_initialized(EEPROM_MAGIC_VALUE))
    {
        eeprom_initialize(EEPROM_MAGIC_VALUE, session_data, data);
    }
    eeprom_read_data(&data, session_data);

    // Initialize the screen and draw the basic interface
    vertical_screen.init(&screen_config);
    vertical_screen.draw_basic();

    vesc_comm.fetch_packet();
    while (!vesc_comm.is_expected_packet())
    {
        vertical_screen.heartbeat(UPDATE_DELAY, false);
        vesc_comm.fetch_packet();
    }
    vesc_comm.process_packet(&data);

    load_startup_values();
    vertical_screen.update(&data);

    //test = ScreenData(&data);
    //ssd1306_screen.init(test);
}

void loop()
{
    check_buttons();

    vesc_comm.fetch_packet();
    if (!vesc_comm.is_expected_packet())
    {
        vertical_screen.heartbeat(UPDATE_DELAY, false);
        return;
    }
    vesc_comm.process_packet(&data);

    process_other_values();

    vertical_screen.update(&data);
    vertical_screen.heartbeat(UPDATE_DELAY, true);

    //ssd1306_screen.update();
}

void check_buttons()
{

    button1.update_button();
    button2.update_button();
    button3.update_button();

    if (button2.get_long_click())
    {
        startup_trip_meters = 0 - rotations_to_meters(data.tachometer / 6);
        vertical_screen.process_buttons(&data, true);
    }
}

void load_startup_values()
{
    // Store the trip and total meter values on startup
    t_session_data startup = eeprom_read_session_data();
    startup_trip_meters = startup.trip_meters;
    startup_total_meters = eeprom_read_total_distance(); // TODO: read from data instead of EEPROM
    last_total_meters_stored = startup_total_meters;     // set the startup values as the last values stored in EEPROM

    // Subtract current VESC values, which could be non-zero in case the display
    // got reset without resetting the VESC as well. The invariant is:
    //   current value = initial value + VESC value
    // and that works correctly with the default initial values in case the VESC values
    // start from 0. If that's not the case though we need to lower the initial values.
    int32_t tachometer = rotations_to_meters(data.tachometer / 6);
    startup_trip_meters -= tachometer;
    startup_total_meters -= tachometer;
}

void process_other_values()
{
    int32_t tachometer_meters = rotations_to_meters(data.tachometer / 6);

    session_data.trip_meters = startup_trip_meters + tachometer_meters;
    int32_t total_meters = startup_total_meters + tachometer_meters;

    data.trip_km = session_data.trip_meters / 1000.0;
    data.total_km = total_meters / 1000.0;

    // update EEPROM
    bool came_to_stop = (last_rpm != 0 && data.rpm == 0);
    bool traveled_enough_distance = (total_meters - last_total_meters_stored >= EEPROM_UPDATE_EACH_METERS);
    if (traveled_enough_distance || (came_to_stop && millis() - last_stopped > EEPROM_UPDATE_MIN_DELAY_ON_STOP))
    {
        if (came_to_stop)
            last_stopped = millis();
        last_total_meters_stored = total_meters;
        eeprom_write_total_distance(total_meters);
        eeprom_write_session_data(session_data);
    }

    last_rpm = data.rpm;
}

//ISR routines for the buttons
void button1_changed()
{
    button1.button_changed();
}

void button2_changed()
{
    button2.button_changed();
}

void button3_changed()
{
    button3.button_changed();
}