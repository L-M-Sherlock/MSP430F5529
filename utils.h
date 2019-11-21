#ifndef UTILS_H_
#define UTILS_H_

#include <msp430f5529.h>

#define GPIO_DIR_OUT(port, pin) P##port##DIR |= BIT##pin
#define GPIO_DIR_IN(port, pin) P##port##DIR &= ~BIT##pin
#define GPIO_OUT_HIGH(port, pin) P##port##OUT |= BIT##pin
#define GPIO_OUT_LOW(port, pin) P##port##OUT &= ~BIT##pin
#define GPIO_IN_HIGH(port, pin) (P##port##IN & BIT##pin)
#define GPIO_IN_LOW(port, pin) !(P##port##IN & BIT##pin)

typedef unsigned char uint8;
typedef unsigned uint16;
typedef unsigned long uint32;

void delay_for(uint32 _delay) {
    while(--_delay)
        __no_operation();
}

#endif
