//
// Created by Leon Boshuizen on 01/11/2021.
//

#ifndef TOUCHCONTROLLER_LCDSCREEN_H
#define TOUCHCONTROLLER_LCDSCREEN_H


#include <LCD_GUI.h>
#include "LcdDriver.h"

class LcdScreen {

private:

    LcdDriver &lcd;
    COLOR     _background;

public:

    explicit LcdScreen(LcdDriver& driver): lcd(driver), _background(0){}

    const uint16_t Width = LCD_2_8_HEIGHT;
    const uint16_t Height = LCD_2_8_WIDTH;

    void clear(COLOR color=0){
        lcd.set_area_color(color, 0,0, Width, Height);
        _background = color;
    }

    inline COLOR background() const { return _background; }

    void set_point_color(COLOR color, uint16_t x, uint16_t y) const;
    void point(uint16_t x, uint16_t y, COLOR color, DOT_PIXEL pixel_size, DOT_STYLE style) const;
    void line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, COLOR color, LINE_STYLE style, DOT_PIXEL width) const;
    void rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, COLOR color, LINE_STYLE style, DOT_PIXEL width) const;
    void circle(uint16_t x, uint16_t y, uint16_t radius, COLOR color, LINE_STYLE style, DOT_PIXEL width) const;

    void character(uint16_t x, uint16_t y, const char c, COLOR txt_color, COLOR bgd_color, const sFONT *font) const;
    void text(uint16_t x, uint16_t y, const char *text, COLOR txt_color, COLOR bgd_color, const sFONT *font) const;
};


#endif //TOUCHCONTROLLER_LCDSCREEN_H
