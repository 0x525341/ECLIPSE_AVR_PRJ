/*
 * systick.c
 *
 *  Created on: 17 нояб. 2023 г.
 *      Author: Tramp
 */

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t time = 0;

//#define USE_SYSTICK

void systick_ini(uint8_t period_ms){
	  TCCR0B = (1 << CS00)|(1 << CS02);		// CLK/1024
	  TCNT0=0;
	  OCR0A = ((F_CPU/1024)/1000)*period_ms;
	  TIMSK0 |= (1 << OCIE0A);//по совпадению
}

#if defined USE_SYSTICK
ISR (TIMER0_COMPA_vect)
{
	TCNT0=0;
	time++;
}
#endif
