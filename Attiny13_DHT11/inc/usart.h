#ifndef USART_H_
#define USART_H_
//#define F_CPU 9600000

#include <avr/io.h>
#include <util/delay.h>

#define SOFT_TX_PIN (1<<PB1) // PB1 будет работать как TXD
#define SOFT_TX_PORT PORTB
#define SOFT_TX_DDR DDRB

void uart_tx_init ();
void num_to_str(unsigned int value, unsigned char nDigit);
void uart_print(char *str);
void uart_send_byte (unsigned char data);

#endif /* USART_H_ */
