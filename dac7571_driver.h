#ifndef DAC7571_DRIVER_H_
#define DAC7571_DRIVER_H_

#include "utils.h"

DECLEAR_GPIO_WRITE_CHAR(dac7571, 3, 1, 3, 0)

// wtf???
void dac7571_wait_wtf() {
    GPIO_OUT_LOW(3, 1);
    GPIO_OUT_HIGH(3, 1);
    GPIO_OUT_LOW(3, 1);
}

void dac7571_write_data(const uint16 _data) {
    GPIO_OUT_HIGH(3, 1);
    GPIO_OUT_HIGH(3, 0);
    GPIO_OUT_LOW(3, 0);

    dac7571_write_char(0x98);
    dac7571_wait_wtf();

    dac7571_write_char((_data >> 8) & 0x0f);
    dac7571_wait_wtf();

    dac7571_write_char(_data);
    dac7571_wait_wtf();
}

#endif
