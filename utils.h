#ifndef UTILS_H_
#define UTILS_H_

#include <msp430f5529.h>

typedef unsigned char uint8;
typedef unsigned uint16;
typedef unsigned long uint32;

#define GPIO_DIR_OUT(port, pin) P##port##DIR |= BIT##pin
#define GPIO_DIR_IN(port, pin) P##port##DIR &= ~BIT##pin
#define GPIO_OUT_HIGH(port, pin) P##port##OUT |= BIT##pin
#define GPIO_OUT_LOW(port, pin) P##port##OUT &= ~BIT##pin
#define GPIO_IN_HIGH(port, pin) (P##port##IN & BIT##pin)
#define GPIO_IN_LOW(port, pin) !(P##port##IN & BIT##pin)

#define DECLEAR_GPIO_WRITE_CHAR(moudle_name, clk_port, clk_pin, dat_port, dat_pin) \
void moudle_name##_write_char(uint8 _char) { \
    uint8 i = 8; \
    for(; i; --i) { \
        GPIO_OUT_LOW(clk_port, clk_pin); \
        _char & 0x80 ? (GPIO_OUT_HIGH(dat_port, dat_pin)) : (GPIO_OUT_LOW(dat_port, dat_pin)); \
        GPIO_OUT_HIGH(clk_port, clk_pin); \
        _char <<= 1; \
    } \
}

/** @note 不初始化时钟，默认配置下适用。 */
void delay_ms_raw(uint32 _ms) {
    while(_ms--)
        __delay_cycles(1000);
}

#endif
