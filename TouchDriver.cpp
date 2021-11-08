//
// Created by Leon Boshuizen on 08/11/2021.
//

#include "TouchDriver.h"

uint16_t TouchDriver::read_adc(uint8_t channel){

    _spi.write(channel);

    //A cycle of at least 400ns.
    sleep_us(10);

    uint16_t data = _spi.read(0X00);
    data <<= 8;//7bit
    data |= _spi.read(0X00);
    data >>= 4;//5bit

    return data;
}

#define MSEC_THRESHOLD 3000
#define Z_THRESHOLD 400
#define Z_THRESHOLD_INT 75

static uint16_t best_two_avg( uint16_t x , uint16_t y , uint16_t z ) {
    uint16_t da, db, dc;
    uint16_t reta = 0;
    if ( x > y ) da = x - y; else da = y - x;
    if ( x > z ) db = x - z; else db = z - x;
    if ( z > y ) dc = z - y; else dc = y - z;

    if ( da <= db && da <= dc ) reta = (x + y) >> 1;
    else if ( db <= da && db <= dc ) reta = (x + z) >> 1;
    else reta = (y + z) >> 1;   //    else if ( dc <= da && dc <= db ) reta = (x + y) >> 1;

    return (reta);
}

void TouchDriver::update()
{
    uint16_t data[6] = {0,0,0,0,0,0};
    int z;
    absolute_time_t now = get_absolute_time();

    if (absolute_time_diff_us(_lastread,now) < MSEC_THRESHOLD) return;

    CS.down();

    read_adc(0xB1); //Z1
    uint16_t z1 = read_adc(0xC1 /* Z2 */) >> 3;
    uint16_t z2 = read_adc(0x91 /* X */) >> 3;

    z = z1 + 4095 - z2;

    if (z < 0) z = 0;
    if (z < Z_THRESHOLD) {
        return;
    }

    read_adc(0x91 /* X */);  // dummy X measure, 1st is always noisy
    data[0] = read_adc(0xD1 /* Y */) >> 3;
    data[1] = read_adc(0x91 /* X */) >> 3;
    data[2] = read_adc(0xD1 /* Y */) >> 3;
    data[3] = read_adc(0x91 /* X */) >> 3;

    data[4] = read_adc(0xD1 /* Y */) >> 3;
    data[5] = read_adc(0x90 /* X */) >> 3; // last, allow panel to power down

    CS.up();

    uint16_t x = best_two_avg( data[0], data[2], data[4] );
    uint16_t y = best_two_avg( data[1], data[3], data[5] );

    _lastread = now;	// good read completed, set wait
    switch (rotation) {
        case 0:
            _x = 4095 - y;
            _y = x;
            break;
        case 1:
            _x = x;
            _y = y;
            break;
        case 2:
            _x = y;
            _y = 4095 - x;
            break;
        default: // 3
            _x = 4095 - x;
            _y = 4095 - y;
    }
}