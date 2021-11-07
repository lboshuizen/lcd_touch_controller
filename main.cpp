#include "main.h"   //Examples
#include "LcdDriver.h"
#include "LcdScreen.h"
#include "Gui.h"

#include <exception>
#include <functional>

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

[[ noreturn ]]
void blink(const std::function<void()> &cb){
    Pin led(25,GPIO_OUT);

    while(1) {
        led.up();
        sleep_ms(500);
        cb();
        led.down();
        sleep_ms(500);
        cb();
    }
}

int main(void)
{
    stdio_init_all();

    LcdDriver lcd;
    lcd.init(static_cast<LCD_SCAN_DIR>(5));

    LcdScreen screen(lcd);
    screen.clear(0x0101);

    Button c1(10,10,50,50,RED);
    Button c2(80,10,50,50,RED, "Zzz", &Font16);
    //Button c2(10,10,50,50,RED);

    Drawable* c[] { &c1, &c2 };

    View v1( c, 2 );
    v1.draw(screen);

    std::function<void()> ontick = [&c2, v1, screen](){
        auto c = c2.get_color();
        c2.set_color(~c);
        v1.draw( screen );
    };

    blink( ontick );
}
