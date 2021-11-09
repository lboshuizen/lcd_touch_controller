// #include "main.h"   //Examples
#include "LcdDriver.h"
#include "LcdScreen.h"
#include "Gui.h"
#include "Spi.h"
#include "TouchDriver.h"

#include <exception>
#include <functional>
#include <string>

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

void scan(const std::function<void()> &cb){
    Pin led(25,GPIO_OUT);
    led.up();

    while(1) {
        sleep_ms(10);
        cb();
    }
}

void itoa(uint x, char *p){
    int digit=0;
    char num[12];

    if( x == 0){
        *p = '0';
        *(p+1)= 0;
        return;
    }

    while (x) {
        num[digit] = x % 10 + '0';
        digit++;
        x /= 10;
    }

    //The string is inverted
    while (digit > 0) {
        *p = num[digit - 1];
        p++;
        digit--;
    }
    *p = 0;
}

int main(void)
{
    // stdio_init_all();

    Spi Spi1(spi1, 18000000);
    Spi1.init();

    LcdDriver lcd(Spi1);
    lcd.init(static_cast<LCD_SCAN_DIR>(5));

    LcdScreen screen(lcd);
    screen.clear();

    TouchDriver td(Spi1);
    td.init();

    Button c1(10,10,50,50,GREEN);
    Button c2(80,10,50,50,RED, "Zzz", &Font16);
    //Button c2(10,10,50,50,RED);

    Drawable* c[] { &c1, &c2 };

    View v1( c, 2 );
    v1.draw(screen);

    std::function<void()> ontick = [&c2, v1, &screen, &td](){
        COLOR c = td.scan(screen) ? GREEN : RED;

        char n[12];
        itoa(td._x, n);
        screen.text(40,100, n, WHITE, BLACK, &Font16);
        itoa(td._y, n);
        screen.text(40,120, n, WHITE, BLACK, &Font16);

        c2.set_color(c);
        v1.draw( screen );
    };

    scan(ontick);
}
