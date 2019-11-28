#ifndef PAPER_DRIVER_H_
#define PAPER_DRIVER_H_

#include "utils.h"

void paper_write_char(uint8 _char) {
    //uint8 i = 8;
    //for(i = 8; i > 0; --i) {
    //    GPIO_OUT_LOW(2, 7);
    //    _char & 0x80 ? (GPIO_OUT_HIGH(3, 3)) : (GPIO_OUT_LOW(3, 3));
    //    GPIO_OUT_HIGH(2, 7);
    //    _char <<= 1;
    //}
    
    gpio_write_char(_char, GPIO_PORT_PIN(2, 7), GPIO_PORT_PIN(3, 3));
}

void paper_write_address(const uint8 _address) {
    GPIO_OUT_LOW(3, 2);
    GPIO_OUT_LOW(3, 4);
    paper_write_char(_address);
    GPIO_OUT_HIGH(3, 2);
}

void paper_write_data(const uint8 _data) {
    GPIO_OUT_LOW(3, 2);
    GPIO_OUT_HIGH(3, 4);
    paper_write_char(_data);
    GPIO_OUT_HIGH(3, 2);
}

void paper_write(const uint8 _address, const uint8 _data) {
    GPIO_OUT_LOW(3, 2);

    GPIO_OUT_LOW(3, 4);
    paper_write_char(_address);

    GPIO_OUT_HIGH(3, 4);
    paper_write_char(_data);

    GPIO_OUT_HIGH(3, 2);
}

void paper_wait_busy() { while(GPIO_IN_HIGH(2, 2)); }

void paper_init() {
    GPIO_DIR_IN(2, 2);  // BUSY
    GPIO_DIR_OUT(1, 4); // RES
    GPIO_DIR_OUT(3, 4); // D/C
    GPIO_DIR_OUT(3, 2); // CSI
    GPIO_DIR_OUT(2, 7); // SCLK
    GPIO_DIR_OUT(3, 3); // SDI

    GPIO_OUT_LOW(1, 4);
    GPIO_OUT_HIGH(1, 4);
    paper_wait_busy();

    paper_write(0x01, 0xf9);
    paper_write(0x01, 0x00);
    paper_write(0x3a, 0x06);
    paper_write(0x3b, 0x0b);
    paper_write(0x3c, 0x33);
    paper_write(0x11, 0x01);
    paper_write(0x44, 0x00);
    paper_write(0x44, 0x0f);
    paper_write(0x45, 0xf9);
    paper_write(0x45, 0x00);
    paper_write(0x2c, 0x4b);

    paper_write_address(0x32);
    {
        const uint8 lut_data[] = {
            0x50, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x11, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F,
            0x0F, 0x0F, 0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
        };
        uint8 i = 0;
        for(i = 0; i < 29; ++i)
            paper_write_data(lut_data[i]);
    }

    paper_write(0x21, 0x83);
    //paper_write(0x21, 0x03);
    //paper_write(0x3c, 0x73);
}

/**
 *  @brief 
 *  @param _buffer 
 */
void display_img(const uint8 _buffer[]) {
    paper_write(0x4e, 0x00);
    paper_write(0x4f, 0xf9);
    paper_write_address(0x24);
    const uint8 size = 250 * 16;
    uint8 cnt = size;
    for(cnt = size; cnt; --cnt)
        paper_write_data(_buffer[size - cnt]);
    paper_write(0x22, 0xc7);
    paper_wait_busy();
}

#endif
