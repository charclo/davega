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

#ifndef SCREEN_DATA_H
#define SCREEN_DATA_H

#include "data.h"

class ScreenData {
public:
    ScreenData(){};
    ScreenData(t_data *screen_data);
    float get_voltage();

private:
    t_data *_screen_data;

};


#endif //SCREEN_DATA_H