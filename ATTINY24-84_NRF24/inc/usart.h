#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <util/delay.h>

#define SOFT_TX_PIN (1<<PB1) // PB1 будет работать как TXD
#define SOFT_TX_PORT PORTB
#define SOFT_TX_DDR DDRB

void uart_tx_init ();
void num_to_str(uint8_t value, uint8_t nDigit);
void uart_print(char *str);
void uart_send_byte (unsigned char data);
void debug_usart(void);

#endif /* USART_H_ */
