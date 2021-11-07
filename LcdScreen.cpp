//
// Created by Leon Boshuizen on 01/11/2021.
//

#include <algorithm>
#include "LcdScreen.h"

using namespace std;

void LcdScreen::set_point_color(COLOR color, uint16_t x, uint16_t y) const{
    lcd.set_window(x,y,x,y);
    lcd.set_color(color,1,1);
}

void LcdScreen::point(uint16_t x, uint16_t y, COLOR color, DOT_PIXEL pixel_size, DOT_STYLE style) const{
    if ( x > Width || y > Height) {
        //DEBUG("GUI_DrawPoint Input exceeds the normal display range\r\n");
        return;
    }

    if (style == DOT_STYLE_DFT) {
        for (auto x_offset = 0; x_offset < 2 * pixel_size - 1; x_offset++) {
            for (auto y_offset = 0; y_offset < 2 * pixel_size - 1; y_offset++) {
                set_point_color(color, x + x_offset - pixel_size, y + y_offset - pixel_size);
            }
        }
        return;
    }

    for (auto x_offset = 0; x_offset < pixel_size; x_offset++) {
        for (auto u_offset = 0; u_offset < pixel_size; u_offset++) {
            set_point_color(color, x + x_offset - 1, y + u_offset - 1);
        }
    }

}

void LcdScreen::line( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, COLOR color, LINE_STYLE style, DOT_PIXEL width ) const
{
    // Bresenham's line algorithm
    const bool steep = (abs(y2 - y1) > abs(x2 - x1));
    if(steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if(x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const uint16_t dx = x2 - x1;
    const uint16_t dy = abs(y2 - y1);
    const int y_step = (y1 < y2) ? 1 : -1;

    int error = dx / 2;

    int y = (int) y1;

    const int maxX = (int)x2;

    for(int x=(int)x1; x<=maxX; x++)
    {
        if(steep)
        {
            point(y, x, color, width, DOT_STYLE_DFT);
        }
        else
        {
            point(x, y, color, width, DOT_STYLE_DFT);
        }

        error -= dy;
        if(error < 0)
        {
            y += y_step;
            error += dx;
        }
    }
}

void LcdScreen::rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, COLOR color, LINE_STYLE style, DOT_PIXEL width) const{
    // horizontals
    line(x1,y1,x2,y1, color, style, width);
    line(x1,y2,x2,y2, color, style, width);

    //verticals
    line(x1,y1,x1,y2, color, style, width);
    line(x2,y1,x2,y2, color, style, width);

}

void LcdScreen::circle(uint16_t xc, uint16_t yc, uint16_t radius, COLOR color, LINE_STYLE style, DOT_PIXEL width) const{

    auto segments = [this,color, width](uint16_t xc, uint16_t yc, uint16_t x, uint16_t y){
        point(xc+x, yc+y, color,width, DOT_STYLE_DFT);
        point(xc-x, yc+y, color,width, DOT_STYLE_DFT);
        point(xc+x, yc-y, color,width, DOT_STYLE_DFT);
        point(xc-x, yc-y, color,width, DOT_STYLE_DFT);
        point(xc+y, yc+x, color,width, DOT_STYLE_DFT);
        point(xc-y, yc+x, color,width, DOT_STYLE_DFT);
        point(xc+y, yc-x, color,width, DOT_STYLE_DFT);
        point(xc-y, yc-x, color,width, DOT_STYLE_DFT);
    };

    int x = 0, y = radius;
    int d = 3 - 2 * radius;
    segments(xc, yc, x, y);
    while (y >= x) {
        // for each pixel we will
        // draw all eight pixels

        x++;

        // check for decision parameter
        // and correspondingly
        // update d, x, y
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else
            d = d + 4 * x + 6;

        segments(xc, yc, x, y);
    }
};

void LcdScreen::character(uint16_t x, uint16_t y, const char c, COLOR txt_color, COLOR bgd_color, const sFONT *font) const{

    uint32_t char_index = (c - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &font->table[char_index];

    for (auto row = 0; row < font->Height; row++) {
        for (auto col = 0; col < font->Width; col++) {
            COLOR clr = *ptr & (0x80 >> (col % 8)) ? txt_color : bgd_color;
            point(x + col, y + row, clr, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            if (col % 8 == 7)
                ptr++;
        }
        if (font->Width % 8 != 0)
            ptr++;
    }
}

void LcdScreen::text(uint16_t x, uint16_t y, const char *text, COLOR txt_color, COLOR bgd_color, const sFONT *font) const{
    POINT Xpoint = x;
    POINT Ypoint = y;

    while (*text != '\0') {
        /*
        // wrap x
        if ((Xpoint + font->Width) > sLCD_DIS.LCD_Dis_Column) {
            Xpoint = x;
            Ypoint += font->Height;
        }

        // wrap y
        if ((Ypoint + font->Height) > sLCD_DIS.LCD_Dis_Page) {
            Xpoint = Xstart;
            Ypoint = Ystart;
        }
        */

        character(Xpoint,Ypoint,*text,txt_color, bgd_color, font);
        text++;
        Xpoint += font->Width;
    }
}