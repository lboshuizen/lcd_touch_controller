// #include "main.h"   //Examples
#include "LcdDriver.h"
#include "LcdScreen.h"
#include "Gui.h"
#include "Spi.h"
#include "TouchDriver.h"
#include "Color.h"
#include "View.h"
#include "esp8266_01.h"

#include <exception>
#include <functional>
#include <string>
#include <pico/stdio.h>

namespace std {

    void __throw_length_error(char const *) {
        while (1){}
    }

    void __throw_bad_alloc(){
        while (1){}
    }

    void __throw_bad_function_call(){
        while (1);
    }

}

extern "C" void __cxa_pure_virtual()
{
    while (1);
}

[[noreturn]]
void loop(const std::function<void()> &cb){
    static bool toggle = false;
    //Pin led(25,GPIO_OUT);

    while(1) {
        cb();
        sleep_ms(100);
        // toggle = !toggle;
        // led.value( toggle );
    }
}

const Color red50 = Red.dim(.5);

void init(){
}

int main(void)
{
    stdio_init_all();

    sleep_ms(4000);

    esp8266_01 wifi;
    wifi.init();

    while(1){
        sleep_ms(1000);
        wifi.check();
     }

    return 0;

    Spi Spi1(spi1, 18000000);
    Spi1.init();

    LcdDriver lcd(Spi1);
    lcd.init(static_cast<LCD_SCAN_DIR>(5));

    LcdScreen screen(lcd);

    TouchDriver td(Spi1);
    td.init();

    screen.clear(Black);

    // Woonkamer
    Button c1({{80, 10}, 75, 50}, Green, "Tuin", &Font12 );
    Button c2({{160, 10}, 75, 50}, Green, "Eettafel", &Font12 );
    Button c3({{240, 10}, 75, 50}, Green, "Plafond", &Font12 );
    Button c4({{80, 65}, 75, 50}, Green, "Lamp\nLinks", &Font12 );
    Button c5({{160, 65}, 75, 50}, Green, "Lamp\nRechts", &Font12);
    Button c6({{80, 130}, 150, 50}, Red, "Zzz", &Font12);
    Drawable* c[] { &c1, &c2 , &c3, &c4, &c5, &c6 };
    View v1( c, 6 );

    v1.draw(screen);

    std::function<void()> ontick = [&](){
        const Point *p = td.scan(screen);

        if(p) {
            v1.touch(*p);
//            screen.point(p->X(),p->Y(), White, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
        }
        else{
            v1.touch_done();
        }
        v1.draw( screen );
    };

    loop(ontick);
}
