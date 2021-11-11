//
// Created by Leon Boshuizen on 01/11/2021.
//

#ifndef TOUCHCONTROLLER_LCDSCREEN_H
#define TOUCHCONTROLLER_LCDSCREEN_H


#include "LcdDriver.h"
#include "Color.h"
#include "fonts.h"

#define LCD_2_8_WIDTH  	240  //LCD widthNew KB
#define LCD_2_8_HEIGHT   320

typedef enum {
    DOT_PIXEL_1X1  = 1,		// dot pixel 1 x 1
    DOT_PIXEL_2X2  , 		// dot pixel 2 X 2
    DOT_PIXEL_3X3  ,		// dot pixel 3 X 3
    DOT_PIXEL_4X4  ,		// dot pixel 4 X 4
    DOT_PIXEL_5X5  , 		// dot pixel 5 X 5
    DOT_PIXEL_6X6  , 		// dot pixel 6 X 6
    DOT_PIXEL_7X7  , 		// dot pixel 7 X 7
    DOT_PIXEL_8X8  , 		// dot pixel 8 X 8
} DOT_PIXEL;
#define DOT_PIXEL_DFT  DOT_PIXEL_1X1  //Default dot pilex

typedef enum {
    DOT_FILL_AROUND  = 1,		// dot pixel 1 x 1
    DOT_FILL_RIGHTUP  , 		// dot pixel 2 X 2
} DOT_STYLE;
#define DOT_STYLE_DFT  DOT_FILL_AROUND  //Default dot pilex

typedef enum {
    LINE_SOLID = 0,
    LINE_DOTTED,
} LINE_STYLE;

typedef enum {
    DRAW_EMPTY = 0,
    DRAW_FULL,
} DRAW_FILL;

class LcdScreen {

private:

    LcdDriver &lcd;
    Color _background;

    void point(uint16_t x,uint16_t y, const Color & color, DOT_PIXEL pixel_size, DOT_STYLE style) const;
    void set_point_color(const Color & color, uint16_t x, uint16_t y) const;

public:

    explicit LcdScreen(LcdDriver& driver): lcd(driver), _background({0,0,0}){}

    const uint16_t Width = LCD_2_8_HEIGHT;
    const uint16_t Height = LCD_2_8_WIDTH;

    void clear(const Color &color){
        lcd.set_area_color(color, 0,0, Width, Height);
        _background = color;
    }

    [[nodiscard]] inline const Color &background() const { return _background; }

    inline void point(const Point &p, const Color & color, DOT_PIXEL pixel_size, DOT_STYLE style) const { point(p.X(), p.Y(),color,pixel_size, style); };
    void line(const Point &s, const Point &e, const Color & color, LINE_STYLE style, DOT_PIXEL width) const;
    void rect(const Point &tl, const Point &br, const Color & color, LINE_STYLE style, DOT_PIXEL width) const;
    void circle(const Point &c, uint16_t radius, const Color & color, LINE_STYLE style, DOT_PIXEL width) const;

    void character(const Point &p, char c, const Color & txt_color, const Color & bgd_color, const sFONT *font) const;
    void text(const Point &p, const char *text, const Color & txt_color, const Color & bgd_color, const sFONT *font) const;
};


#endif //TOUCHCONTROLLER_LCDSCREEN_H
