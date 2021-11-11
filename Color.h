//
// Created by Leon Boshuizen on 10/11/2021.
//

#ifndef TOUCHCONTROLLER_COLOR_H
#define TOUCHCONTROLLER_COLOR_H

#include "stdint.h"

class Point{
private:

    uint16_t _x,_y;

public:

    Point(uint16_t x, uint16_t y): _x(x), _y(y){}

    [[nodiscard]] Point flip() const{ return {_y,_x}; }

    [[nodiscard]] inline uint16_t X() const { return _x;};
    [[nodiscard]] inline uint16_t Y() const { return _y;};

    [[nodiscard]] Point shiftx(uint16_t x) const { return shift(x,0); }
    [[nodiscard]] Point shifty(uint16_t y) const { return shift(0,y); }
    [[nodiscard]] Point shift(uint16_t x, uint16_t y) const {return Point(_x+x,_y+y); }
};

class Rect {

public:
    Rect(const Point &tl, const Point& br): TL(tl), BR(br){};
    Rect(const Point &tl, uint16_t w, uint16_t h): TL(tl), BR(Point(tl.X() + w, tl.Y() + h)){};

    const Point &TL;
    const Point BR;

    [[nodiscard]] inline bool covers(const Point& p) const { return p.X() >= TL.X() && p.X() <= BR.X() && p.Y() >= TL.Y() && p.Y() <= BR.Y(); }
};

class Color {
private:

    uint8_t red;
    uint8_t green;
    uint8_t blue;

    [[nodiscard]] static inline uint8_t mult(uint8_t c, float f) { return (uint8_t)  ((float) c * f); }

public:
    Color(uint8_t r, uint8_t g, uint8_t b): red(r), blue(b), green(g){}

    Color(const Color& c){
        red = c.red;
        blue = c.blue;
        green = c.green;
    }

    [[nodiscard]] Color dim(float f) const {
        return {mult(red,f), mult(green,f), mult(blue,f) };
    }

    explicit operator uint16_t() const {return (red & 0x1f) << 11 | (green & 0x3f) << 5 | (blue & 0x1f); }
};

extern const Color Black;
extern const Color White;
extern const Color Red;
extern const Color Green;
extern const Color Blue;

#endif //TOUCHCONTROLLER_COLOR_H
