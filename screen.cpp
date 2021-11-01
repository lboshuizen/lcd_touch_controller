//
// Created by Leon Boshuizen on 14/09/2021.
//

#include "screen.h"

static void T(){
    TouchPanel p;
    Lcd s;

    Display d(p,s );

    d.panel.Scan();
}

Rect Drawable::BoundingBox(const std::vector<Drawable>& items) {
    if(items.empty())
        return Rect{Point{0, 0}, Point{0, 0}};

    auto f = items.front();
    Point tl = {f._rect.tl};
    Point br = {f._rect.br};

    for (auto & item : items) {
        if (tl.x > item._rect.tl.x) tl.x = item._rect.tl.x;
        if (br.x < item._rect.br.x) br.x = item._rect.br.x;
        if (tl.y > item._rect.tl.y) tl.y = item._rect.tl.y;
        if (br.y < item._rect.br.y) br.y = item._rect.br.y;
    }

    return Rect{tl, br};
}

bool Drawable::Touched(const Point &p) {

    auto hit = p.x >= _rect.tl.x && p.x <= _rect.br.x &&
               p.y >= _rect.tl.y && p.y <= _rect.br.y;

    if(!hit) return hit;

    for (auto & child : children){
        if( child.Touched(p) ) return true;
    }

    if(!onTouch){
        return false;
    }

    onTouch(_rect);
    return true;
}
