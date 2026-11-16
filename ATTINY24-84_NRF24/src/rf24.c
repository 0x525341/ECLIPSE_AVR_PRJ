#include <avr/io.h>
#include <util/delay.h>
#include "rf24.h"
#include "spi.h"
#include <avr/interrupt.h>

//
// SPI functions
//

//#define ATMEGA
// For the ATTINY MISO and MOSI must be crossed over to communicate with the RF24. Thus,
// programming and operating has two different configurations that must be manually set on
// the breadboard programmer.
#define ATTINY

//
// functions for the RF24
//

static bool rf_if_command(uint8_t reg)
{
	uint8_t temp = reg & 0xF8;
    if(reg == W_TX_PAYLOAD || reg == R_RX_PAYLOAD
    		|| reg == FLUSH_TX || reg == FLUSH_RX
			|| reg == REUSE_TX_PL || reg == R_RX_PL_WID
			|| temp == W_ACK_PAYLOAD || reg == W_TX_PAYLOAD_NOACK
			|| reg == NOP)
    	return true;
    return false;
}

static void rf_write(uint8_t reg, uint8_t num, uint8_t* data) {
    if(rf_if_command(reg) == false)
        reg = W_REGISTER | reg;

    _delay_us(10);
    RESET_CSN;
    _delay_us(10);
    spi_write_byte(reg);
    _delay_us(10);

    for(int i=0;i<num;i++) {
        spi_write_byte(data[i]);
        _delay_us(10);
    }

    SET_CSN;
}

static void rf_read(uint8_t reg, uint8_t num, uint8_t* data) {
    if(rf_if_command(reg) == false)
        reg = R_REGISTER | reg;

    _delay_us(10);
    RESET_CSN;
    _delay_us(10);
    spi_write_byte(reg);
    _delay_us(10);

    for(int i=0;i<num;i++) {
        data[i] = spi_write_byte(NOP);
        _delay_us(10);
    }

    SET_CSN;
}

static void rf_write_register(uint8_t reg, uint8_t data) {
    rf_write(reg, 1, &data);
}

static uint8_t rf_read_register(uint8_t reg) {
    uint8_t data;
    rf_read(reg, 1, &data);
    return data;
}

static void rf_reset_bits_register(uint8_t reg, uint8_t bits) {
	uint8_t temp = rf_read_register(reg);
	temp &= ~(bits);
    rf_write(reg, 1, &temp);
}

static void rf_set_bits_register(uint8_t reg, uint8_t bits) {
	uint8_t temp = rf_read_register(reg);
	temp |=(bits);
    rf_write(reg, 1, &temp);
}

uint8_t rf_get_status(void)
{
    return rf_read_register(STATUS);
}

void rf_reset_status(void)
{
	    // STATUS reset current status
    rf_write_register(STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));
}

void rf_init(uint8_t CH, uint8_t pipe_mask, bool RX_mode, bool dynamic_ack, uint8_t retr_delay, uint8_t retr_count) {

    _delay_ms(500);
    spi_init();
    PIN_SET_OUTPUT(NRF24_CSN_PIN);
    PIN_SET_OUTPUT(NRF24_CE_PIN);
    PIN_SET_INPUT(NRF24_IRQ_PIN);
    SET_CSN;
    RESET_CE;
    _delay_ms(500);

    // CONFIG
    rf_set_bits_register(CONFIG, (1<<CRCO));//TX_MODE_default, CRC 2byte, EN_IR

    if(RX_mode)
    rf_set_bits_register(CONFIG, (1<<PRIM_RX));

    // EN_AA
    rf_set_bits_register(EN_AA, pipe_mask);//Enable auto acknowledgement data pipe
	
	// EN_RXADDR
    rf_set_bits_register(EN_RXADDR, pipe_mask);//Enabled RX Addresses.
	
	// SETUP_AW
    rf_reset_bits_register(SETUP_AW, (1<<1)); //3bytes Address Widths

    // SETUP_RETR
    rf_write_register(SETUP_RETR, (retr_delay&0xf)<<ARD | (retr_count&0xf)<<ARC);//Auto Retransmit Delay,Auto Retransmit Count 15MAX_VAL!

    // RF_CH set channel
    rf_write_register(RF_CH, CH);

    // RF_SETUP set data rate to 1MB/s, -0dBm RF_PWR
    rf_reset_bits_register(RF_SETUP,(1<<RF_DR_HIGH));

	rf_reset_status();

//   // toggle
    rf_write_register(ACTIVATE, 0x73);

    // FEATURE DYNPD disable dynamic payload
    if (dynamic_ack)
    {
        rf_write_register(FEATURE, (1<<EN_ACK_PAY) | (1<<EN_DPL));
        rf_write_register(DYNPD, pipe_mask);
    }
    else
    {
    rf_write_register(FEATURE, 0);
    rf_write_register(DYNPD, 0);
    }

    // flush
    rf_write(FLUSH_TX, 0, NULL);
    rf_write(FLUSH_RX, 0, NULL);

    // power up
    rf_set_bits_register(CONFIG, (1<<PWR_UP));
    _delay_ms(5);

    if(RX_mode)
    	SET_CE;
}

