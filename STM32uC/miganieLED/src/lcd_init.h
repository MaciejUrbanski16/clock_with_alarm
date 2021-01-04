/*
 * lcd_init.h
 *
 *  Created on: 30.12.2020
 *      Author: Admin
 */

#ifndef LCD_INIT_H_
#define LCD_INIT_H_

#include <stdint.h>
#include "stm32f1xx.h"

#define LCD_DC			GPIO_PIN_1
#define LCD_CE			GPIO_PIN_2
#define LCD_RST			GPIO_PIN_3

extern SPI_HandleTypeDef spi;

extern GPIO_InitTypeDef gpio;

void lcdInit(void);

void lcd_setup(void);

void lcd_clear(void);
//void lcd_draw_bitmap(const uint8_t* data);
void lcd_draw_text(int row, int col, const char* text);

void lcd_copy(void);

#endif /* LCD_INIT_H_ */
