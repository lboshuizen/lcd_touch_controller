#include "main.h"   //Examples
#include "LcdDriver.h"
#include "LcdScreen.h"

int main(void)
{
    stdio_init_all();

    LcdDriver lcd;
    lcd.init();

    LcdScreen screen(lcd);
    screen.clear(0x0101);

    Pin led(25,GPIO_OUT);

    while(1) {
        led.up();
        sleep_ms(500);
        led.down();
        sleep_ms(500);
    }

    return 0;
}
