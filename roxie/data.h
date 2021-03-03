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

#ifndef DATA_H
#define DATA_H

#include "vesc_comm.h"

typedef struct {
    uint32_t millis_elapsed;
    uint32_t millis_riding;
    float max_speed_kph;
    float min_voltage;
    int32_t trip_meters;
} t_session_data;

typedef struct {
    float mosfet_celsius;
    float motor_celsius;
    float motor_amps;
    float battery_amps;
    float duty_cycle;
    float voltage;
    float voltage_percent;
    int32_t mah_left;
    float mah_reset_progress;
    float mah_percent;
    float battery_percent;
    float speed_kph;
    float trip_km;
    float session_reset_progress;
    float total_km;
    float wh_spent;
    float mah_spent;
    vesc_comm_fault_code vesc_fault_code = FAULT_CODE_NONE;
    t_session_data* session;
    float mah_discharged;
    float mah_charged;
    float tachometer;
    float rpm;
} t_data;

typedef struct {
    const char* fw_version;
    bool imperial_units;
    bool use_fahrenheit;
    bool per_cell_voltage;
    uint8_t battery_cells;
    uint8_t orientation;
} t_screen_config;

#endif //DATA_H
