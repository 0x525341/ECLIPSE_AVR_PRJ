/*
 * rf24.h
 *
 *  Created on: 16 авг. 2023 г.
 *      Author: User
 */

#ifndef INC_RF24_H_
#define INC_RF24_H_

#include "Board_PIN.h"
#include <nRF24L01.h>
#include <stdbool.h>
#include <stddef.h>
#include "stdint.h"
#define NRF24_CSN_PIN      	A,7
#define NRF24_CE_PIN      	A,3
#define NRF24_IRQ_PIN      	B,2

#define RF24_data_size 		1
#define RF24_addr_size 		3

#define SET_CSN 			PIN_SET(NRF24_CSN_PIN)
#define RESET_CSN 			PIN_RESET(NRF24_CSN_PIN)

#define SET_CE 				PIN_SET(NRF24_CE_PIN)
#define RESET_CE 			PIN_RESET(NRF24_CE_PIN)

#define RF_IRQ_HANDLER       ISR(INT0_vect)

enum rf_send_status {
    rf_send_success,
	rf_send_success_rx_ack,
	rf_send_error_timeout,
	rf_send_error_max_rt,
	rf_send_error_max_fifo,
	rf_send_error_status
};

extern void rf_init(uint8_t CH, uint8_t pipe_mask, bool RX_mode, bool dynamic_ack, uint8_t retr_delay, uint8_t retr_count);
extern void rf_send_ack_data(uint8_t pipe_num, uint8_t *data, uint8_t num);
extern enum rf_send_status rf_send(uint8_t* tx_data, size_t num, uint8_t* ack_data);
extern void rf_set_rx_mode(void);
extern void rf_set_tx_mode(uint8_t *tx_addr, uint8_t data_size);
extern void rf_set_rx_addr(uint8_t pipe_num, uint8_t *addr, uint8_t data_size);
extern void rf_read_rx(uint8_t *rx_data);
extern uint8_t rf_get_status(void);
extern void rf_reset_status(void);


#endif /* INC_RF24_H_ */
