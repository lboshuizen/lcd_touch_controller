//
// Created by Leon Boshuizen on 10/11/2021.
//

#ifndef TOUCHCONTROLLER_VIEW_H
#define TOUCHCONTROLLER_VIEW_H


#include "Gui.h"

class View {

private:
    Drawable **_controls;
    size_t   _cnt;

    bool in_touch = false;

public:

    explicit View(Drawable *c[], size_t l): _controls(c), _cnt(l){};

    void draw(const LcdScreen &scr) const {
        for(uint i=0; i<_cnt; i++ ){
            auto c = _controls[i];
            if( c->IsVisible()) {
                c->draw(scr);
            }
        }
    }

    void touch(const Point &p){
        if( in_touch ) return;
        in_touch = true;

        for(auto i=0; i<_cnt;i++){
            _controls[i]->touch_start(p);
        }
    }

    void touch_done(){
        if( !in_touch) return;
        in_touch = false;

        for(auto i=0; i<_cnt;i++){
            _controls[i]->touch_end();
        }
    }

};

#endif //TOUCHCONTROLLER_VIEW_H
