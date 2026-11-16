/* Copyright (c) 2007 Fabian Greif
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
// ----------------------------------------------------------------------------


#include <avr/io.h>
#include <util/delay.h>

#include <stdint.h>
#include <avr/pgmspace.h>
#include "mcp2515.h"
#include "mcp2515_defs.h"
#include "spi.h"
#include "Board_PIN.h"
// -------------------------------------------------------------------------
void mcp2515_write_register( uint8_t adress, uint8_t data )
{
	SPI_M_Start();
	
	SPI_M_byte_io(SPI_WRITE);
	SPI_M_byte_io(adress);
	SPI_M_byte_io(data);
	
	SPI_M_Stop();
}

// -------------------------------------------------------------------------
uint8_t mcp2515_read_register(uint8_t adress)
{
	uint8_t data;
	
	SPI_M_Start();
	
	SPI_M_byte_io(SPI_READ);
	SPI_M_byte_io(adress);
	
	data = SPI_M_byte_io(0xff);	
	
	SPI_M_Stop();
	
	return data;
}

// -------------------------------------------------------------------------
void mcp2515_bit_modify(uint8_t adress, uint8_t mask, uint8_t data)
{
	SPI_M_Start();
	
	SPI_M_byte_io(SPI_BIT_MODIFY);
	SPI_M_byte_io(adress);
	SPI_M_byte_io(mask);
	SPI_M_byte_io(data);
	
	SPI_M_Stop();
}

// ----------------------------------------------------------------------------
uint8_t mcp2515_read_status(uint8_t type)
{
	uint8_t data;
	
	SPI_M_Start();
	
	SPI_M_byte_io(type);
	data = SPI_M_byte_io(0xff);
	
	SPI_M_Stop();
	
	return data;
}

// -------------------------------------------------------------------------
void mcp2515_init(enum CAN_SPEED canSpeed,enum CAN_CLOCK canClock, enum FILTER_SETUP filterSetup)
{
	SPI_M_Stop();
	
	PIN_SET_INPUT(MCP2515_INT);
	
	// reset MCP2515 by software reset.
	// After this he is in configuration mode.
	SPI_M_Start();
	SPI_M_byte_io(SPI_RESET);
	SPI_M_Stop();
	
	// wait a little bit until the MCP2515 has restarted
	_delay_us(10);
	
	// load CNF1..3 Register
	uint8_t cfg1, cfg2, cfg3;
	    switch (canClock)
	    {
	        case (MCP_8MHZ):
	        switch (canSpeed)
	        {
	            case (CAN_5KBPS):                                               //   5KBPS
	            cfg1 = MCP_8MHz_5kBPS_CFG1;
	            cfg2 = MCP_8MHz_5kBPS_CFG2;
	            cfg3 = MCP_8MHz_5kBPS_CFG3;
	            break;

	            case (CAN_10KBPS):                                              //  10KBPS
	            cfg1 = MCP_8MHz_10kBPS_CFG1;
	            cfg2 = MCP_8MHz_10kBPS_CFG2;
	            cfg3 = MCP_8MHz_10kBPS_CFG3;
	            break;

	            case (CAN_20KBPS):                                              //  20KBPS
	            cfg1 = MCP_8MHz_20kBPS_CFG1;
	            cfg2 = MCP_8MHz_20kBPS_CFG2;
	            cfg3 = MCP_8MHz_20kBPS_CFG3;
	            break;

	            case (CAN_31K25BPS):                                            //  31.25KBPS
	            cfg1 = MCP_8MHz_31k25BPS_CFG1;
	            cfg2 = MCP_8MHz_31k25BPS_CFG2;
	            cfg3 = MCP_8MHz_31k25BPS_CFG3;
	            break;

	            case (CAN_33KBPS):                                              //  33.333KBPS
	            cfg1 = MCP_8MHz_33k3BPS_CFG1;
	            cfg2 = MCP_8MHz_33k3BPS_CFG2;
	            cfg3 = MCP_8MHz_33k3BPS_CFG3;
	            break;

	            case (CAN_40KBPS):                                              //  40Kbps
	            cfg1 = MCP_8MHz_40kBPS_CFG1;
	            cfg2 = MCP_8MHz_40kBPS_CFG2;
	            cfg3 = MCP_8MHz_40kBPS_CFG3;
	            break;

	            case (CAN_50KBPS):                                              //  50Kbps
	            cfg1 = MCP_8MHz_50kBPS_CFG1;
	            cfg2 = MCP_8MHz_50kBPS_CFG2;
	            cfg3 = MCP_8MHz_50kBPS_CFG3;
	            break;

	            case (CAN_80KBPS):                                              //  80Kbps
	            cfg1 = MCP_8MHz_80kBPS_CFG1;
	            cfg2 = MCP_8MHz_80kBPS_CFG2;
	            cfg3 = MCP_8MHz_80kBPS_CFG3;
	            break;

	            case (CAN_95K2BPS):                                              //  95.2Kbps
	            cfg1 = MCP_8MHz_95k2BPS_CFG1;
	            cfg2 = MCP_8MHz_95k2BPS_CFG2;
	            cfg3 = MCP_8MHz_95k2BPS_CFG3;
	            break;

	            case (CAN_100KBPS):                                             // 100Kbps
	            cfg1 = MCP_8MHz_100kBPS_CFG1;
	            cfg2 = MCP_8MHz_100kBPS_CFG2;
	            cfg3 = MCP_8MHz_100kBPS_CFG3;
	            break;

	            case (CAN_125KBPS):                                             // 125Kbps
	            cfg1 = MCP_8MHz_125kBPS_CFG1;
	            cfg2 = MCP_8MHz_125kBPS_CFG2;
	            cfg3 = MCP_8MHz_125kBPS_CFG3;
	            break;

	            case (CAN_200KBPS):                                             // 200Kbps
	            cfg1 = MCP_8MHz_200kBPS_CFG1;
	            cfg2 = MCP_8MHz_200kBPS_CFG2;
	            cfg3 = MCP_8MHz_200kBPS_CFG3;
	            break;

	            case (CAN_250KBPS):                                             // 250Kbps
	            cfg1 = MCP_8MHz_250kBPS_CFG1;
	            cfg2 = MCP_8MHz_250kBPS_CFG2;
	            cfg3 = MCP_8MHz_250kBPS_CFG3;
	            break;

	            case (CAN_500KBPS):                                             // 500Kbps
	            cfg1 = MCP_8MHz_500kBPS_CFG1;
	            cfg2 = MCP_8MHz_500kBPS_CFG2;
	            cfg3 = MCP_8MHz_500kBPS_CFG3;
	            break;

	            case (CAN_1000KBPS):                                            //   1Mbps
	            cfg1 = MCP_8MHz_1000kBPS_CFG1;
	            cfg2 = MCP_8MHz_1000kBPS_CFG2;
	            cfg3 = MCP_8MHz_1000kBPS_CFG3;
	            break;
	        }
	        break;

	        case (MCP_16MHZ):
	        switch (canSpeed)
	        {
	            case (CAN_5KBPS):                                               //   5Kbps
	            cfg1 = MCP_16MHz_5kBPS_CFG1;
	            cfg2 = MCP_16MHz_5kBPS_CFG2;
	            cfg3 = MCP_16MHz_5kBPS_CFG3;
	            break;

	            case (CAN_10KBPS):                                              //  10Kbps
	            cfg1 = MCP_16MHz_10kBPS_CFG1;
	            cfg2 = MCP_16MHz_10kBPS_CFG2;
	            cfg3 = MCP_16MHz_10kBPS_CFG3;
	            break;

	            case (CAN_20KBPS):                                              //  20Kbps
	            cfg1 = MCP_16MHz_20kBPS_CFG1;
	            cfg2 = MCP_16MHz_20kBPS_CFG2;
	            cfg3 = MCP_16MHz_20kBPS_CFG3;
	            break;

	            case (CAN_33KBPS):                                              //  33.333Kbps
	            cfg1 = MCP_16MHz_33k3BPS_CFG1;
	            cfg2 = MCP_16MHz_33k3BPS_CFG2;
	            cfg3 = MCP_16MHz_33k3BPS_CFG3;
	            break;

	            case (CAN_40KBPS):                                              //  40Kbps
	            cfg1 = MCP_16MHz_40kBPS_CFG1;
	            cfg2 = MCP_16MHz_40kBPS_CFG2;
	            cfg3 = MCP_16MHz_40kBPS_CFG3;
	            break;

	            case (CAN_50KBPS):                                              //  50Kbps
	            cfg1 = MCP_16MHz_50kBPS_CFG1;
	            cfg2 = MCP_16MHz_50kBPS_CFG2;
	            cfg3 = MCP_16MHz_50kBPS_CFG3;
	            break;

	            case (CAN_80KBPS):                                              //  80Kbps
	            cfg1 = MCP_16MHz_80kBPS_CFG1;
	            cfg2 = MCP_16MHz_80kBPS_CFG2;
	            cfg3 = MCP_16MHz_80kBPS_CFG3;
	            break;

	            case (CAN_83K3BPS):                                             //  83.333Kbps
	            cfg1 = MCP_16MHz_83k3BPS_CFG1;
	            cfg2 = MCP_16MHz_83k3BPS_CFG2;
	            cfg3 = MCP_16MHz_83k3BPS_CFG3;
	            break;

	            case (CAN_100KBPS):                                             // 100Kbps
	            cfg1 = MCP_16MHz_100kBPS_CFG1;
	            cfg2 = MCP_16MHz_100kBPS_CFG2;
	            cfg3 = MCP_16MHz_100kBPS_CFG3;
	            break;

	            case (CAN_125KBPS):                                             // 125Kbps
	            cfg1 = MCP_16MHz_125kBPS_CFG1;
	            cfg2 = MCP_16MHz_125kBPS_CFG2;
	            cfg3 = MCP_16MHz_125kBPS_CFG3;
	            break;

	            case (CAN_200KBPS):                                             // 200Kbps
	            cfg1 = MCP_16MHz_200kBPS_CFG1;
	            cfg2 = MCP_16MHz_200kBPS_CFG2;
	            cfg3 = MCP_16MHz_200kBPS_CFG3;
	            break;

	            case (CAN_250KBPS):                                             // 250Kbps
	            cfg1 = MCP_16MHz_250kBPS_CFG1;
	            cfg2 = MCP_16MHz_250kBPS_CFG2;
	            cfg3 = MCP_16MHz_250kBPS_CFG3;
	            break;

	            case (CAN_500KBPS):                                             // 500Kbps
	            cfg1 = MCP_16MHz_500kBPS_CFG1;
	            cfg2 = MCP_16MHz_500kBPS_CFG2;
	            cfg3 = MCP_16MHz_500kBPS_CFG3;
	            break;

	            case (CAN_1000KBPS):                                            //   1Mbps
	            cfg1 = MCP_16MHz_1000kBPS_CFG1;
	            cfg2 = MCP_16MHz_1000kBPS_CFG2;
	            cfg3 = MCP_16MHz_1000kBPS_CFG3;
	            break;
	        }
	        break;

	        case (MCP_20MHZ):
	        switch (canSpeed)
	        {
	            case (CAN_33KBPS):                                              //  33.333Kbps
	            cfg1 = MCP_20MHz_33k3BPS_CFG1;
	            cfg2 = MCP_20MHz_33k3BPS_CFG2;
	            cfg3 = MCP_20MHz_33k3BPS_CFG3;
		    break;

	            case (CAN_40KBPS):                                              //  40Kbps
	            cfg1 = MCP_20MHz_40kBPS_CFG1;
	            cfg2 = MCP_20MHz_40kBPS_CFG2;
	            cfg3 = MCP_20MHz_40kBPS_CFG3;
	            break;

	            case (CAN_50KBPS):                                              //  50Kbps
	            cfg1 = MCP_20MHz_50kBPS_CFG1;
	            cfg2 = MCP_20MHz_50kBPS_CFG2;
	            cfg3 = MCP_20MHz_50kBPS_CFG3;
	            break;

	            case (CAN_80KBPS):                                              //  80Kbps
	            cfg1 = MCP_20MHz_80kBPS_CFG1;
	            cfg2 = MCP_20MHz_80kBPS_CFG2;
	            cfg3 = MCP_20MHz_80kBPS_CFG3;
	            break;

	            case (CAN_83K3BPS):                                             //  83.333Kbps
	            cfg1 = MCP_20MHz_83k3BPS_CFG1;
	            cfg2 = MCP_20MHz_83k3BPS_CFG2;
	            cfg3 = MCP_20MHz_83k3BPS_CFG3;
		    break;

	            case (CAN_100KBPS):                                             // 100Kbps
	            cfg1 = MCP_20MHz_100kBPS_CFG1;
	            cfg2 = MCP_20MHz_100kBPS_CFG2;
	            cfg3 = MCP_20MHz_100kBPS_CFG3;
	            break;

	            case (CAN_125KBPS):                                             // 125Kbps
	            cfg1 = MCP_20MHz_125kBPS_CFG1;
	            cfg2 = MCP_20MHz_125kBPS_CFG2;
	            cfg3 = MCP_20MHz_125kBPS_CFG3;
	            break;

	            case (CAN_200KBPS):                                             // 200Kbps
	            cfg1 = MCP_20MHz_200kBPS_CFG1;
	            cfg2 = MCP_20MHz_200kBPS_CFG2;
	            cfg3 = MCP_20MHz_200kBPS_CFG3;
	            break;

	            case (CAN_250KBPS):                                             // 250Kbps
	            cfg1 = MCP_20MHz_250kBPS_CFG1;
	            cfg2 = MCP_20MHz_250kBPS_CFG2;
	            cfg3 = MCP_20MHz_250kBPS_CFG3;
	            break;

	            case (CAN_500KBPS):                                             // 500Kbps
	            cfg1 = MCP_20MHz_500kBPS_CFG1;
	            cfg2 = MCP_20MHz_500kBPS_CFG2;
	            cfg3 = MCP_20MHz_500kBPS_CFG3;
	            break;

	            case (CAN_1000KBPS):                                            //   1Mbps
	            cfg1 = MCP_20MHz_1000kBPS_CFG1;
	            cfg2 = MCP_20MHz_1000kBPS_CFG2;
	            cfg3 = MCP_20MHz_1000kBPS_CFG3;
	            break;
	        }
	        break;
	    }
	mcp2515_write_register(CNF1, cfg1);
	mcp2515_write_register(CNF2, cfg2);
	mcp2515_write_register(CNF3, cfg3);

	// deaktivate the RXnBF Pins (High Impedance State)
	mcp2515_write_register(BFPCTRL, 0);
	
	mcp2515_write_register(CANINTE, (1<<RX0IE)|(1<<RX1IE));

	// set TXnRTS as inputs
	mcp2515_write_register(TXRTSCTRL, 0);
	
	// turn off filters => receive any message
	switch (filterSetup) {
		case FILTER_OFF:
			mcp2515_write_register(RXB0CTRL, (1<<RXM1)|(1<<RXM0));
			mcp2515_write_register(RXB1CTRL, (1<<RXM1)|(1<<RXM0));
			break;
		case FILTER_ON_STANDART_ID:
			mcp2515_write_register(RXB0CTRL, (1<<RXM0));
			mcp2515_write_register(RXB1CTRL, (1<<RXM0));

			mcp2515_write_register(RXF0SIDH,((uint8_t)(MCP2515_FILTER0_ID >> 3)));
			mcp2515_write_register(RXF0SIDL,((uint8_t)(MCP2515_FILTER0_ID << 5)));
			mcp2515_write_register(RXF1SIDH,((uint8_t)(MCP2515_FILTER1_ID >> 3)));
			mcp2515_write_register(RXF1SIDL,((uint8_t)(MCP2515_FILTER1_ID << 5)));

			mcp2515_write_register(RXF2SIDH,((uint8_t)(MCP2515_FILTER2_ID >> 3)));
			mcp2515_write_register(RXF2SIDL,((uint8_t)(MCP2515_FILTER2_ID << 5)));
			mcp2515_write_register(RXF3SIDH,((uint8_t)(MCP2515_FILTER3_ID >> 3)));
			mcp2515_write_register(RXF3SIDL,((uint8_t)(MCP2515_FILTER3_ID << 5)));

			mcp2515_write_register(RXF4SIDH,((uint8_t)(MCP2515_FILTER4_ID >> 3)));
			mcp2515_write_register(RXF4SIDL,((uint8_t)(MCP2515_FILTER4_ID << 5)));
			mcp2515_write_register(RXF5SIDH,((uint8_t)(MCP2515_FILTER5_ID >> 3)));
			mcp2515_write_register(RXF5SIDL,((uint8_t)(MCP2515_FILTER5_ID << 5)));

			mcp2515_write_register(RXM0SIDH,((uint8_t)(MCP2515_MASK0_ID >> 3)));
			mcp2515_write_register(RXM0SIDL,((uint8_t)(MCP2515_MASK0_ID << 5)));
			mcp2515_write_register(RXM1SIDH,((uint8_t)(MCP2515_MASK1_ID >> 3)));
			mcp2515_write_register(RXM1SIDL,((uint8_t)(MCP2515_MASK1_ID << 5)));
			break;
		case FILTER_ON_EXTENDED_ID:
			mcp2515_write_register(RXB0CTRL, (1<<RXM1));
			mcp2515_write_register(RXB1CTRL, (1<<RXM1));

			mcp2515_write_register(RXF0SIDH, (uint8_t)(MCP2515_FILTER0_ID >> 21));
			mcp2515_write_register(RXF0SIDL, (uint8_t)((((MCP2515_FILTER0_ID >> 13) & 0b11100000)|((MCP2515_FILTER0_ID >> 16) & 0b00000011))|(1<<EXIDE)));
			mcp2515_write_register(RXF0EID8, (uint8_t)(MCP2515_FILTER0_ID >> 8));
			mcp2515_write_register(RXF0EID0, (uint8_t)(MCP2515_FILTER0_ID));

			mcp2515_write_register(RXF1SIDH, (uint8_t)(MCP2515_FILTER1_ID >> 21));
			mcp2515_write_register(RXF1SIDL, (uint8_t)((((MCP2515_FILTER1_ID >> 13) & 0b11100000)|((MCP2515_FILTER1_ID >> 16) & 0b00000011))|(1<<EXIDE)));
			mcp2515_write_register(RXF1EID8, (uint8_t)(MCP2515_FILTER1_ID >> 8));
			mcp2515_write_register(RXF1EID0, (uint8_t)(MCP2515_FILTER1_ID));

			mcp2515_write_register(RXF2SIDH, (uint8_t)(MCP2515_FILTER2_ID >> 21));
			mcp2515_write_register(RXF2SIDL, (uint8_t)((((MCP2515_FILTER2_ID >> 13) & 0b11100000)|((MCP2515_FILTER2_ID >> 16) & 0b00000011))|(1<<EXIDE)));
			mcp2515_write_register(RXF2EID8, (uint8_t)(MCP2515_FILTER2_ID >> 8));
			mcp2515_write_register(RXF2EID0, (uint8_t)(MCP2515_FILTER2_ID));

			mcp2515_write_register(RXF3SIDH, (uint8_t)(MCP2515_FILTER3_ID >> 21));
			mcp2515_write_register(RXF3SIDL, (uint8_t)((((MCP2515_FILTER3_ID >> 13) & 0b11100000)|((MCP2515_FILTER3_ID >> 16) & 0b00000011))|(1<<EXIDE)));
			mcp2515_write_register(RXF3EID8, (uint8_t)(MCP2515_FILTER3_ID >> 8));
			mcp2515_write_register(RXF3EID0, (uint8_t)(MCP2515_FILTER3_ID));

			mcp2515_write_register(RXF4SIDH, (uint8_t)(MCP2515_FILTER4_ID >> 21));
			mcp2515_write_register(RXF4SIDL, (uint8_t)((((MCP2515_FILTER4_ID >> 13) & 0b11100000)|((MCP2515_FILTER4_ID >> 16) & 0b00000011))|(1<<EXIDE)));
			mcp2515_write_register(RXF4EID8, (uint8_t)(MCP2515_FILTER4_ID >> 8));
			mcp2515_write_register(RXF4EID0, (uint8_t)(MCP2515_FILTER4_ID));

			mcp2515_write_register(RXF5SIDH, (uint8_t)(MCP2515_FILTER5_ID >> 21));
			mcp2515_write_register(RXF5SIDL, (uint8_t)((((MCP2515_FILTER5_ID >> 13) & 0b11100000)|((MCP2515_FILTER5_ID >> 16) & 0b00000011))|(1<<EXIDE)));
			mcp2515_write_register(RXF5EID8, (uint8_t)(MCP2515_FILTER5_ID >> 8));
			mcp2515_write_register(RXF5EID0, (uint8_t)(MCP2515_FILTER5_ID));

			mcp2515_write_register(RXM0SIDH, (uint8_t)(MCP2515_MASK0_ID >> 21));
			mcp2515_write_register(RXM0SIDL, (uint8_t)((((MCP2515_MASK0_ID >> 13) & 0b11100000)|((MCP2515_MASK0_ID >> 16) & 0b00000011))));
			mcp2515_write_register(RXM0EID8, (uint8_t)(MCP2515_MASK0_ID >> 8));
			mcp2515_write_register(RXM0EID0, (uint8_t)(MCP2515_MASK0_ID));

			mcp2515_write_register(RXM1SIDH, (uint8_t)(MCP2515_MASK1_ID >> 21));
			mcp2515_write_register(RXM1SIDL, (uint8_t)((((MCP2515_MASK1_ID >> 13) & 0b11100000)|((MCP2515_MASK1_ID >> 16) & 0b00000011))));
			mcp2515_write_register(RXM1EID8, (uint8_t)(MCP2515_MASK1_ID >> 8));
			mcp2515_write_register(RXM1EID0, (uint8_t)(MCP2515_MASK1_ID));
			break;
		case FILTER_ON_ALL_ID:
			break;
	}
	
	// reset device to normal mode
	mcp2515_write_register(CANCTRL, 0);
//	SET(LED2_HIGH);
}

// ----------------------------------------------------------------------------
// check if there are any new messages waiting

uint8_t mcp2515_check_message(void) {
	return (!PIN_IS_SET(MCP2515_INT));
}

// ----------------------------------------------------------------------------
// check if there is a free buffer to send messages

uint8_t mcp2515_check_free_buffer(void)
{
	uint8_t status = mcp2515_read_status(SPI_READ_STATUS);
	
	if ((status & 0x54) == 0x54) {
		// all buffers used
		return false;
	}
	
	return true;
}

// ----------------------------------------------------------------------------
uint8_t mcp2515_get_message(tCAN *message)
{
	// read status
	uint8_t status = mcp2515_read_status(SPI_RX_STATUS);
	uint8_t addr;
	uint8_t t;
	if (bit_is_set(status,6)) {
		// message in buffer 0
		addr = SPI_READ_RX;
	}
	else if (bit_is_set(status,7)) {
		// message in buffer 1
		addr = SPI_READ_RX | 0x04;
	}
	else {
		// Error: no message available
		return 0;
	}

	SPI_M_Start();
	SPI_M_byte_io(addr);
	
	// read id
	if (bit_is_set(status, 4)) {
		message->id  = (uint32_t)SPI_M_byte_io(0xff) << 21;
		uint8_t SIDL = SPI_M_byte_io(0xff);
		message->id |= ((uint32_t)(SIDL & 0b11100000)) << 18;
		message->id |= ((uint32_t)(SIDL & 0b00000011)) << 16;
		message->id |= (uint32_t)SPI_M_byte_io(0xff) << 8;
		message->id |= (uint32_t)SPI_M_byte_io(0xff);
	} else {
		message->id = (uint16_t) SPI_M_byte_io(0xff) << 3;
		message->id |= SPI_M_byte_io(0xff) >> 5;
		SPI_M_byte_io(0xff);
		SPI_M_byte_io(0xff);
	}
	
	// read DLC
	uint8_t length = SPI_M_byte_io(0xff) & 0x0f;
	
	message->header.length = length;
	message->header.rtr = (bit_is_set(status, 3)) ? 1 : 0;
	
	// read data
	for (t=0;t<length;t++) {
		message->data[t] = SPI_M_byte_io(0xff);
	}
	SPI_M_Stop();
	
	// clear interrupt flag
	if (bit_is_set(status, 6)) {
		mcp2515_bit_modify(CANINTF, (1<<RX0IF), 0);
	}
	else {
		mcp2515_bit_modify(CANINTF, (1<<RX1IF), 0);
	}
	
	return (status & 0x07) + 1;
}

// ----------------------------------------------------------------------------
uint8_t mcp2515_send_message(tCAN *message)
{
	uint8_t status = mcp2515_read_status(SPI_READ_STATUS);
	
	/* Statusbyte:
	 *
	 * Bit	Function
	 *  2	TXB0CNTRL.TXREQ
	 *  4	TXB1CNTRL.TXREQ
	 *  6	TXB2CNTRL.TXREQ
	 */
	uint8_t address;
	uint8_t t;
