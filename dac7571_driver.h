#ifndef DAC7571_DRIVER_H_
#define DAC7571_DRIVER_H_

#include "utils.h"

void dac7571_write_char(uint8 _char) {
    //uint8 i = 8;
    //for(i = 8; i > 0; --i) {
    //    GPIO_OUT_LOW(3, 2);
    //    _char & 0x80 ? (GPIO_OUT_HIGH(3, 1)) : (GPIO_OUT_LOW(3, 1));
    //    GPIO_OUT_HIGH(3, 2);
    //    _char <<= 1;
    //}

    gpio clk = GPIO_PORT_PIN(3, 2), dat = GPIO_PORT_PIN(3, 1);
    gpio_write_char(_char, &clk, &dat);
}

void dac7571_write_data(const uint16 _data) {
    dac7571_write_char(0x98);
    dac7571_write_char((_data >> 8) & 0x0f);
    dac7571_write_char(_data);
}

#endif
