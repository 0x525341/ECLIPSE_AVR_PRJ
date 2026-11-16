#ifndef BOARD_PIN_H_
#define BOARD_PIN_H_
//-------------------------------------------------------
#define RELAY_DDR  DDRD
#define RELAY_PORT PORTD
#define RELAY_PIN  PIND3
//-------------------------------------------------------
#define INT0_DDR   DDRD
#define INT0_PORT  PORTD
#define INT0_PIN   PIND2
//-------------------------------------------------------
#define TRIAC_CONTROL_DDR  DDRD
#define TRIAC_CONTROL_PORT PORTD
#define TRIAC_CONTROL_PIN  PIND4
//-------------------------------------------------------
#define HALL_SENSOR_DDR    DDRB
#define HALL_SENSOR_PORT   PORTB
#define HALL_SENSOR_PIN    PINB0
//-------------------------------------------------------
#define POT_RESISTOR_DDR    DDRC
#define POT_RESISTOR_PORT   PORTC
#define POT_RESISTOR_SET_RPS_PIN             PINC5
#define POT_RESISTOR_PIN_1                   PINC4
#define POT_RESISTOR_PIN_2                   PINC3
#define POT_RESISTOR_PIN_3                   PINC2
//-------------------------------------------------------
void Board_pin_ini(void);
//-------------------------------------------------------
#endif /* BOARD_H_ */
