/*
 * main.c
 *
 *  Created on: 13 янв. 2022 г.
 *      Author: r.sibgatullin
 */
/*
 * main.cpp
 *
 *  Created on: 12 янв. 2022 г.
 *      Author: r.sibgatullin
 */
#include "Board_PIN.h"
#include "spi.h"
#include "mcp2515.h"
#include "usart.h"
#include "stdlib.h"
#include "avr/sleep.h"
#include <util/delay.h>
#define UART_SPEED 38400
tCAN CANmessage;

SPI_InitTypeDef SPI_struct;
SPI_InitTypeDef SPI;

void init_int0(void)
{
	EICRA |=(1 << ISC01);
	EIMSK |=(1 << INT0);
}

void init_board(void)
{
	SPI_InitTypeDef SPI_struct;
	SPI_struct.SPI_set = SPI_ON;
	SPI_struct.SPI_Mode = SPI_MASTER;
	SPI_struct.SPI_Direct = SPI_DIRECT_MSB;
	SPI_struct.SPI_Prescaler = SPI_PRESCALER_NO;
	SPI_struct.SPI_Phase = SPI_CPHA_1EDGE;
	SPI_struct.SPI_Polaric = SPI_CPOL_LOW;
	SPI_init(&SPI_struct);

	PIN_SET_OUTPUT(GREEN_LED_PIN);
	PIN_SET_OUTPUT(BLUE_LED_PIN);
	PIN_SET(GREEN_LED_PIN);
	PIN_SET(BLUE_LED_PIN);

	PIN_SET_OUTPUT(UP_PIN);
	PIN_SET_OUTPUT(DOWN_PIN);
	PIN_SET_OUTPUT(START_STOP_PIN);

	PIN_SET_INPUT(RX1BF_PIN);
	PIN_SET_INPUT(RX0BF_PIN);

	mcp2515_init(CAN_33KBPS, MCP_8MHZ, FILTER_ON_EXTENDED_ID);
	usart_init(UART_SPEED);
	init_int0();
}

ISR(INT0_vect)
{
	cli();
	if (mcp2515_get_message(&CANmessage)) {
		if (CANmessage.id == (uint32_t)BUTTONS_CAN_ID)
		{
			switch(CANmessage.data[0])
			{
				case VOICE_BUTTON_DATA:
					PIN_SET(START_STOP_PIN);
					PIN_RESET(GREEN_LED_PIN);
					PIN_RESET(BLUE_LED_PIN);
					_delay_ms(200);
					PIN_RESET(START_STOP_PIN);
					PIN_SET(GREEN_LED_PIN);
					PIN_SET(BLUE_LED_PIN);
					usart_char_out('S');
					break;
				case NEXT_BUTTON_DATA:
					PIN_SET(UP_PIN);
					PIN_RESET(GREEN_LED_PIN);
					_delay_ms(200);
					PIN_RESET(UP_PIN);
					PIN_SET(GREEN_LED_PIN);
					usart_char_out('N');
					break;
				case PREV_BUTTON_DATA:
					PIN_SET(DOWN_PIN);
					PIN_RESET(BLUE_LED_PIN);
					_delay_ms(200);
					PIN_RESET(DOWN_PIN);
					PIN_SET(BLUE_LED_PIN);
					usart_char_out('D');
					break;
				default:
					PIN_RESET(START_STOP_PIN);
					PIN_RESET(UP_PIN);
					PIN_RESET(DOWN_PIN);
			}
//			for (uint8_t x=0; x<8; x++)
//			{
//			usart_char_out(CANmessage.data[x]);
//			usart_char_out('|');
//			}
			}
		}
	sei();
}
int main(void)
{
	init_board();
	set_sleep_mode(SLEEP_MODE_STANDBY);
	sei();
	while(1)
	{
	sleep_mode();
	}
	return 0;
}
