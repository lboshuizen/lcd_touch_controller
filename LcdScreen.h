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

public:

    LcdScreen(LcdDriver& driver): lcd(driver){}

    const uint16_t Width = LCD_2_8_WIDTH;
    const uint16_t Height = LCD_2_8_HEIGHT;

    void clear(COLOR color=BLACK){
        lcd.set_area_color(color, 0,0, Width, Height);
    }

    void set_point_color(COLOR color, uint16_t x, uint16_t y);
    void point(uint16_t x, uint16_t y, COLOR color, DOT_PIXEL pixel_size, DOT_STYLE style);
    void line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, COLOR color, LINE_STYLE style, DOT_PIXEL width);
    void rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, COLOR color, LINE_STYLE style, DOT_PIXEL width);
    void circle(uint16_t x, uint16_t y, uint16_t radius, COLOR color, LINE_STYLE style, DOT_PIXEL width);

    void character(uint16_t x, uint16_t y, const char c, COLOR txt_color, COLOR bgd_color, sFONT *font);
    void text(uint16_t x, uint16_t y, const char *text, COLOR txt_color, COLOR bgd_color, sFONT *font);
};


#endif //TOUCHCONTROLLER_LCDSCREEN_H
