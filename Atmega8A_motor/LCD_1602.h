
#ifndef LCD_1602_H_
#define LCD_1602_H_
#include <stdint.h> //uint
//-------------------------------------------------------
#define LCD_DATA_PORT_D5_D7	        PORTD		// Порт и пины,
#define LCD_DATA_DDR_D5_D7		    DDRD		// к которым подключены сигналы D4-D7.

#define LCD_DATA_PORT_D4	        PORTB
#define LCD_DATA_DDR_D4	            DDRB

#define LCD_D4				        PINB3
#define LCD_D5				        PIND5
#define LCD_D6				        PIND6
#define LCD_D7				        PIND7

#define LCD_CONTROL_PORT	        PORTB		// Порт и пины,
#define LCD_CONTROL_DDR		        DDRB		// к которым подключены
#define LCD_RS				        PINB1		// сигналы RS и E.
#define LCD_E				        PINB2

#define LCD_STROBE_DELAY_US	        50

#define LCD_COMMAND			        0
#define LCD_DATA			        1

#define LCD_CURSOR_OFF		        0
#define LCD_CURSOR_ON		        2
#define LCD_CURSOR_BLINK	        3

#define LCD_DISPLAY_OFF		        0
#define LCD_DISPLAY_ON		        4

#define LCD_SCROLL_LEFT		        0
#define LCD_SCROLL_RIGHT	        4
//-------------------------------------------------------
void LCD_portini(void);
//void LCD_sendhalfByte(uint8_t byte);
//void LCD_sendByte(uint8_t byte, uint8_t mode);
void LCD_print(uint8_t byte);
void LCD_setcursor(int8_t str, int8_t col);
void LCD_clear(void);
void LCD_ini(void);
void LCD_printstr(char str1[]);
//-------------------------------------------------------
#endif /* LCD_1602_H_ */
