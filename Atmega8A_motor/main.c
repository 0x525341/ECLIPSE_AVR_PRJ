#include "ADC_ATMEGA8.h"
#include "PD_CONTROL.h"
#include "MAP.h"
#include "BOARD_PIN.h"
#include "BOARD_LCD_OUT.h"
//-------------------------------------------------------
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h> //uint
#include <stdbool.h>
//-------------------------------------------------------
#define HALL_SENSOR_ONE_TURN_MOTOR  8
#define TIMER1_GREQUENCY            16000000UL
//-------------------------------------------------------
#define MOTOR_MIN_RPS               0
#define MOTOR_MAX_RPS               25
#define MOTOR_ALARM_RPS             35
//-------------------------------------------------------
#define TRIAC_DELAY_VALUE_MOTOR_RPS_0         100
#define TRIAC_DELAY_VALUE_MOTOR_RPS_MIN       125 //задержка открытия симистора меньще значение - больше открыто
#define TRIAC_DELAY_VALUE_MOTOR_RPS_MAX       244
//-------------------------------------------------------
#define LCD_OUT_DELAY               10   //количество проходов через 0 для вывода на экран(1 - 10мс)
#define MOTOR_RPS_TOLERANCE         1    //допуск -+ оборотов в секунду
#define MOTOR_Smooth_Start          1    //скорость плавного старта

#define Initialization              true
#define Regulation                  false
//-------------------------------------------------------
struct PDControl PD;

volatile uint16_t TIMER1_COUNTER_BUFFER = 0;
volatile uint8_t TIMER1_OVERFLOW_BUFFER = 0;
volatile uint8_t TIMER1_OVERFLOW_FIXED_BUFFER = 0;
volatile uint8_t TRIAC_DELAY_BUFFER = TRIAC_DELAY_VALUE_MOTOR_RPS_0; //от 100 до 244
volatile uint8_t PD_CONTROL_DELAY = 1;
volatile uint8_t PD_CONTROL_COUNTER_BUFFER = 0;
volatile uint8_t ZERO_CROSS_COUNTER_BUFFER = 0;
volatile bool ZERO_CROSS_FLAG_BUFFER = false;
volatile bool LCD_OUT_FLAG_BUFFER = false;
volatile bool HALL_SENSOR_FLAG_BUFFER = true;
//-------------------------------------------------------
#define Filter_array_size 4
uint8_t Filter_array[Filter_array_size] = { 0 };
uint8_t *Filter_array_end = &Filter_array[Filter_array_size - 1];
uint8_t *Filter_array_current_position = &Filter_array[0];
//-------------------------------------------------------
void Int_ini(void) {
	MCUCR |= (1 << ISC01); //прерывание 0 по заднему фронту
	GICR |= (1 << INT0);
}
//-------------------------------------------------------
ISR(INT0_vect) //обработчик прерывания 0
{
	ZERO_CROSS_FLAG_BUFFER = true;

	ZERO_CROSS_COUNTER_BUFFER++;
	if (ZERO_CROSS_COUNTER_BUFFER == LCD_OUT_DELAY) {
		LCD_OUT_FLAG_BUFFER = true;
		ZERO_CROSS_COUNTER_BUFFER = 0;
	}
	TCNT0 = TRIAC_DELAY_BUFFER;
	TCCR0 = (1 << CS02) | (1 << CS00); //СТАРТ ТАЙМЕР 0 делитель 1024(0,064мс один такт) за 10 мс досчитает до 156.
}
//-------------------------------------------------------
void Timer1_ini(void) {
	TCCR1B = 0;
	TCCR1A = 0;
	TCNT1 = 0;
	TIMSK |= (1 << TICIE1) | (1 << TOIE1);
	TCCR1B = (1 << ICES1) | (1 << CS10) | (1 << ICNC1); // старт таймера шумоподавление срабатывание по положительному фронту без делителя
}
//-------------------------------------------------------
ISR(TIMER1_CAPT_vect) //обработчик прерывания ICP1
{
	TCNT1 = 0;
	TIMER1_COUNTER_BUFFER = ICR1;
	if (HALL_SENSOR_FLAG_BUFFER == false) {
		HALL_SENSOR_FLAG_BUFFER = true;
	}

	if (TIMER1_OVERFLOW_BUFFER > 0) {
		TIMER1_OVERFLOW_FIXED_BUFFER = TIMER1_OVERFLOW_BUFFER;
		TIMER1_OVERFLOW_BUFFER = 0;
	} else {
		TIMER1_OVERFLOW_FIXED_BUFFER = 0;
	}
}
//-------------------------------------------------------
ISR (TIMER1_OVF_vect) {
	TCNT1 = 0;
	TIMER1_OVERFLOW_BUFFER++;

	if (TIMER1_OVERFLOW_BUFFER > 30) {
		TIMER1_OVERFLOW_BUFFER = 0;
		HALL_SENSOR_FLAG_BUFFER = false;
	}
}
//-------------------------------------------------------
void Timer0_ini(void) {
	TCCR0 = 0; //СТОП ТАЙМЕР 0
	TCNT0 = 0;
	TIMSK |= (1 << TOIE0); //прерывание по переполнению
}
//-------------------------------------------------------
ISR(TIMER0_OVF_vect) {
	TCCR0 = 0; //СТОП ТАЙМЕР 0
	TCNT0 = 0;
	TRIAC_CONTROL_PORT |= (1 << TRIAC_CONTROL_PIN);
	_delay_us(10);
	TRIAC_CONTROL_PORT &= ~(1 << TRIAC_CONTROL_PIN);
}
//-------------------------------------------------------
/*void Timer2_ini(void) {
 TCCR2 = 0;
 TCNT2 = 0;
 OCR2 = 0;
 TCCR2 |= (1 << CS20) | (1 << CS21) | (1 << CS22);  //делитель 1024
 TIMSK |= (1 << OCIE2);                   //Прерывание по совпадению
 }
 //-------------------------------------------------------
 ISR(TIMER2_COMP_vect) {
 PD_CONTROL_ON_OFF_FLAG_BUFFER = true;
 TCNT2 = 0; //сброс счётного регистра 2 таймера
 }*/
