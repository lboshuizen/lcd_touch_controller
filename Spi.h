//
// Created by Leon Boshuizen on 08/11/2021.
//

#ifndef TOUCHCONTROLLER_SPI_H
#define TOUCHCONTROLLER_SPI_H

#include <hardware/spi.h>
#include "stdint.h"

class Spi {

    spi_inst_t *_channel;
    uint _baud_rate;

public:

    Spi(spi_inst_t *channel, uint rate): _channel(channel), _baud_rate(rate){};

    inline void init(){ spi_init(_channel, _baud_rate); }

    inline uint set_baudrate(uint br){
        _baud_rate = spi_set_baudrate(_channel, br);
        return _baud_rate;
    }

    uint switch_baudrate(uint br){
        auto prev = _baud_rate;
        _baud_rate = set_baudrate(br);
        return prev;
    }

    uint8_t write(uint8_t value){
        uint8_t rxDat;
        spi_write_read_blocking(_channel,&value,&rxDat,1);
        return rxDat;
    }

    inline uint8_t read(uint8_t value){
        return write(value);
    }

    uint16_t write8_read16(uint8_t d){
        write(d);

        uint16_t data = read(0X00);
        data <<= 8;//7bit
        data |= read(0X00);

        return data;
    }



};


#endif //TOUCHCONTROLLER_SPI_H
