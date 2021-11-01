//
// Created by Leon Boshuizen on 01/11/2021.
//

#ifndef TOUCHCONTROLLER_LCDSCREEN_H
#define TOUCHCONTROLLER_LCDSCREEN_H


#include <LCD_GUI.h>
#include "LcdDriver.h"

class LcdScreen {

private:

    LcdDriver &lcd;

public:

    LcdScreen(LcdDriver& driver): lcd(driver){}

    const uint16_t Width = LCD_2_8_WIDTH;
    const uint16_t Height = LCD_2_8_HEIGHT;

    void clear(COLOR color=BLACK){
        lcd.set_area_color(color, 0,0, Width, Height);
    }

};


#endif //TOUCHCONTROLLER_LCDSCREEN_H
