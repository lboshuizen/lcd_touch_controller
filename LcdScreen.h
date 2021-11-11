//
// Created by Leon Boshuizen on 01/11/2021.
//

#ifndef TOUCHCONTROLLER_LCDSCREEN_H
#define TOUCHCONTROLLER_LCDSCREEN_H


#include <LCD_GUI.h>
#include "LcdDriver.h"
#include "Color.h"

class LcdScreen {

private:

    LcdDriver &lcd;
    Color _background;
//    View *_view;

public:

    explicit LcdScreen(LcdDriver& driver): lcd(driver), _background(Color(0,0,0)){}

    const uint16_t Width = LCD_2_8_HEIGHT;
    const uint16_t Height = LCD_2_8_WIDTH;

    void clear(const Color &color){
        lcd.set_area_color(color, 0,0, Width, Height);
        _background = color;
    }

//    inline void set_view(View *v){ _view = v; }
//    inline View *get_view() const {return _view; }

    inline const Color &background() const { return _background; }

    void set_point_color(const Color & color, uint16_t x, uint16_t y) const;
    void point(uint16_t x, uint16_t y, const Color & color, DOT_PIXEL pixel_size, DOT_STYLE style) const;
    void line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const Color & color, LINE_STYLE style, DOT_PIXEL width) const;
    void rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const Color & color, LINE_STYLE style, DOT_PIXEL width) const;
    void circle(uint16_t x, uint16_t y, uint16_t radius, const Color & color, LINE_STYLE style, DOT_PIXEL width) const;

    void character(uint16_t x, uint16_t y, const char c, const Color & txt_color, const Color & bgd_color, const sFONT *font) const;
    void text(uint16_t x, uint16_t y, const char *text, const Color & txt_color, const Color & bgd_color, const sFONT *font) const;
};


#endif //TOUCHCONTROLLER_LCDSCREEN_H
