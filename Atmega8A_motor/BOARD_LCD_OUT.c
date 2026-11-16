#include "BOARD_LCD_OUT.h"
#include "LCD_1602.h"
#include <stdlib.h>
//-------------------------------------------------------
char PRINT_LCD_BUFFER[12];
//-------------------------------------------------------
void Uint_to_char_convertion(uint16_t value) {
	utoa(value, PRINT_LCD_BUFFER, 10);
}
//-------------------------------------------------------
void Int_to_char_convertion(int16_t value) {
	itoa(value, PRINT_LCD_BUFFER, 10);
}
//-------------------------------------------------------
void Board_lcd_ini(void){
	LCD_portini();
	LCD_ini();
	LCD_clear();
}
//-------------------------------------------------------
void Lcd_out_work(uint16_t motor_rps, uint16_t set_motor_rps, uint8_t triac_delay,
		int16_t PD_out,bool start) {
	LCD_clear();
	LCD_setcursor(1, 0);
    if(motor_rps>0)
    {
    	motor_rps=motor_rps*60;
    }
	Uint_to_char_convertion(motor_rps);
	LCD_printstr(PRINT_LCD_BUFFER);

	LCD_setcursor(1, 5);
	LCD_printstr("RPM   ");
	Uint_to_char_convertion(triac_delay);
	LCD_printstr(PRINT_LCD_BUFFER);
    if(set_motor_rps>0)
    {
    	set_motor_rps=set_motor_rps*60;
    }
	LCD_setcursor(0, 0);
	Uint_to_char_convertion(set_motor_rps);
	LCD_printstr(PRINT_LCD_BUFFER);

	LCD_setcursor(0, 5);
	LCD_printstr("S.RPM ");

	if(start==true)
	{
		LCD_printstr("START");
	}
	else
	{
	Int_to_char_convertion(PD_out);
	LCD_printstr(PRINT_LCD_BUFFER);
	}
}
//-------------------------------------------------------
void Lcd_out_setup(uint16_t PD_control_KP,uint16_t PD_control_KD)
{
LCD_clear();
LCD_setcursor(0, 0);

LCD_printstr("KP ");
Uint_to_char_convertion(PD_control_KP);
LCD_printstr(PRINT_LCD_BUFFER);

LCD_printstr(" KD ");
Uint_to_char_convertion(PD_control_KD);
LCD_printstr(PRINT_LCD_BUFFER);

/*LCD_setcursor(1, 0);
Uint_to_char_convertion(PD_control_delay);
LCD_printstr(PRINT_LCD_BUFFER);
LCD_printstr("*0.064");*/

}
