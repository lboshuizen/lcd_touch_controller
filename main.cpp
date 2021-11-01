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
    screen.point(10,10,RED, DOT_PIXEL_2X2 , DOT_FILL_AROUND);
    screen.point(230,310,YELLOW, DOT_PIXEL_2X2 , DOT_FILL_AROUND);

    screen.line(101,101,200,200,BLUE, LINE_DOTTED, DOT_PIXEL_2X2);
    screen.circle(101,101,40,WHITE,LINE_DOTTED,DOT_PIXEL_1X1);

    screen.rect(10,200,230, 300,YELLOW, LINE_DOTTED, DOT_PIXEL_1X1);

    screen.text(30,205,"Leon",RED, WHITE, &Font20);

    Pin led(25,GPIO_OUT);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while(1) {
        led.up();
        sleep_ms(500);
        led.down();
        sleep_ms(500);
    }
#pragma clang diagnostic pop

    return 0;
}
