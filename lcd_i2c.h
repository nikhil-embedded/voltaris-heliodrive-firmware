/**
 * @file    lcd_i2c.h
 * @brief   16x2 I2C LCD Driver (PCF8574 backpack)
 * @author  Nikhil Sanjay Nikam
 * @date    2026
 */

#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "i2c.h"
#include "main.h"

#define LCD_ADDR        (0x27 << 1)
#define LCD_BACKLIGHT   0x08
#define LCD_NOBACKLIGHT 0x00
#define LCD_EN          0x04
#define LCD_RW          0x02
#define LCD_RS          0x01

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Set_Cursor(uint8_t row, uint8_t col);
void LCD_Send_String(char *str);
void LCD_Send_Char(char ch);

#endif /* LCD_I2C_H */
