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
#include <util/delay.h>
#include "DHT11.h"
#include "usart.h"
#include <avr/io.h>

uint8_t DHTtemperature=0;
uint8_t DHThumidity=0;

int main(void)
{
	uart_tx_init();
	PIN_SET_OUTPUT(TRIAC_PIN);
	uint8_t temperature=0;
	uint8_t humidity=0;
	while(1)
	{
		if (tdhtReadDataDHT11(3, &temperature, &humidity))
		{
			uart_send_byte(temperature);
			uart_send_byte(humidity);
			if (humidity>40)
			{
				PIN_SET(TRIAC_PIN);
			}
			else {PIN_RESET(TRIAC_PIN);}
		}
		_delay_ms(60000);
	}
	return 0;
}
