//
// Created by Leon Boshuizen on 07/11/2021.
//

#ifndef TOUCHCONTROLLER_GUI_H
#define TOUCHCONTROLLER_GUI_H

#include <vector>
#include "LcdScreen.h"

class Drawable {
public:
    virtual void draw(const LcdScreen &scr) const = 0;
};

class Control : public Drawable {

    bool _enabled;
    bool _visible;

public:
    Control() : _enabled(true), _visible(true){}

    void draw(const LcdScreen &scr) const override;

    bool IsVisible(){ return _visible; };
    bool IsEnabled(){ return _enabled; };

    void Enable(bool f){ _enabled = f;};
    void Visible(bool f){ _visible = f;};
};

class Button : public Control {
private:
    uint16_t _x;
    uint16_t _y;
    uint16_t _w;
    uint16_t _h;
    COLOR   _color;
    const char *_text;
    const sFONT *_font;

public:

    void set_color(COLOR c){ _color = c; }
    [[nodiscard]] COLOR get_color() const { return _color; };

    Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, COLOR c):
        _x(x), _y(y), _w(w), _h(h), _color(c), _text(nullptr), _font(nullptr){}

    Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, COLOR c, const char *t, const sFONT *f):
            _x(x), _y(y), _w(w), _h(h), _color(c), _text(t), _font(f){}

    void draw(const LcdScreen &scr) const override;

};

class View {

private:
    Drawable **_controls;
    size_t   _cnt;
public:

    explicit View(Drawable *c[], size_t l): _controls(c), _cnt(l){};

    void draw(const LcdScreen &scr) const {
        for(uint i=0; i<_cnt; i++ ){
            _controls[i]->draw(scr);
        }
    }
};

class Gui {

};


#endif //TOUCHCONTROLLER_GUI_H
