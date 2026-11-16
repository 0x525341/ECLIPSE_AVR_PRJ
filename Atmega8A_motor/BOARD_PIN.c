#include "BOARD_PIN.h"

#include <avr/io.h>
//-------------------------------------------------------
void Board_pin_ini(void) {
	RELAY_DDR |= (1 << RELAY_PIN);
	TRIAC_CONTROL_DDR |= (1 << TRIAC_CONTROL_PIN);
	INT0_DDR &= ~(1 << INT0_PIN);
	HALL_SENSOR_DDR &= ~(1 << HALL_SENSOR_PIN);
	POT_RESISTOR_DDR &= ~(1 << POT_RESISTOR_SET_RPS_PIN)
			| (1 << POT_RESISTOR_PIN_1) | (1 << POT_RESISTOR_PIN_2)
			| (1 << POT_RESISTOR_PIN_3);
}
//-------------------------------------------------------
