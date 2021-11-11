//
// Created by Leon Boshuizen on 07/11/2021.
//

#include "Gui.h"

void Control::draw(const LcdScreen &scr) const
{
}

void Button::draw(const LcdScreen &scr) const
{
    scr.rect(r.TL,r.BR,_color, LINE_SOLID, DOT_PIXEL_1X1);
    if( _text == nullptr) return;
    scr.text( r.TL.shift(5,5+_font->Height), _text, _color,scr.background(),_font);
};
