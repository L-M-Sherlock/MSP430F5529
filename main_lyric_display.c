#include "paper_driver.h"
#include "img_data.h"
#include "state_machine.h"

#define IMG_NUM 2

enum State state;

const uint8* img[IMG_NUM] = {
    g_image_1,
    g_image_2,
};

const uint32 delay[IMG_NUM] = {
    1,
    20,
};

int main_lyric_display() {
    WDTCTL = WDTPW + WDTHOLD;

    __enable_interrupt();
    state_init();
    btn_init();
    paper_init();

    paper_display_mono(0xff);

    while(1) {
        if(state == stop)
            continue;

        uint8 idx = 0;
        for(; idx < IMG_NUM; ++idx) {
            paper_display_img(img[idx]);
            delay_ms(delay[idx]);
        }

        state = stop;
        paper_display_mono(0xff);
    }

    return 0;
}

#pragma vector = PORT1_VECTOR
DECLEAR_STATE_CALLBACK
