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

    Rect    r;
    bool _visible;

private:

public:
    Drawable(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2):
        r({x1,y1},{x2,y2}),
        _visible(true)
        {};

    Drawable(const Rect &rect):
            r(rect),
            _visible(true)
    {};

    bool touch_start(const Point &p){
        if( !r.covers(p) ) return false;

        on_touch();
        return true;
    }

    virtual void touch_end(){}

    virtual bool on_touch(){ return false; };

    virtual void draw(const LcdScreen &scr) const = 0;

    [[nodiscard]] inline bool IsVisible() const { return _visible; };
    inline void visible(bool f){ _visible = f;};
};

class RectArea : Drawable {
};

class Text : Drawable {

};

class Circle : Drawable {

};

class Control : public Drawable {

    bool _enabled;

public:
    Control(const Rect &rect) :
    Drawable(rect),
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

    Button(const Rect &r,const Color &c):
        Control(r),
        _color(c), _text(nullptr), _font(nullptr){}

    Button(const Rect &r,const Color &c, const char *t, const sFONT *f):
            Control(r),
            _color(c), _text(t), _font(f){}

    void draw(const LcdScreen &scr) const override;
};

#endif //TOUCHCONTROLLER_GUI_H
