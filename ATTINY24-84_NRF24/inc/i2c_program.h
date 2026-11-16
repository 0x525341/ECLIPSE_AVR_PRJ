/*
 * i2c_program.h
 *
 *  Created on: 19 авг. 2023 г.
 *      Author: User
 */

#ifndef INC_I2C_PROGRAM_H_
#define INC_I2C_PROGRAM_H_

#include "Board_PIN.h"

#define SCL_PIN 	B,2
#define SDA_PIN     B,3
#define PCIE_NUM    PCIE1
#define PCINT_SCL   PCINT8
#define PCINT_SDA   PCINT9
#define DELAY_I2C_MASTER 10
#define MY_I2C_ADDRESS 0x28
//#define I2C_SLAVE_MODE
#define I2C_MASTER_MODE

extern volatile uint8_t i2c_read_buffer[12];
extern volatile uint8_t i2c_write_buffer[12];
extern void i2c_init_slave(void);
extern void i2c_init_master(void);
#endif /* INC_I2C_PROGRAM_H_ */
