#include "sdcard_driver.h"
#include "dac7571_driver.h"
#include "state_machine.h"

#define WAV_BUFF_LEN (2048)
uint8 wav_buff[WAV_BUFF_LEN];

enum State state;

void play_wav(uint16 _chunk_size) {
    uint16 cnt = 0;
    for(; cnt < _chunk_size; ++cnt) {
        uint16 data = wav_buff[cnt];

        while(state == pause);

        dac7571_write_data(data << 3);
        __delay_cycles(1800); // magic
    }
}

int main_wav_playback() {
    WDTCTL = WDTPW + WDTHOLD;

    __enable_interrupt();
    state_init();
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
DECLEAR_STATE_CALLBACK
