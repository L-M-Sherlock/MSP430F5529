#ifndef PAPER_DRIVER_H_
#define PAPER_DRIVER_H_

#include "utils.h"

void paper_write_char(uint8 _char) {
    uint8 i = 8;
    for(i = 8; i > 0; --i) {
        GPIO_OUT_LOW(2, 7);
        _char & 0x80 ? (GPIO_OUT_HIGH(3, 3)) : (GPIO_OUT_LOW(3, 3));
        GPIO_OUT_HIGH(2, 7);
        _char <<= 1;
    }
}

void paper_write(const uint8 _address, const uint8 _data) {
    GPIO_OUT_LOW(3, 2);

    GPIO_OUT_LOW(3, 4);
    paper_write_char(_address);

    GPIO_OUT_HIGH(3, 4);
    paper_write_char(_data);

    GPIO_OUT_HIGH(3, 2);
}

void paper_init() {
    GPIO_DIR_IN(2, 2);  // BUSY
    GPIO_DIR_OUT(1, 4); // RES
    GPIO_DIR_OUT(3, 4); // D/C
    GPIO_DIR_OUT(3, 2); // CSI
    GPIO_DIR_OUT(2, 7); // SCLK
    GPIO_DIR_OUT(3, 3); // SDI

    GPIO_OUT_LOW(1, 4);
    GPIO_OUT_HIGH(1, 4);
    while(GPIO_IN_HIGH(2, 2)); // wait busy

    paper_write(0x01, 0xf9);
    paper_write(0x01, 0x00);
    paper_write(0x3a, 0x06);
    paper_write(0x3b, 0x0b);

    // todo
}

#endif
