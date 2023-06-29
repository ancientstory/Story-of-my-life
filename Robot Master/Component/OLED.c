#include "OLED.h"
#include "i2c.h"
#include "OLEDFont.h"
static uint8_t OLED_GRAM[128][8];

void OLED_Write_Byte(uint8_t data, uint8_t cmd)
{
    static uint8_t cmd_data[2];
    if (cmd == OLED_CMD)
    {
        cmd_data[0] = 0x00;
    }
    else
    {
        cmd_data[0] = 0x40;
    }
    cmd_data[1] = data;
    HAL_I2C_Master_Transmit(&hi2c2, OLED_I2C_ADDRESS, cmd_data, 2, 10);
}
void OLED_Init(void)
{
    OLED_Write_Byte(0xAE, OLED_CMD); // display off
    OLED_Write_Byte(0x20, OLED_CMD); // Set Memory Addressing Mode
    OLED_Write_Byte(0x10, OLED_CMD); // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    OLED_Write_Byte(0xb0, OLED_CMD); // Set Page Start Address for Page Addressing Mode,0-7
    OLED_Write_Byte(0xc8, OLED_CMD); // Set COM Output Scan Direction
    OLED_Write_Byte(0x00, OLED_CMD); //---set low column address
    OLED_Write_Byte(0x10, OLED_CMD); //---set high column address
    OLED_Write_Byte(0x40, OLED_CMD); //--set start line address
    OLED_Write_Byte(0x81, OLED_CMD); //--set contrast control register
    OLED_Write_Byte(0xff, OLED_CMD); // brightness 0x00~0xff
    OLED_Write_Byte(0xa1, OLED_CMD); //--set segment re-map 0 to 127
    OLED_Write_Byte(0xa6, OLED_CMD); //--set normal display
    OLED_Write_Byte(0xa8, OLED_CMD); //--set multiplex ratio(1 to 64)
    OLED_Write_Byte(0x3F, OLED_CMD); //
    OLED_Write_Byte(0xa4, OLED_CMD); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    OLED_Write_Byte(0xd3, OLED_CMD); //-set display offset
    OLED_Write_Byte(0x00, OLED_CMD); //-not offset
    OLED_Write_Byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
    OLED_Write_Byte(0xf0, OLED_CMD); //--set divide ratio
    OLED_Write_Byte(0xd9, OLED_CMD); //--set pre-charge period
    OLED_Write_Byte(0x22, OLED_CMD); //
    OLED_Write_Byte(0xda, OLED_CMD); //--set com pins hardware configuration
    OLED_Write_Byte(0x12, OLED_CMD);
    OLED_Write_Byte(0xdb, OLED_CMD); //--set vcomh
    OLED_Write_Byte(0x20, OLED_CMD); // 0x20,0.77xVcc
    OLED_Write_Byte(0x8d, OLED_CMD); //--set DC-DC enable
    OLED_Write_Byte(0x14, OLED_CMD); //
    OLED_Write_Byte(0xaf, OLED_CMD); //--turn on oled panel
}
void OLED_display_on(void)
{
    OLED_Write_Byte(0x8d, OLED_CMD);
    OLED_Write_Byte(0x14, OLED_CMD);
    OLED_Write_Byte(0xaf, OLED_CMD);
}
void OLED_display_off(void)
{
    OLED_Write_Byte(0x8d, OLED_CMD);
    OLED_Write_Byte(0x10, OLED_CMD);
    OLED_Write_Byte(0xAE, OLED_CMD);
}
void OLED_operate_gram(PENTypeDef pen)
{
    static uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        for (n = 0; n < 128; n++)
        {
            if (pen == PEN_WRITE)
            {
                OLED_GRAM[n][i] = 0xff;
            }
            else if (pen == PEN_CLEAR)
            {
                OLED_GRAM[n][i] = 0x00;
            }
            else
            {
                OLED_GRAM[n][i] = 0xff - OLED_GRAM[n][i];
            }
        }
    }
}
void OLED_cursor_position(uint8_t x, uint8_t y)
{
    OLED_Write_Byte((0xb0 + y), OLED_CMD);               // set page address y
    OLED_Write_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD); // set column high address
    OLED_Write_Byte((x & 0x0f), OLED_CMD);               // set column low address
}
void OLED_refresh_gram(void)
{
    uint8_t i, n;

    for (i = 0; i < 8; i++)
    {
        OLED_cursor_position(0, i);
        for (n = 0; n < 128; n++)
        {
            OLED_Write_Byte(OLED_GRAM[n][i], OLED_DATA);
        }
    }
}
void OLED_clear(void)
{
    OLED_operate_gram(PEN_CLEAR);
    OLED_refresh_gram();
}
void OLED_fill(void)
{
    OLED_operate_gram(PEN_WRITE);
    OLED_refresh_gram();
}
// x:0~127
// y:0_63
void OLED_draw_point(int8_t x, int8_t y, PENTypeDef pen)
{
    uint8_t page = 0, row = 0;
    if ((x < 0) || (x > (X_WIDTH - 1)) || (y < 0) || (y > (Y_WIDTH - 1)))
    {
        return;
    }
    page = y / 8;
    row = y % 8;

    if (pen == PEN_WRITE)
    {
        OLED_GRAM[x][page] |= 1 << row;
    }
    else if (pen == PEN_INVERSION)
    {
        OLED_GRAM[x][page] ^= 1 << row;
    }
    else
    {
        OLED_GRAM[x][page] &= ~(1 << row);
    }
    // OLED_refresh_gram();
}
void OLED_show_char(uint8_t row, uint8_t col, uint8_t chr)
{
    uint8_t x = col * 6;
    uint8_t y = row * 12;
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    chr = chr - ' ';

    for (t = 0; t < 12; t++)
    {
        temp = asc2_1206[chr][t];

        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)
                OLED_draw_point(x, y, PEN_WRITE);
            else
                OLED_draw_point(x, y, PEN_CLEAR);

            temp <<= 1;
            y++;
            if ((y - y0) == 12)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
    OLED_refresh_gram();
}
void OLED_show_string(uint8_t row, uint8_t col, uint8_t *chr)
{
    uint8_t n = 0;

    while (chr[n] != '\0')
    {
        OLED_show_char(row, col, chr[n]);
        col++;

        if (col > 20)
        {
            col = 0;
            row += 1;
        }
        n++;
    }
    OLED_refresh_gram();
}
void OLED_show_number(uint8_t row, uint8_t col, int16_t num)
{
    uint8_t len = 0;
    uint8_t str[10];
    uint8_t flag = 0;
    if (num < 0)
    {
        flag = 1;
        num = -num;
    }
    do
    {
        str[len++] = num % 10 + '0';
        num /= 10;
    } while (num != 0);
    if (flag)
        str[len++] = '-';
    str[len] = '\0';
    for (int i = 0; i < len / 2; i++)
    {
        uint8_t temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
    OLED_show_string(row, col, str);
}
void OLED_LOGO(void)
{
    uint8_t temp_char = 0;
    uint8_t x = 0, y = 0;
    uint8_t i = 0;
    OLED_operate_gram(PEN_CLEAR);

    for (; y < 64; y += 8)
    {
        for (x = 0; x < 128; x++)
        {
            temp_char = LOGO_BMP[x][y / 8];
            for (i = 0; i < 8; i++)
            {
                if (temp_char & 0x80)
                {
                    OLED_draw_point(x, y + i, PEN_WRITE);
                }
                else
                {
                    OLED_draw_point(x, y + i, PEN_CLEAR);
                }
                temp_char <<= 1;
            }
        }
    }
    OLED_refresh_gram();
}
