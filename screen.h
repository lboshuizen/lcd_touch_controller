//
// Created by Leon Boshuizen on 14/09/2021.
//

#ifndef TOUCHCONTROLLER_SCREEN_H
#define TOUCHCONTROLLER_SCREEN_H

#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#include <LCD_Touch.h>

struct Point {
    uint16_t x;
    uint16_t y;
};

struct Rect {
    Rect &operator=(Rect r) {
        tl = r.tl;
        br = r.br;
        return *this;
    }

    Point tl;
    Point br;
};

typedef void (*TouchCallback)(Rect &);

static TouchCallback _dummy = [](Rect &_) -> void {};

struct Style {
    COLOR fill;

};

class Drawable {

private:
    TouchCallback onTouch;
    Rect _rect;
    Style _style;

    std::vector<Drawable> &children;

    static std::vector<Drawable> empty;

public:

    static Rect BoundingBox(const std::vector<Drawable> &children);

    explicit Drawable(Rect &r, Style &s, TouchCallback cb = nullptr) : _rect(r), _style(s), onTouch(cb), children(empty) {}

    explicit Drawable(std::vector<Drawable> &c, TouchCallback cb = nullptr) : children(c), _rect(BoundingBox(c)), onTouch(cb) {}

    bool Touched(const Point &p);

    void Draw() { GUI_DrawRectangle(_rect.tl.x, _rect.tl.y, _rect.br.x, _rect.br.y, BLUE, DRAW_FULL, DOT_PIXEL_DFT); }
};

class TouchPanel {
public:

    void Scan();

    void Init(LCD_SCAN_DIR scanDir) { TP_Init(scanDir); }
};

struct Screen {
    std::vector<Drawable> items;
};

class Lcd {
public:
    void Init(LCD_SCAN_DIR scanDir) { LCD_Init(scanDir); }
};

class Display {

public:

    Display(TouchPanel &panel_, Lcd scr) : panel(panel_), screen(scr) {}

    TouchPanel &panel;
    Lcd &screen;

    void Paint(std::vector<Drawable> screen) { for (auto &item: screen) { item.Draw(); }}

};


#endif //TOUCHCONTROLLER_SCREEN_H
