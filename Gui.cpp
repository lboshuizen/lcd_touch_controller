//
// Created by Leon Boshuizen on 07/11/2021.
//

#include "Gui.h"

void Control::draw(const LcdScreen &scr) const
{
    scr.rect(200,10,220,20, Black, LINE_SOLID, DOT_PIXEL_2X2);
}

void Button::draw(const LcdScreen &scr) const
{
    scr.rect(_x,_y,_x2, _y2,_color, LINE_SOLID, DOT_PIXEL_1X1);
    if( _text == nullptr) return;
    scr.text(_x+5, _y+5+_font->Height, _text, _color,scr.background(),_font);
};
