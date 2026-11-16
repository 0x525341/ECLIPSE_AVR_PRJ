/*
 * usart.c
 *
 *  Created on: 26 янв. 2022 г.
 *      Author: r.sibgatullin
 */
#include  "usart.h"
#include <avr/io.h>

void uart_tx_init ()
{
  TCCR0A = 1 << WGM01;		// compare mode
  TCCR0B = 1 << CS00;		// prescaler 1
  SOFT_TX_PORT |= SOFT_TX_PIN;
  SOFT_TX_DDR |= SOFT_TX_PIN;
  OCR0A = 201;			//38400 baudrate at prescaler 1
}

// Функция вывода содержимого переменной
void num_to_str(uint8_t value, uint8_t nDigit)
{
  switch (nDigit)
  {
  case 4:
    uart_send_byte((value / 1000) + '0');
  case 3:
    uart_send_byte(((value / 100) % 10) + '0');
  case 2:
    uart_send_byte(((value / 10) % 10) + '0');
  case 1:
    uart_send_byte((value % 10) + '0');
  }
}

void uart_print(char *str)
{
  uint8_t i = 0;
  while (str[i]) {
    uart_send_byte(str[i++]);
  }
}

//bitbanged UART transmit byte
void uart_send_byte (unsigned char data)
{
  unsigned char i;
  TCCR0B = 0;
  TCNT0 = 0;
  TIFR0 |= 1 << OCF0A;
  TCCR0B |= (1 << CS00);
  TIFR0 |= 1 << OCF0A;
  SOFT_TX_PORT &= ~SOFT_TX_PIN;
  while (!(TIFR0 & (1 << OCF0A)));
  TIFR0 |= 1 << OCF0A;
  for (i = 0; i < 8; i++)
  {
    if (data & 1)
      SOFT_TX_PORT |= SOFT_TX_PIN;
    else
      SOFT_TX_PORT &= ~SOFT_TX_PIN;
    data >>= 1;
    while (!(TIFR0 & (1 << OCF0A)));
    TIFR0 |= 1 << OCF0A;
  }
  SOFT_TX_PORT |= SOFT_TX_PIN;
  while (!(TIFR0 & (1 << OCF0A)));
  TIFR0 |= 1 << OCF0A;
}

void debug_usart(void)
{
	OCR0A = 10;
	while(1)
	{
		uart_print("OCR0A_VALUE\r\n");
		uart_send_byte(OCR0A);
		OCR0A = OCR0A+1;
		if(OCR0A == 254)
			OCR0A=10;
		_delay_ms(200);
	}
}
