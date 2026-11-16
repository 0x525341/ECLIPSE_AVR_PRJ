/*
 * i2c_program.c
 *
 *  Created on: 19 авг. 2023 г.
 *      Author: User
 */
#include "i2c_program.h"
#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile static bool SCL_old_state = false;
volatile static bool SDA_old_state = false;

volatile static bool SCL_rising_edge_flag = false;
volatile static bool SCL_falling_edge_flag = false;

volatile static bool SDA_rising_edge_flag = false;
volatile static bool SDA_falling_edge_flag = false;

volatile uint8_t i2c_read_buffer[12];
volatile uint8_t i2c_write_buffer[12];

static enum i2c_state
{
	WHAIT_START,
	START,
	BIT_RW_P1,
	BIT_RW_P2,
	ACK_P1,
	ACK_P2,
	ACK_P3,
	STOP
};

void i2c_init_slave(void)
{
	PIN_SET_INPUT(SCL_PIN);
	PIN_SET_INPUT(SDA_PIN);

	GIMSK |= (1<<PCIE_NUM);
	PCMSK1 |= (1<<PCINT_SCL)|(1<<PCINT_SDA);

	SCL_old_state = PIN_IS_SET(SCL_PIN);
	SDA_old_state = PIN_IS_SET(SDA_PIN);
}

void i2c_init_master(void)
{
	PIN_SET_OUTPUT(SCL_PIN);
	PIN_SET_OUTPUT(SDA_PIN);
}

static void i2c_gen_start_master(void)
{
	PIN_RESET(SDA_PIN);
	_delay_us(DELAY_I2C_MASTER);
	PIN_RESET(SCL_PIN);
	_delay_us(DELAY_I2C_MASTER);
}
static bool i2c_write_byte_master(uint8_t byte)
{
	bool ack=false;

	for (uint8_t i = 0; i<8; i++)
	{
	if((byte>>i) & 0x1) PIN_SET(SDA_PIN);
	PIN_SET(SCL_PIN);
	_delay_us(DELAY_I2C_MASTER);
	PIN_RESET(SCL_PIN);
	PIN_RESET(SDA_PIN);
	}

	PIN_SET_INPUT(SDA_PIN);
	PIN_SET(SCL_PIN);
	_delay_us(DELAY_I2C_MASTER);
	ack = PIN_IS_RESET(SDA_PIN);
	PIN_RESET(SCL_PIN);
	PIN_SET_OUTPUT(SDA_PIN);

	return ack;
}
bool i2c_write_master(uint8_t *addr, uint8_t *byte, uint8_t byte_num)
{
	i2c_gen_start_master();
	if(i2c_write_byte_master(*addr))
			{
		for(uint8_t i = 0; i <= byte_num; i++)
		{
			if(i2c_write_byte_master(byte[i]))
				continue;
			else return false;
		}
			}
	return false;
}

#if defined I2C_SLAVE_MODE
ISR(PCINT1_vect)
 {
	static uint8_t bit_num;
	static uint8_t byte;
	static uint8_t byte_num;
	uint8_t temp = 0;
	static bool read_command_detect;
	static bool my_address_detect;
	static enum i2c_state i2c_state = WHAIT_START;

	bool SCL_new_state = PIN_IS_SET(SCL_PIN);
	bool SDA_new_state = PIN_IS_SET(SDA_PIN);

	if (SCL_new_state != SCL_old_state) {
		if (SCL_new_state) {
			SCL_rising_edge_flag = true;
			SCL_falling_edge_flag = false;
		} else {
			SCL_rising_edge_flag = false;
			SCL_falling_edge_flag = true;
		}
		SCL_old_state = SCL_new_state;
	}

	else if (SDA_new_state != SDA_old_state) {
		if (SDA_new_state) {
			SDA_rising_edge_flag = true;
			SDA_falling_edge_flag = false;
		} else {
			SDA_rising_edge_flag = false;
			SDA_falling_edge_flag = true;
		}
		SDA_old_state = SDA_new_state;
	}

	if (SDA_falling_edge_flag && !SCL_falling_edge_flag) {
		i2c_state = START;
	} else if (SDA_rising_edge_flag && SCL_rising_edge_flag) {
		i2c_state = STOP;
	}

	switch (i2c_state) {
	case WHAIT_START:
		break;
	case START:
		i2c_state = BIT_RW_P1;
		bit_num = 0;
		byte = 0;
		byte_num = 0;
		read_command_detect = false;
		my_address_detect = false;
	case BIT_RW_P1:
		BIT_P1: if (!read_command_detect) {
			if (SCL_rising_edge_flag) {
				PIN_SET_INPUT(SDA_PIN);
				byte = byte << bit_num;
				byte = byte | PIN_IS_SET(SDA_PIN);
				i2c_state = BIT_RW_P2;
			}
		} else {
			if (SCL_falling_edge_flag) {
				PIN_SET_OUTPUT(SDA_PIN);
				if ((byte << bit_num) & 0x80)
					PIN_SET(SDA_PIN);
				else
					PIN_RESET(SDA_PIN);
				i2c_state = BIT_RW_P2;
			}
		}
		break;
	case BIT_RW_P2:
		if (SCL_falling_edge_flag) {
			if (read_command_detect) {
				PIN_RESET(SDA_PIN);
			}

			bit_num++;
			if (bit_num < 8) {
				i2c_state = BIT_RW_P1;
				goto BIT_P1;
			} else
				i2c_state = ACK_P1;
		} else
			break;
	case ACK_P1:
		if (!my_address_detect) {
			temp = byte >> 1;
			if (temp == MY_I2C_ADDRESS) {

				read_command_detect = byte & 0x1;
				i2c_state = ACK_P2;
			} else {
				//no my address
				i2c_state = WHAIT_START;
				break;
			}

			if (my_address_detect) {
				if (!read_command_detect) {
					PIN_SET_OUTPUT(SDA_PIN);
					PIN_RESET(SDA_PIN);
				} else {
					PIN_SET_INPUT(SDA_PIN);
				}
			}
		}
	case ACK_P2:
		if (SCL_rising_edge_flag) {
			if (!read_command_detect) {
				i2c_state = ACK_P3;
			} else {
				if (PIN_IS_SET(SDA_PIN)) {
					//master no ACK
					i2c_state = WHAIT_START;
				}
			}
		}
		break;
	case ACK_P3:
		if (SCL_falling_edge_flag) {

			if (!read_command_detect) {
				PIN_SET_INPUT(SDA_PIN);
				i2c_write_buffer[byte_num] = byte;
				byte_num++;
				i2c_state = BIT_RW_P1;
				break;
			} else {
				byte = i2c_read_buffer[byte_num];
				byte_num++;
				i2c_state = BIT_RW_P1;
				goto BIT_P1;
			}
		}
		break;
	case STOP:
		bit_num = 0;
		byte = 0;
		byte_num = 0;
		read_command_detect = false;
		my_address_detect = false;
		PIN_SET_INPUT(SDA_PIN);
		i2c_state = WHAIT_START;
		break;
	}

}
#endif
