/*
 * spi.h
 *
 *  Created on: 18 авг. 2023 г.
 *      Author: User
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_
#include <stdint.h>

extern uint8_t spi_write_byte(uint8_t data);
extern void spi_init(void);

#endif /* INC_SPI_H_ */
