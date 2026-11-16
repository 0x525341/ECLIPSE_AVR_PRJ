
#include "LCD_1602.h"

#include <avr/io.h>
#include <util/delay.h>
//-------------------------------------------------------
void LCD_portini(void)
{
	LCD_DATA_DDR_D5_D7 |=((1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7)); //пины управления и сигнальные как выход
	LCD_DATA_DDR_D4 |=(1<<LCD_D4);
	LCD_CONTROL_DDR |= ((1<<LCD_RS)|(1<<LCD_E));
}
//-------------------------------------------------------
void LCD_sendhalfByte(uint8_t byte)
{
	LCD_CONTROL_PORT |=(1<<LCD_E);
	_delay_us(LCD_STROBE_DELAY_US);
	
	LCD_DATA_PORT_D5_D7 &=~((1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7));
	LCD_DATA_PORT_D4 &=~(1<<LCD_D4);
	
	if (byte & (1<<3))						  // Записываем младшую
	LCD_DATA_PORT_D5_D7 |= (1<<LCD_D7);			  // половину байта
	
	if (byte & (1<<2))
	LCD_DATA_PORT_D5_D7 |= (1<<LCD_D6);
	
	if (byte & (1<<1))
	LCD_DATA_PORT_D5_D7 |= (1<<LCD_D5);
	
	if (byte & (1<<0))
	LCD_DATA_PORT_D4 |= (1<<LCD_D4);
	
	LCD_CONTROL_PORT &=~(1<<LCD_E);
	_delay_us(LCD_STROBE_DELAY_US);
}
//-------------------------------------------------------
void LCD_sendByte(uint8_t byte, uint8_t mode)
{
	if (mode) LCD_CONTROL_PORT|=(1<<LCD_RS);
	else LCD_CONTROL_PORT&=~(1<<LCD_RS);
	
	LCD_sendhalfByte((byte>>4));
	LCD_sendhalfByte(byte);
}
//-------------------------------------------------------
void LCD_print(uint8_t byte)
{
	LCD_sendByte(byte,LCD_DATA);
}
//-------------------------------------------------------
void LCD_setcursor(int8_t str, int8_t col)
{
	LCD_sendByte((0b10000000|((0x40*str)+col)), LCD_COMMAND);
}
//-------------------------------------------------------
void LCD_clear(void)
{
	LCD_sendByte(0b00000001,LCD_COMMAND);
	_delay_us(1500);
}
//-------------------------------------------------------
void LCD_ini(void)
{
	_delay_ms(15);
	LCD_sendhalfByte(0b0011);
	_delay_ms(4);
	LCD_sendhalfByte(0b0011);
	_delay_us(100);
	LCD_sendhalfByte(0b0011);
	_delay_ms(1);
	LCD_sendhalfByte(0b0010);
	_delay_ms(1);
	LCD_sendByte(0b00101000, LCD_COMMAND); //4бит-режим (DL=0) и 2 линии (N=1)
	_delay_ms(1);
	LCD_sendByte(0b00001100, LCD_COMMAND); //включаем изображение на дисплее (D=1), курсоры никакие не включаем (C=0, B=0)
	_delay_ms(1);
	LCD_sendByte(0b00000110, LCD_COMMAND); //курсор (хоть он у нас и невидимый) будет двигаться влево
	_delay_ms(1);
}
//-------------------------------------------------------
void LCD_printstr(char str1[])
{
	uint8_t n;
	for(n=0;str1[n]!='\0';n++)
	LCD_print(str1[n]);
}
//-------------------------------------------------------
