/*
 * lcd_init.c
 *
 *  Created on: 30.12.2020
 *      Author: Admin
 */


#include <string.h>
#include "font.h"
#include "lcd_init.h"

#define PCD8544_FUNCTION_SET		0x20
#define PCD8544_DISP_CONTROL		0x08
#define PCD8544_DISP_NORMAL			0x0c
#define PCD8544_SET_Y				0x40
#define PCD8544_SET_X				0x80
#define PCD8544_H_TC				0x04
#define PCD8544_H_BIAS				0x10
#define PCD8544_H_VOP				0x80

#define LCD_BUFFER_SIZE			(84 * 48 / 8)

SPI_HandleTypeDef spi;
GPIO_InitTypeDef gpio;
static uint8_t lcd_buffer[LCD_BUFFER_SIZE];

void lcdInit(void){
	//porty we/wy dedykowane dla transmisji danych do wyœwietlacza

	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pin = GPIO_PIN_5 | GPIO_PIN_7;		// SCK, MOSI
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Mode = GPIO_MODE_AF_INPUT;
	gpio.Pin = GPIO_PIN_6;						// MISO
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pin = LCD_DC | LCD_CE | LCD_RST;
	HAL_GPIO_Init(GPIOC, &gpio);
	HAL_GPIO_WritePin(GPIOC, LCD_CE | LCD_RST, GPIO_PIN_SET);

	//konfuguracja interface'u SPI dla przesy³u danych do wyœwietlacza
	spi.Instance = SPI1;
	spi.Init.Mode = SPI_MODE_MASTER;
	spi.Init.NSS = SPI_NSS_SOFT;
	spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;	// 1MHz
	spi.Init.Direction = SPI_DIRECTION_2LINES;
	spi.Init.CLKPhase = SPI_PHASE_1EDGE;
	spi.Init.CLKPolarity = SPI_POLARITY_LOW;
	spi.Init.DataSize = SPI_DATASIZE_8BIT;
	spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spi.Init.TIMode = SPI_TIMODE_DISABLE;
	spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	spi.Init.CRCPolynomial = 7;
	HAL_SPI_Init(&spi);

	__HAL_SPI_ENABLE(&spi);
}

void lcd_cmd(uint8_t cmd)
{
	HAL_GPIO_WritePin(GPIOC, LCD_CE|LCD_DC, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, LCD_CE|LCD_DC, GPIO_PIN_SET);
}

void lcd_setup(void)
{
	HAL_GPIO_WritePin(GPIOC, LCD_RST, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, LCD_RST, GPIO_PIN_SET);

	lcd_cmd(PCD8544_FUNCTION_SET | 1);
	lcd_cmd(PCD8544_H_BIAS | 4);
	lcd_cmd(PCD8544_H_VOP | 0x2f);
	lcd_cmd(PCD8544_FUNCTION_SET);
	lcd_cmd(PCD8544_DISP_NORMAL);
}

void lcd_clear(void)
{
	memset(lcd_buffer, 0, LCD_BUFFER_SIZE);
}

void lcd_draw_bitmap(const uint8_t* data)
{
	memcpy(lcd_buffer, data, LCD_BUFFER_SIZE);
}

void lcd_draw_text(int row, int col, const char* text)
{
	int i;
	uint8_t* pbuf = &lcd_buffer[row * 84 + col];
	while ((*text) && (pbuf < &lcd_buffer[LCD_BUFFER_SIZE - 6])) {
		int ch = *text++;
		const uint8_t* font = &font_ASCII[ch - ' '][0];
		for (i = 0; i < 5; i++) {
			*pbuf++ = *font++;
		}
		*pbuf++ = 0;
	}
}

void lcd_copy(void)
{
	HAL_GPIO_WritePin(GPIOC, LCD_DC, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, LCD_CE, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&spi, lcd_buffer, LCD_BUFFER_SIZE, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, LCD_CE, GPIO_PIN_SET);
}
