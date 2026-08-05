/**
 * @file    lcd_i2c.c
 * @brief   16x2 I2C LCD Driver (PCF8574 backpack)
 * @author  Nikhil Sanjay Nikam
 * @date    2026
 */

#include "lcd_i2c.h"

static void LCD_Send_Nibble(uint8_t nibble, uint8_t rs)
{
    uint8_t data = (nibble & 0xF0) | LCD_BACKLIGHT | rs;

    uint8_t en_high = data | LCD_EN;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR,
                             &en_high, 1, 100);
    HAL_Delay(1);

    uint8_t en_low = data & ~LCD_EN;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR,
                             &en_low, 1, 100);
    HAL_Delay(1);
}

static void LCD_Send_Byte(uint8_t byte, uint8_t rs)
{
    LCD_Send_Nibble(byte & 0xF0, rs);
    LCD_Send_Nibble((byte << 4) & 0xF0, rs);
}

static void LCD_Send_Cmd(uint8_t cmd)
{
    LCD_Send_Byte(cmd, 0x00);
}

void LCD_Send_Char(char ch)
{
    LCD_Send_Byte((uint8_t)ch, LCD_RS);
}

void LCD_Init(void)
{
    HAL_Delay(50);

    LCD_Send_Nibble(0x30, 0x00);
    HAL_Delay(5);
    LCD_Send_Nibble(0x30, 0x00);
    HAL_Delay(1);
    LCD_Send_Nibble(0x30, 0x00);
    HAL_Delay(1);
    LCD_Send_Nibble(0x20, 0x00);
    HAL_Delay(1);

    LCD_Send_Cmd(0x28);
    HAL_Delay(1);
    LCD_Send_Cmd(0x0C);
    HAL_Delay(1);
    LCD_Send_Cmd(0x06);
    HAL_Delay(1);
    LCD_Send_Cmd(0x01);
    HAL_Delay(5);
}

void LCD_Clear(void)
{
    LCD_Send_Cmd(0x01);
    HAL_Delay(5);
}

void LCD_Set_Cursor(uint8_t row, uint8_t col)
{
    uint8_t row_offsets[] = {0x00, 0x40};
    LCD_Send_Cmd(0x80 | (col + row_offsets[row]));
    HAL_Delay(1);
}

void LCD_Send_String(char *str)
{
    while (*str)
        LCD_Send_Char(*str++);
}
