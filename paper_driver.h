#ifndef PAPER_DRIVER_H_
#define PAPER_DRIVER_H_

#include "utils.h"
#include <stdarg.h>

void paper_write_char(const uint8 _char) {
    gpio clk = GPIO_OUT(2, 7), dat = GPIO_OUT(3, 3);
    gpio_write_char(_char, &clk, &dat);
}

void paper_write_address(const uint8 _address) {
    GPIO_OUT_LOW(3, 2);
    GPIO_OUT_LOW(3, 4);
    paper_write_char(_address);
    GPIO_OUT_HIGH(3, 2);
}

void paper_write_data(const uint8 _data) {
    GPIO_OUT_LOW(3, 2);
    GPIO_OUT_HIGH(3, 4);
    paper_write_char(_data);
    GPIO_OUT_HIGH(3, 2);
}

void paper_write(const uint8 _address, const uint8 _dat_num, ...) {
    paper_write_address(_address);
    va_list valist;
    va_start(valist, _dat_num);
    uint8 i = _dat_num;
    for(i = _dat_num; i; --i)
        paper_write_data(va_arg(valist, uint8));
    va_end(valist);
}

void paper_write_uint8(const uint8 _address, const uint8 _data) {
    paper_write_address(_address);
    paper_write_data(_data);
}

void paper_write_uint16(const uint8 _address, const uint16 _data) {
    paper_write_address(_address);
    paper_write_data(_data >> 8);
    paper_write_data(_data);
}

void paper_wait_busy() { while(GPIO_IN_HIGH(2, 2)); }

void paper_init() {
    GPIO_DIR_IN(2, 2);  // BUSY
    GPIO_DIR_OUT(1, 4); // RES
    GPIO_DIR_OUT(3, 4); // D/C
    GPIO_DIR_OUT(3, 2); // CSI
    GPIO_DIR_OUT(2, 7); // SCLK
    GPIO_DIR_OUT(3, 3); // SDI

    // reset
    GPIO_OUT_LOW(1, 4);
    GPIO_OUT_HIGH(1, 4);
    paper_wait_busy();

    GPIO_OUT_HIGH(3, 2);
    GPIO_OUT_LOW(2, 7);

    paper_write_uint16(0x01, 0xf900);
    paper_write_uint8(0x3a, 0x06);
    paper_write_uint8(0x3b, 0x0b);
    paper_write_uint8(0x3c, 0x33);
    paper_write_uint8(0x11, 0x01);
    paper_write_uint16(0x44, 0x000f);
    paper_write_uint16(0x45, 0xf900);
    paper_write_uint8(0x2c, 0x4b);
    paper_write(0x32, 29,
                0x50, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x11, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F,
                0x0F, 0x0F, 0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00);
    paper_write_uint8(0x21, 0x83);
    paper_write_uint8(0x21, 0x03);
    paper_write_uint8(0x3c, 0x73);
}

/**
 *  @brief 墨水屏显示图片。完全刷新时间约 4s，其中上一帧残留约 2s。
 *  @param _buffer 长度为 4000 的 8 位数组
 *  @note Image2Lcd 部分转换参数：垂直扫描，250 * 128，颜色反转
 */
void paper_display_img(const uint8 _buffer[4000]) {
    paper_write_uint8(0x4e, 0x00);
    paper_write_uint8(0x4f, 0xf9);
    paper_write_address(0x24);
    const uint16 size = 250 * 16;
    uint16 cnt = size;
    for(cnt = size; cnt; --cnt)
        paper_write_data(_buffer[size - cnt]);
    paper_write_uint8(0x22, 0xc7);
    paper_write_address(0x20);
    paper_wait_busy();
}

#endif
