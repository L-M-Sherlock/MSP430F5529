#include "max7219_driver.h"
#include "state_machine.h"

#define FRAME_NUM 2

enum State state;

const uint8 frames[FRAME_NUM][8] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

const uint32 delay[FRAME_NUM] = {
    1,
    20,
};

int main_led_display() {
    WDTCTL = WDTPW + WDTHOLD;

    __enable_interrupt();
    state_init();
    btn_init();
    max7219_init();

    while(1) {
        if(state == stop)
            continue;

        max7219_write_animation(frames, delay, FRAME_NUM);

        state = stop;
    }

    return 0;
}

#pragma vector = PORT1_VECTOR
DECLEAR_STATE_CALLBACK
