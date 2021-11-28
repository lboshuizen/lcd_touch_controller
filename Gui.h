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

    const Rect &r;
    bool _visible = true;
    bool _enabled = false;

private:

public:

    explicit Drawable(const Rect &rect) : r(rect) {};

    void touch_start(const Point &p) {
        if (!r.covers(p)) return;

        on_touch();
    }

    virtual void touch_end() {}

    virtual void on_touch() { return; };

    virtual void draw(const LcdScreen &scr) const = 0;

    [[nodiscard]] inline bool IsVisible() const { return _visible; };

    inline void visible(bool f) { _visible = f; };

    inline bool IsEnabled() const { return _enabled; };

    inline void disable() { enable(false); }

    inline void enable(bool f = true) { _enabled = f; };
};

class RectArea : Drawable {
};

class Text : Drawable {
};

class Circle : Drawable {
};

class Button : public Drawable {
private:
    Color _color;
    Color _dimmed = Color(0, 0, 0);

    const char *_text;
    const sFONT *_font;

    bool _touched = false;

public:

    inline void set_color(const Color &c) { _color = c; }

    inline const Color &get_color() const { return _color; };

    void on_touch() override {
        _touched = true;
    };

    void touch_end() override {
        _touched = false;
    }

    Button(const Rect &r, const Color &c) :
            Drawable(r),
            _color(c), _dimmed(c.dim(.5)), _text(nullptr), _font(nullptr) {
        _enabled = true;
    }

    Button(const Rect &r, const Color &c, const char *t, const sFONT *f) :
            Drawable(r),
            _color(c), _text(t), _font(f) {
        _enabled = true;
    }

    void draw(const LcdScreen &scr) const override {
        const Color &rc = (!_touched && _enabled) ? _color : _dimmed;
        scr.rect(r.TL, r.BR, rc, LINE_SOLID, DOT_PIXEL_1X1);
        if (_text == nullptr) return;
        scr.text(r.TL.shift(5, 5 + _font->Height), _text, rc, scr.background(), _font);
    };
};

#endif //TOUCHCONTROLLER_GUI_H
