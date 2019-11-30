#include "sdcard_driver.h"
#include "dac7571_driver.h"

#define WAV_BUFF_LEN (2048)
uint8 wav_buff[WAV_BUFF_LEN];

enum {
    play,
    pause,
    stop,
} state;

void play_wav(uint16 _chunk_size) {
    uint16 cnt = 0;
    for(; cnt < _chunk_size; ++cnt) {
        uint16 data = wav_buff[cnt];
        dac7571_write_data(data << 3);
        while(state == pause);
        __delay_cycles(2000);
    }
}

void btn_init() {
    P1DIR &= ~(BIT2 | BIT3);
    P1DIR |= BIT2 | BIT3;
    P1OUT |= BIT2 | BIT3;
    P1IE |= BIT2 | BIT3;
    P1IES |= BIT2 | BIT3;
}

int main() {
    state = stop;

    WDTCTL = WDTPW + WDTHOLD;

    __enable_interrupt();

    btn_init();
    sdcard_init();

    while(1) {
        if(state == stop)
            continue;

        sdcard_load_wav(wav_buff, sizeof(wav_buff), play_wav);
        state = stop;
    }

    return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void onBtnDown() {
    if(P1IFG & BIT3) {
        // S2 暂停
        if(state == play)
            state = pause;
    }
    else if(P1IFG & BIT2) {
        // S1 播放
        state = play;
    }

    P1IFG = 0;
}
