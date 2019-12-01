#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "utils.h"

enum State {
    stop,
    play,
    pause,
};

extern enum State state;

void state_init() {
    state = stop;
}

void delay_ms(uint32 _ms) {
    while(_ms--) {
        while(state == pause);
        __delay_cycles(1000);
    }
}

void btn_init() {
    P1DIR &= ~(BIT2 | BIT3);
    P1DIR |= BIT2 | BIT3;
    P1OUT |= BIT2 | BIT3;
    P1IE |= BIT2 | BIT3;
    P1IES |= BIT2 | BIT3;
}

#define DECLEAR_STATE_CALLBACK \
__interrupt void onBtnDown() { \
    if(P1IFG & BIT3) { \
        if(state == play) \
            state = pause; \
    } \
    else if(P1IFG & BIT2) { \
        state = play; \
    } \
    P1IFG = 0; \
}

#endif
