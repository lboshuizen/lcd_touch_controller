//
// Created by Leon Boshuizen on 01/11/2021.
//

#include "LcdDriver.h"

uint8_t LcdDriver::spi_write(uint8_t value)
{
    uint8_t rxDat;
    spi_write_read_blocking(spi1,&value,&rxDat,1);
    return rxDat;
}

uint8_t LcdDriver::spi_read(uint8_t value)
{
    return spi_write(value);
}

void LcdDriver::write_parameter(uint8_t data){
    DC.up();
    CS.down();
    spi_write( data );
    CS.up();
}

void LcdDriver::write_reg(uint8_t reg){
    DC.down();
    CS.down();
    spi_write( reg );
    CS.up();
}

void LcdDriver::write_all_data(uint16_t data, uint32_t len){
    DC.up();
    CS.down();

    for(auto i = 0; i < len; i++) {
        spi_write(data >> 8);
        spi_write(data & 0XFF);
    }

    CS.up();
}

void LcdDriver::set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    write_reg(0x2A); // Column Address Set
    write_parameter(x1 >> 8);	 		//Set the horizontal starting point to the high octet
    write_parameter(x1 & 0xff);	 	//Set the horizontal starting point to the low octet
    write_parameter((x2 - 1) >> 8);	//Set the horizontal end to the high octet
    write_parameter((x2 - 1) & 0xff);	//Set the horizontal end to the low octet

    //set the Y coordinates
    write_reg(0x2B); // Row Address Set
    write_parameter(y1 >> 8);
    write_parameter(y1 & 0xff);
    write_parameter((y2 - 1) >> 8);
    write_parameter((y2 - 1) & 0xff);

    write_reg(0x2C); // Memory Write
}

void LcdDriver::set_area_color(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    if( (x2 > x1) && (y2 > y1)){
        set_window(x1,y1,x2,y2);
        set_color(color, x2-x1, y2-y1);
    }
}

void LcdDriver::set_reg(uint8_t reg, uint8_t data){
    write_reg( reg );
    write_parameter(data);
}

void LcdDriver::init(LCD_SCAN_DIR scanDir){
    spi_init(spi1, 4000000);
    gpio_set_function(LCD_CLK_PIN,GPIO_FUNC_SPI);
    gpio_set_function(LCD_MOSI_PIN,GPIO_FUNC_SPI);
    gpio_set_function(LCD_MISO_PIN,GPIO_FUNC_SPI);

    Pin(LCD_BKL_PIN, GPIO_OUT).up();
    CS.up();

    reset();
    init_reg();

    set_scandir(scanDir);

    sleep_ms(200);
}


void LcdDriver::init_reg() {
    write_reg(0x11); // Sleep OUT
    sleep_ms(100);

    set_reg(0x36, 0x00); // Memory Access Control - aka scandir R2L-T2B
    set_reg(0x3a, 0x55); // COLMOD: 65k RGB-Interface, 16bit/pixel

    write_reg(0xb2); // Porch Settting
    write_parameter(0x0c);
    write_parameter(0x0c);
    write_parameter(0x00);
    write_parameter(0x33);
    write_parameter(0x33);

    write_reg(0xb7); // Gate control
    write_parameter(0x35); // VGH:13.26, VGL:-10.43

    write_reg(0xbb); // VCOM
    write_parameter(0x28); // 1.1V

    write_reg(0xc0); // LCM Control
    write_parameter(0x3c);

    write_reg(0xc2); // VDV and VRH Command Enable
    write_parameter(0x01); // VDV and VRH register value comes from command write
    write_parameter(0xff); // ignored

    write_reg(0xc3); // VRH Set
    write_parameter(0x0b); // 4.1+( vcom+vcom offset+vdv)

    write_reg(0xc4); // VDV Set
    write_parameter(0x20); // 0V

    write_reg(0xc6); // Frame Rate Control in Normal Mode
    write_parameter(0x0f); // 60Hz

    write_reg(0xD0); // Power Control 1
    write_parameter(0xa4);
    write_parameter(0xa1);

    write_reg(0xe0); // Positive Voltage Gamma Control
    write_parameter(0xd0);
    write_parameter(0x01);
    write_parameter(0x08);
    write_parameter(0x0f);
    write_parameter(0x11);
    write_parameter(0x2a);
    write_parameter(0x36);
    write_parameter(0x55);
    write_parameter(0x44);
    write_parameter(0x3a);
    write_parameter(0x0b);
    write_parameter(0x06);
    write_parameter(0x11);
    write_parameter(0x20);

    write_reg(0xe1); // Negative Voltage Gamma Control
    write_parameter(0xd0);
    write_parameter(0x02);
    write_parameter(0x07);
    write_parameter(0x0a);
    write_parameter(0x0b);
    write_parameter(0x18);
    write_parameter(0x34);
    write_parameter(0x43);
    write_parameter(0x4a);
    write_parameter(0x2b);
    write_parameter(0x1b);
    write_parameter(0x1c);
    write_parameter(0x22);
    write_parameter(0x1f);

    write_reg(0x55); // Write Content Adaptive Brightness Control and Color Enhancement
    write_parameter(0xB0); // Color Enhancement On, High enhancement, content based adaptive brightness control: OFF

    write_reg(0x29); // Memory Write
}
