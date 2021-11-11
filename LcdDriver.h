//
// Created by Leon Boshuizen on 01/11/2021.
//

#ifndef TOUCHCONTROLLER_LCDDRIVER_H
#define TOUCHCONTROLLER_LCDDRIVER_H

#include <hardware/gpio.h>
#include "hardware/spi.h"
#include "Spi.h"
#include "Color.h"
#include <sys/unistd.h>
#include <LCD_Driver.h>

#define LCD_RST_PIN		15
#define LCD_DC_PIN		8
#define LCD_CS_PIN		9
#define LCD_CLK_PIN		10
#define LCD_BKL_PIN		13
#define LCD_MOSI_PIN	11
#define LCD_MISO_PIN	12

class Pin {

private:

public:
    explicit Pin(uint pinnr, bool pinmode): pin(pinnr), mode(pinmode){
        gpio_init(pinnr);
        gpio_set_dir(pinnr, pinmode);
    }

    inline void up() const{ gpio_put(pin, 1); }
    inline void down() const{ gpio_put(pin, 0); }

    bool value() const { return gpio_get(pin); }
    void value(bool v) const { return gpio_put(pin,v); }
    bool isUp() const { return value(); }
    bool isDown() const { return !isUp(); }

    const uint pin;
    const bool mode;
};

class LcdDriver {

private:

    Pin RST;
    Pin CS;
    Pin DC;

    Spi &_spi;

    inline void spi_write(uint8_t value){ _spi.write(value); }
    inline uint8_t spi_read(uint8_t value){ return _spi.read(value); }

    void write_parameter(uint8_t data);
    void write_reg(uint8_t reg);

    void set_reg(uint8_t reg, uint8_t data);

    void set_scandir(LCD_SCAN_DIR sd){ set_reg( 0x36, sd); write_reg(0x29); }

    void write_all_data(uint16_t data, uint32_t len);
    void init_reg();

public:

    LcdDriver(Spi &spi) : RST(Pin(LCD_RST_PIN, GPIO_OUT)),
                  CS(Pin(LCD_CS_PIN, GPIO_OUT)),
                  DC(Pin(LCD_DC_PIN, GPIO_OUT)),
                  _spi(spi)
                  {}

    LcdDriver(const LcdDriver& copy) = delete;

    void init(LCD_SCAN_DIR scanDir = L2R_U2D);

    void reset(){
        RST.up();
        sleep_ms(500);
        RST.down();
        sleep_ms(500);
        RST.up();
        sleep_ms(500);
    }

    void set_color(const Color & color, uint16_t x, uint16_t y ){
        write_all_data( (uint16_t) color, (uint32_t) x * (uint32_t) y);
    }

    void set_area_color(const Color & color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

    void set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

};


#endif //TOUCHCONTROLLER_LCDDRIVER_H