static void rf_set_tx_addr(uint8_t *addr)
{
    rf_write(TX_ADDR, RF24_addr_size, addr);
}

void rf_set_rx_addr(uint8_t pipe_num, uint8_t *addr, uint8_t data_size)
{
	uint8_t addr_size = RF24_addr_size;
	if (pipe_num>1)
		addr_size = 1;
	rf_write((10+pipe_num), addr_size, addr);
    rf_write_register((17+pipe_num), data_size);
}

void rf_set_tx_mode(uint8_t *tx_addr, uint8_t data_size)
{
	rf_reset_bits_register(CONFIG,(1<<PRIM_RX));
	rf_set_tx_addr(tx_addr);
	rf_set_rx_addr(0, tx_addr, data_size);
	RESET_CE;
	_delay_us(150);

	GIMSK &= ~(1 << INT0);//disable int0

    rf_write(FLUSH_TX, 0, NULL);
    rf_write(FLUSH_RX, 0, NULL);
}
void rf_set_rx_mode(void)
{
	rf_set_bits_register(CONFIG,(1<<PRIM_RX));
	SET_CE;
	_delay_us(150);

	MCUCR |=(1 << ISC01);//falling edge of INT0
	GIMSK |=(1 << INT0);//enable int0

    rf_write(FLUSH_TX, 0, NULL);
    rf_write(FLUSH_RX, 0, NULL);
}

void rf_send_ack_data(uint8_t pipe_num, uint8_t *data, uint8_t num)
{
	rf_write(W_ACK_PAYLOAD|pipe_num, num, data);
}

void rf_read_rx(uint8_t *rx_data)
{
    rf_read(R_RX_PAYLOAD, rf_read_register(R_RX_PL_WID), rx_data);
}

enum rf_send_status rf_send(uint8_t* tx_data, size_t num, uint8_t* ack_data) {
    enum rf_send_status ret = rf_send_success;

    rf_write(FLUSH_TX, 0, NULL);
    rf_write(W_TX_PAYLOAD, num, tx_data);

    SET_CE;
    _delay_us(10);
    RESET_CE;

    uint8_t cnt=0;
    while(PIN_IS_SET(NRF24_IRQ_PIN))
    {
        _delay_us(1000);
        cnt++;

        if(cnt > 95) {
        	rf_write(FLUSH_TX, 0, NULL);
        	rf_write(FLUSH_RX, 0, NULL);
        	return rf_send_error_timeout;
        }
    }

    RESET_CE;
    uint8_t status = rf_get_status();

    if( status & (1<<MAX_RT) ) {
    	ret = rf_send_error_max_rt;
        rf_write(FLUSH_TX, 0, NULL);
        rf_set_bits_register(STATUS,(1<<MAX_RT));
    }

    if( status & (1<<RX_FULL) ) {
    	ret = rf_send_error_max_fifo;
        rf_write(FLUSH_TX, 0, NULL);
    }

    if( status & (1<<TX_DS) ) {
    	ret = rf_send_success;
        rf_set_bits_register(STATUS,(1<<TX_DS));
    }

    if( status & (1<<RX_DR) ) {
    	ret = rf_send_success_rx_ack;
    	rf_read_rx(ack_data);
        rf_set_bits_register(STATUS,(1<<RX_DR));
    }

    return ret;
}
