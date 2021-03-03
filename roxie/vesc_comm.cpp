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

#include "vesc_comm.h"
#include "crc.h"
#include "roxie_config.h"

#ifdef ARDUINO_NANO_EVERY
HardwareSerial &vesc_serial = Serial1;
#else
HardwareSerial &vesc_serial = Serial;
#endif

#define PACKET_GET_VALUES_TYPE 4
#define PACKET_LENGTH_IDENTIFICATION_BYTE_SHORT 2
#define PACKET_LENGTH_IDENTIFICATION_BYTE_LONG 3
#define PACKET_TERMINATION_BYTE 3

uint8_t GET_VALUES_PACKET[] = {0x02, 0x01, 0x04, 0x40, 0x84, 0x03};

uint8_t expected_packet_length(uint8_t payload_length)
{
    return (1 + 1 + payload_length + 2 + 1);
}

VescComm::~VescComm()
{
    if (_packet != NULL)
        free(_packet);
}

void VescComm::init(uint32_t baud)
{
    vesc_serial.begin(baud);
}

uint8_t VescComm::fetch_packet(uint16_t timeout)
{
#ifndef SIM_VALUES
    //DEB("fetching packet");
    vesc_serial.write(GET_VALUES_PACKET, sizeof(GET_VALUES_PACKET));
    return receive_packet(timeout);
#endif
    //DEB("Using simulated values");
    _bytes_read = 78;
    return 0;
}

uint8_t VescComm::receive_packet(uint16_t timeout)
{
    int32_t start = millis();
    _bytes_read = 0;
    while (millis() - start < timeout)
    {
        if (vesc_serial.available())
            _packet[_bytes_read++] = vesc_serial.read();

        if (_bytes_read >= _max_packet_length)
            break;

        if (_bytes_read >= 2 && _packet[0] != PACKET_LENGTH_IDENTIFICATION_BYTE_SHORT)
        {
            uint8_t payload_length = _packet[1];
            if (_bytes_read >= expected_packet_length(payload_length))
                break;
        }
    }
    //read any left-over bytes without storing
    while (vesc_serial.available())
    {
        // TODO: warning
        vesc_serial.read();
    }
    return _bytes_read;
}

bool VescComm::is_expected_packet()
{
    /*     for(int i = 0; i < _bytes_read; i++)
    {
        //DEB(String(_packet[i]) + " ");
    }*/
    //DEB("bytes read : " + String(_bytes_read) + " bytes)");
    if (_bytes_read < 3)
    {
        //DEB("packet too short (" + String(_bytes_read) + " bytes)");
        return false;
    }

    if (_packet[0] != PACKET_LENGTH_IDENTIFICATION_BYTE_SHORT)
    {
        //DEB("unexpected length id byte: expected " + String(PACKET_LENGTH_IDENTIFICATION_BYTE_SHORT) +
        // ", got " + String(_packet[0]));
        return false;
    }

    if (_packet[2] != PACKET_GET_VALUES_TYPE)
    {
        //DEB("unexpected packet type: expected " + String(PACKET_GET_VALUES_TYPE) +
        // ", got " + String(_packet[2]));
        return false;
    }

    uint8_t payload_length = _packet[1];
    if (_bytes_read != expected_packet_length(payload_length))
    {
        //DEB("packet length (" + String(payload_length) + ") does not correspond to the payload length (" +
        //  String(payload_length) + ")");
        return false;
    }

    uint16_t crc = get_word(payload_length + 2);
    uint16_t expected_crc = crc16(&_packet[2], payload_length);
    if (crc != expected_crc)
    {
        //DEB("CRC error: expected " + String(expected_crc) + ", got " + String(crc));
        return false;
    }

    return true;
}

uint16_t VescComm::get_word(uint8_t index)
{
    return ((uint16_t)_packet[index]) << 8 | ((uint16_t)_packet[index + 1]);
}

uint32_t VescComm::get_long(uint8_t index)
{
    return ((uint32_t)_packet[index]) << 24 |
           ((uint32_t)_packet[index + 1]) << 16 |
           ((uint32_t)_packet[index + 2]) << 8 |
           ((uint32_t)_packet[index + 3]);
}
