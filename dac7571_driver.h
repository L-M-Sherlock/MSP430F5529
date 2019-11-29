#ifndef DAC7571_DRIVER_H_
#define DAC7571_DRIVER_H_

#include "utils.h"

void dac7571_write_char(const uint8 _char) {
    gpio clk = GPIO_OUT(3, 1), dat = GPIO_OUT(3, 0);
    gpio_write_char(_char, &clk, &dat);
}

void dac7571_write_data(const uint16 _data) {
    dac7571_write_char(0x98);
    dac7571_write_char((_data >> 8) & 0x0f);
    dac7571_write_char(_data);
}

#endif
