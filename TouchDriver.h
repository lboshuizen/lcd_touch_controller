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

    int rotation = 0;

    Point last = {0,0};

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
        gpio_set_pulls(IRQ.pin, true, false);
        _lastread = get_absolute_time();
        auto br = _spi.switch_baudrate(1000000);
        read_adc(0x80);
        _spi.set_baudrate( br );
    }

    [[nodiscard]] Point lastTouch() const {return {_x, _y}; }

    bool isTouched(){ return IRQ.isDown(); }

    const Point *scan(LcdScreen &scr){

        //printf("%d", IRQ.value());

        auto br = _spi.switch_baudrate(1000000);
        update();
        _spi.set_baudrate( br );

        _x /= (4095/scr.Width);
        _y /= (4095/scr.Height);

        last = {_x,_y};

        if(IRQ.value()) return nullptr;

        printf("%03d|%03d\t", _x, _y);
        printf("%03d|%03d\n", last.X(), last.Y());

        return &last;
    }

};


#endif //TOUCHCONTROLLER_TOUCHDRIVER_H
