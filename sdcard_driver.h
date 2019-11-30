#ifndef SDCARD_DRIVER_H_
#define SDCARD_DRIVER_H_

#include "utils.h"

#include "dr_sdcard/HAL_SDCard.h"
#include "dr_sdcard/ff.h"

void sdcard_init_clk() {
    P5SEL |= BIT4 + BIT5; // 选择 XT1
    UCSCTL6 &= ~(XT1OFF); // XT1 开启
    UCSCTL6 |= XCAP_3;    // Internal load cap
    P5SEL |= BIT2 + BIT3; //XT2引脚功能选择
    UCSCTL6 &= ~XT2OFF;   //启动XT2
    while(SFRIFG1 & OFIFG) {
        //等待XT1、XT2与DCO稳定
        UCSCTL7 &= ~(DCOFFG + XT1LFOFFG + XT2OFFG);
        SFRIFG1 &= ~OFIFG;
    }

    UCSCTL4 = SELA__XT1CLK + SELS__XT2CLK + SELM__XT2CLK; //避免DCO调整中跑飞

    UCSCTL1 = DCORSEL_5;                     //6000kHz~23.7MHz
    UCSCTL2 = 20000000 / (4000000 / 8);      //XT2频率较高，分频后作为基准可获得更高的精度
    UCSCTL3 = SELREF__XT2CLK + FLLREFDIV__8; //XT2进行16分频后作为基准
    while(SFRIFG1 & OFIFG) {
        //等待XT1、XT2与DCO稳定
        UCSCTL7 &= ~(DCOFFG + XT1LFOFFG + XT2OFFG);
        SFRIFG1 &= ~OFIFG;
    }
    UCSCTL5 = DIVA__1 + DIVS__1 + DIVM__1;                //设定几个CLK的分频
    UCSCTL4 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK; //设定几个CLK的时钟源
}

void sdcard_io_init() {
    P3DIR |= BIT1 + BIT0;
    P3REN |= BIT1 + BIT0;
    P3OUT |= BIT1 + BIT0;

    P8DIR |= BIT2;
    P8REN |= BIT2;
    P8OUT &= ~BIT2;

    P1DIR |= BIT4;
    P1OUT |= BIT4;

    P2DIR |= BIT1 + BIT7;
    P2REN |= BIT1 + BIT7;
    P2OUT |= BIT1 + BIT7;

    P2DIR |= BIT0;
    P2OUT |= BIT0;

    P3DIR |= BIT2 + BIT3 + BIT4;
    P3REN |= BIT2 + BIT3 + BIT4;
    P3OUT |= BIT2 + BIT3 + BIT4;

    P2DIR &= ~(BIT3 + BIT6);
    P2REN |= BIT3 + BIT6;
    P2OUT |= BIT3 + BIT6;
}

void sdcard_init_lower() {
    SDCard_init();
}

void sdcard_init() {
    sdcard_init_clk();
    sdcard_io_init();
    sdcard_init_lower();
}

/** @note 根目录下应只有一个 wav 文件 */
void sdcard_load_wav(uint8* _buffer, uint16 _buf_size, void (*_callback)(uint16 _real_size)) {
    FATFS fatfs;      // file sys obj
    DIRS dirs;        // directory obj
    FILINFO fil_info; // file info obj
    FIL fil;          // file obj

    f_mount(0, &fatfs);
    f_opendir(&dirs, (TCHAR*)"");
    f_readdir(&dirs, &fil_info); // the first one is system file
    f_readdir(&dirs, &fil_info); // the second one is wav
    f_open(&fil, fil_info.fname, FA_OPEN_EXISTING | FA_READ);
    UINT br;
    while(1) {
        f_read(&fil, _buffer, _buf_size, &br);
        if(!br)
            break;
        _callback(br);
    }
    f_close(&fil);
}

#endif
