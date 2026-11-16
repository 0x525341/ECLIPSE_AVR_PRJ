#ifndef BOARD_LCD_OUT_H_
#define BOARD_LCD_OUT_H_
//-------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
//-------------------------------------------------------
void Board_lcd_ini(void);
//-------------------------------------------------------
void Lcd_out_work(uint16_t motor_rps, uint16_t set_motor_rps, uint8_t triac_delay,
		int16_t PD_out,bool start);
//-------------------------------------------------------
void Lcd_out_setup(uint16_t PD_control_KP,uint16_t PD_control_KD);
//-------------------------------------------------------
#endif /* BOARD_LCD_OUT_H_ */
