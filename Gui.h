//
// Created by Leon Boshuizen on 07/11/2021.
//

#ifndef TOUCHCONTROLLER_GUI_H
#define TOUCHCONTROLLER_GUI_H

#include <vector>
#include <functional>
#include "LcdScreen.h"

class Drawable {

protected:
    uint16_t _x;
    uint16_t _y;
    uint16_t _x2;
    uint16_t _y2;

private:

    std::function<bool(Drawable &)> _on_touched;
    static bool default_handler(Drawable &_){ return false; }

public:
    Drawable(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2):
        _x(x1), _y(y1), _x2(x2), _y2(y2),
        _on_touched(default_handler){};
    virtual void draw(const LcdScreen &scr) const = 0;

    void touched(std::function<bool(Drawable &)> &f){ _on_touched = f;}
};

class Control : public Drawable {

    bool _enabled;
    bool _visible;

public:
    Control(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) :
    Drawable(x1,y1,x2,y2),
    _enabled(true), _visible(true){}

    void draw(const LcdScreen &scr) const override;

    inline bool IsVisible() const { return _visible; };
    inline bool IsEnabled() const { return _enabled; };

    inline void disable(){ enable(false); }

    inline void enable(bool f=true){ _enabled = f;};
    inline void visible(bool f){ _visible = f;};
};

class Button : public Control {
private:
    COLOR   _color;
    const char *_text;
    const sFONT *_font;

public:

    void set_color(COLOR c){ _color = c; }
    [[nodiscard]] COLOR get_color() const { return _color; };

    Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, COLOR c):
        Control(x,y,x+w, y+h),
        _color(c), _text(nullptr), _font(nullptr){}

    Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, COLOR c, const char *t, const sFONT *f):
            Control(x,y,x+w, y+h),
            _color(c), _text(t), _font(f){}

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
