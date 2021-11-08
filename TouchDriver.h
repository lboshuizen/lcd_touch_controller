//
// Created by Leon Boshuizen on 08/11/2021.
//

#ifndef TOUCHCONTROLLER_TOUCHDRIVER_H
#define TOUCHCONTROLLER_TOUCHDRIVER_H


#include "LcdDriver.h"
#include "LcdScreen.h"

#define CS_PIN		16
#define IRQ_PIN		17

class TouchDriver {

private:

    uint16_t read_adc(uint8_t channel);
    void update();

    Pin CS;
    Pin IRQ;
    Spi &_spi;

    absolute_time_t _lastread = {0};

    int rotation = 1;

public:

    uint16_t _x;
    uint16_t _y;

    explicit TouchDriver(Spi &spi):
                   CS(Pin(CS_PIN, GPIO_OUT)),
                   IRQ(Pin(IRQ_PIN, GPIO_IN)),
                   _x(0), _y(0),
                   _spi(spi)
                   {}

    void init(){
        CS.up();
        gpio_set_pulls(IRQ_PIN, true, false);
        _lastread = get_absolute_time();
    }

    inline bool isTouched(){ return IRQ.isDown(); }

    bool scan(LcdScreen &scr){

        if( !isTouched() ) return false;

        auto br = _spi.switch_baudrate(1000000);
        update();
        _spi.set_baudrate( br );

        auto t = _x;
        _x = _y;
        _y = t;
        _x = scr.Width - _x;

        scr.point(_x,_y,WHITE, DOT_PIXEL_4X4, DOT_FILL_AROUND);


        return true;
    }

};


#endif //TOUCHCONTROLLER_TOUCHDRIVER_H
