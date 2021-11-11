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

    bool _visible;

private:

    static bool default_handler(int, int){ return false; }

public:
    Drawable(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2):
        _x(x1), _y(y1), _x2(x2), _y2(y2),
        _visible(true)
        //on_touched(default_handler)
        {};

    bool touch_start(const Point &p){
        if( p.X() < _x || p.X() > _x2 || p.Y() < _y || p.Y() > _y2 ) return false;
        printf("hit\n");
        on_touch();
        return true;
    }

    virtual void touch_end(){}

    virtual bool on_touch(){ return false; };

    virtual void draw(const LcdScreen &scr) const = 0;

    inline bool IsVisible() const { return _visible; };
    inline void visible(bool f){ _visible = f;};
};

class Rect : Drawable {

};

class Text : Drawable {

};

class Circle : Drawable {

};

class Control : public Drawable {

    bool _enabled;

public:
    Control(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) :
    Drawable(x1,y1,x2,y2),
    _enabled(true){}

    void draw(const LcdScreen &scr) const override;

    inline bool IsEnabled() const { return _enabled; };
    inline void disable(){ enable(false); }
    inline void enable(bool f=true){ _enabled = f;};
};

class Button : public Control {
private:
    Color _color;
    const char *_text;
    const sFONT *_font;

    Color _tColor = Color(0,0,0);

    bool hit = false;

public:

    inline void set_color(const Color &c){ _color = c; }
    inline const Color & get_color() const { return _color; };

    bool on_touch() override {
        hit = true;
        _tColor = _color;
        _color = _color.dim(.5f);
        return true;
    };

    void touch_end() override {
        if(!hit) return;
        _color = _tColor;
    }

    Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h,const Color &c):
        Control(x,y,x+w, y+h),
        _color(c), _text(nullptr), _font(nullptr){}

    Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h,const Color &c, const char *t, const sFONT *f):
            Control(x,y,x+w, y+h),
            _color(c), _text(t), _font(f){}

    void draw(const LcdScreen &scr) const override;
};

#endif //TOUCHCONTROLLER_GUI_H
