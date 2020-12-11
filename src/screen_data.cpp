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

#include "screen_data.h"

ScreenData::ScreenData(t_data *screen_data){
    _screen_data = screen_data;
}

float ScreenData::get_voltage(){
    return _screen_data->voltage;
}

float ScreenData::get_trip_km(){
    return _screen_data->trip_km;
}

float ScreenData::get_speed(){
    return convert_km_to_miles(_screen_data->speed_kph, false);
}

float ScreenData::convert_km_to_miles(float kilometer, bool imperial_units)
{
    if (imperial_units)
        return kilometer * KM_PER_MILE;
    else
        return kilometer;
}