//-------------------------------------------------------
void Board_Setup(bool mode) {
	uint16_t PD_control_Kd = Map((ADC_convert(POT_RESISTOR_PIN_3)), 0, 1023, 0,
			1000);
	uint16_t PD_control_Kp = Map((ADC_convert(POT_RESISTOR_PIN_2)), 0, 1023, 0,
			1000);
	//	uint8_t PD_control_Delay = Map((ADC_convert(POT_RESISTOR_PIN_1)), 0, 1023, 1, 250);
	if (mode == Initialization) {
		PD_control_init(&PD, PD_control_Kd, PD_control_Kd);
		//		OCR2 =PD_control_Delay;
		Lcd_out_setup(PD_control_Kp, PD_control_Kd);
	}
	if (mode == Regulation) {
		PD_control_Kp_Kd(&PD, PD_control_Kp, PD_control_Kd);
		//		PD_CONTROL_DELAY=PD_control_Delay;
	}
}
//-------------------------------------------------------
uint8_t Motor_rps_calc(void) {
	cli();
	uint32_t TIMER1_COUNTER_VALUE = TIMER1_COUNTER_BUFFER;
	uint32_t TIMER1_OVERFLOW_FIXED_VALUE = TIMER1_OVERFLOW_FIXED_BUFFER;
	bool HALL_SENSOR_FLAG = HALL_SENSOR_FLAG_BUFFER;
	sei();

	if (HALL_SENSOR_FLAG == true) {
		if (TIMER1_OVERFLOW_FIXED_VALUE > 0) {
			TIMER1_COUNTER_VALUE = TIMER1_COUNTER_VALUE
					+ (TIMER1_OVERFLOW_FIXED_VALUE * 65536UL);
		}
		return (((TIMER1_GREQUENCY / (TIMER1_COUNTER_VALUE))
				/ HALL_SENSOR_ONE_TURN_MOTOR));
	} else {
		return (0);
	}
}
//-------------------------------------------------------
uint8_t Filter(uint8_t value) {
	uint16_t Filter_result = 0;
	*Filter_array_current_position = value;
	Filter_array_current_position++;
	if (Filter_array_current_position > Filter_array_end) {
		Filter_array_current_position = &Filter_array[0];
	}

	for (uint8_t position = 0; position < Filter_array_size; position++) {
		Filter_result = Filter_result + Filter_array[position];
	}

	if (Filter_result > 0) {
		return ((uint8_t) (Filter_result / Filter_array_size));
	}
	return (0);
}
//-------------------------------------------------------
int main(void) {
	Board_pin_ini();
	Board_lcd_ini();
	ADC_Init();
	uint8_t MOTOR_RPS = 0;
	uint8_t SET_MOTOR_RPS = 0;
	int16_t PD_out = 0;
	uint8_t TRIAC_DELAY_VALUE = 0;
	bool Smooth_Start = true;
	//------------------------------------------------------
	for (uint8_t x = 0; x < 60; x++) {
		Board_Setup(Initialization);
		_delay_ms(500);
	}
	RELAY_PORT |= (1 << RELAY_PIN);
	_delay_ms(3000);
	Int_ini();
	Timer1_ini();
	Timer0_ini();
	sei();
	//------------------------------------------------------
	while (1) {
		cli();
		bool LCD_OUT_FLAG = LCD_OUT_FLAG_BUFFER;
		LCD_OUT_FLAG_BUFFER = false;
		sei();
		//------------------------------------------------------

		if (SET_MOTOR_RPS > MOTOR_MIN_RPS) {
			cli();
			bool ZERO_CROSS_FLAG = ZERO_CROSS_FLAG_BUFFER;
			ZERO_CROSS_FLAG_BUFFER=false;
			sei();

			if (Smooth_Start == true && ZERO_CROSS_FLAG == true) {

				cli();
				TRIAC_DELAY_BUFFER = TRIAC_DELAY_BUFFER + MOTOR_Smooth_Start;
				sei();

				MOTOR_RPS = Filter(Motor_rps_calc());

				if (MOTOR_RPS > MOTOR_MIN_RPS) {
					Smooth_Start = false;
				}
			}
			//------------------------------------------------------
			if (Smooth_Start == false && ZERO_CROSS_FLAG == true) {

				MOTOR_RPS = Filter(Motor_rps_calc());

				PD_out = PD_control(&PD, SET_MOTOR_RPS, MOTOR_RPS);
				PD_out = PD_out >> 8;
				TRIAC_DELAY_VALUE = TRIAC_DELAY_VALUE + PD_out;
				if (TRIAC_DELAY_VALUE > TRIAC_DELAY_VALUE_MOTOR_RPS_MAX) {
					TRIAC_DELAY_VALUE = TRIAC_DELAY_VALUE_MOTOR_RPS_MAX;
				} else if (TRIAC_DELAY_VALUE < TRIAC_DELAY_VALUE_MOTOR_RPS_MIN) {
					TRIAC_DELAY_VALUE = TRIAC_DELAY_VALUE_MOTOR_RPS_MIN;
				}
				cli();
				TRIAC_DELAY_BUFFER = TRIAC_DELAY_VALUE;
				sei();

				if (MOTOR_RPS == 0 && SET_MOTOR_RPS<=MOTOR_MIN_RPS) {
					Smooth_Start = true; //разрешение плавного старта
					PD_control_reset(&PD);
				}

			}
		} else {
			cli();
			TRIAC_DELAY_BUFFER = TRIAC_DELAY_VALUE_MOTOR_RPS_0;
			sei();
		}
		//------------------------------------------------------
		if (LCD_OUT_FLAG == true) {

			Board_Setup(Regulation);

			SET_MOTOR_RPS = Map((ADC_convert(POT_RESISTOR_SET_RPS_PIN)), 0,
					1023, 0,
					MOTOR_MAX_RPS);
			Lcd_out_work(MOTOR_RPS, SET_MOTOR_RPS, TRIAC_DELAY_VALUE, PD_out,
					Smooth_Start);
		}

	}
}