//	SET(LED2_HIGH);
	if (bit_is_clear(status, 2)) {
		address = 0x00;
	}
	else if (bit_is_clear(status, 4)) {
		address = 0x02;
	} 
	else if (bit_is_clear(status, 6)) {
		address = 0x04;
	}
	else {
		// all buffer used => could not send message
		return 0;
	}
	
	SPI_M_Start();
	SPI_M_byte_io(SPI_WRITE_TX | address);
	
	SPI_M_byte_io(message->id >> 3);
    SPI_M_byte_io(message->id << 5);
	
	SPI_M_byte_io(0);
	SPI_M_byte_io(0);
	
	uint8_t length = message->header.length & 0x0f;
	
	if (message->header.rtr) {
		// a rtr-frame has a length, but contains no data
		SPI_M_byte_io((1<<RTR) | length);
	}
	else {
		// set message length
		SPI_M_byte_io(length);
		
		// data
		for (t=0;t<length;t++) {
			SPI_M_byte_io(message->data[t]);
		}
	}
	SPI_M_Stop();
	
	_delay_us(1);
	
	// send message
	SPI_M_Start();
	address = (address == 0) ? 1 : address;
	SPI_M_byte_io(SPI_RTS | address);
	SPI_M_Stop();
	
	return address;
}
