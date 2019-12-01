#ifndef MAX7219_DRIVER_H_
#define MAX7219_DRIVER_H_

#include "utils.h"
#include "state_machine.h"

DECLEAR_GPIO_WRITE_CHAR(max7219, 6, 3, 6, 1)

void max7219_write(const uint8 _address, const uint8 _data) {
    GPIO_OUT_LOW(6, 2);
    max7219_write_char(_address);
    max7219_write_char(_data);
    GPIO_OUT_HIGH(6, 2);
}

void max7219_io_init() {
    // P6.1 -> DIN
    GPIO_DIR_OUT(6, 1);
    GPIO_OUT_LOW(6, 1);

    // P6.2 -> CS
    GPIO_DIR_OUT(6, 2);
    GPIO_OUT_HIGH(6, 2); // low enable

    // P6.3 -> CLK
    GPIO_DIR_OUT(6, 3);
    GPIO_OUT_HIGH(6, 3); // rising edge is active
}

void max7219_init() {
    max7219_io_init();

    max7219_write(0x09, 0x00); // bcd
    max7219_write(0x0a, 0x00); // intensity dimmest
    max7219_write(0x0b, 0x07); // scan limit
    max7219_write(0x0c, 0x01); // normal
    max7219_write(0x0f, 0x00); // quit display test
}

void max7219_write_animation(const uint8 _frames[][8], const uint32 _delay[], const uint16 _frame_count) {
    uint16 frame_idx = 0;
    uint8 byte = 0;
    for(frame_idx = 0; frame_idx < _frame_count; ++frame_idx) {
        for(byte = 0; byte < 8; ++byte)
            max7219_write(byte + 1, _frames[frame_idx][byte]);
        delay_ms(_delay[frame_idx]);
    }
}

#endif
