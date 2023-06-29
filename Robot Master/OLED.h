#ifndef _OLED_H_
#define _OLED_H_

/******************Basic define*******************/
#include "stdint.h"

#define OLED_I2C_ADDRESS 0x78

#define MAX_COLUMN 128
#define MAX_ROW 64

#define X_WIDTH MAX_COLUMN
#define Y_WIDTH MAX_ROW

#define OLED_CMD 0x00
#define OLED_DATA 0x01

#define CHAR_SIZE_WIDTH 6
#define CHAR_SIZE_HIGHT 12

typedef enum
{
    PEN_CLEAR = 0x00,
    PEN_WRITE = 0x01,
    PEN_INVERSION = 0x02,
} PENTypeDef;

void OLED_Init(void);
void OLED_display_on(void);
void OLED_display_off(void);

void OLED_clear(void);
void OLED_fill(void);
void OLED_draw_point(int8_t x, int8_t y, PENTypeDef pen);
void OLED_show_char(uint8_t row, uint8_t col, uint8_t chr);
void OLED_show_string(uint8_t row, uint8_t col, uint8_t *chr);
void OLED_show_number(uint8_t row, uint8_t col, int16_t num);

void OLED_LOGO(void);
#endif
