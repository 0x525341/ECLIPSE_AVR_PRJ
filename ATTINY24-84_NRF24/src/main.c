#include "Board_PIN.h"
#include <util/delay.h>
#include "usart.h"
#include "rf24.h"
//#include "i2c_program.h"
#include <avr/interrupt.h>
#include "DHT11.h"
#include "main.h"
#include "stdint.h"

//#define RX_board_mode
#define TX_board_mode
//---------------------------------------------------------------------------------------------------------
#define TRIAC_PIN    A,0
#define TDHT_PIN     1
volatile uint8_t humidity_fan_on_val = 40;
//---------------------------------------------------------------------------------------------------------

enum power_state
{
	POWER_OFF,
	POWER_ON
};

uint8_t rx_data[data_size_addr_5] = {};

RF_IRQ_HANDLER
{
uint8_t status = rf_get_status();
if(status & (1<<RX_DR))
		{
	uint8_t pipe = ((status & 0xE)>>1);
	if ( pipe < 6)
	{
		rf_read_rx(rx_data);
		if(rx_data[3]==0xFF)
		{
			if(rx_data[0] == POWER_ON)
				PIN_SET(TRIAC_PIN);
			else PIN_RESET(TRIAC_PIN);
		}
		else humidity_fan_on_val = rx_data[3];
	}
		}
rf_reset_status();
}

int main(void)
 {
	_delay_ms(1000);
	uart_tx_init();
	rf_init(80, (1 << 0), true, false, 5, 15);
	rf_set_rx_addr(0, addr_0, data_size_addr_5);
	sei();

	uint8_t temperature=0;
	uint8_t humidity=0;
	enum power_state new_fan_state = POWER_OFF;
	enum power_state old_fan_state = POWER_OFF;
	while (1) {
		if (tdhtReadDataDHT11(TDHT_PIN, &temperature, &humidity))
		{
			if (humidity>humidity_fan_on_val)
			{
				new_fan_state = POWER_ON;
			}
			else {
				new_fan_state = POWER_OFF;
			}
		}

		if(new_fan_state != old_fan_state)
		{
			switch (new_fan_state)
			{
			case POWER_ON:
				PIN_SET_OUTPUT(TRIAC_PIN);
				PIN_RESET(TRIAC_PIN);
				break;
			case POWER_OFF:
				PIN_SET_INPUT(TRIAC_PIN);
				PIN_SET_INPUT_HIZ(TRIAC_PIN);
				break;
			}

			old_fan_state = new_fan_state;

			rf_set_tx_mode(addr_5, data_size_addr_5);
			uint8_t ack_data[data_size_addr_5];
			uint8_t tx_data[data_size_addr_5] = {(uint8_t)new_fan_state, temperature, humidity, humidity_fan_on_val};
			rf_send(tx_data, data_size_addr_5, ack_data);

			rf_set_rx_addr(0, addr_0, data_size_addr_5);
			rf_set_rx_mode();
		}
		uart_send_byte(humidity);
		uart_send_byte(' ');
		uart_send_byte(temperature);
		_delay_ms(1500);
	}
return 0;
}
