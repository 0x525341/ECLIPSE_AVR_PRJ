#ifndef BOARD_PIN_H_
#define BOARD_PIN_H_
#include <avr/io.h>

#define BUTTONS_CAN_ID   ((uint32_t)0x10438040)

#define MCP2515_FILTER0_ID BUTTONS_CAN_ID
#define MCP2515_FILTER1_ID BUTTONS_CAN_ID
#define MCP2515_FILTER2_ID BUTTONS_CAN_ID
#define MCP2515_FILTER3_ID BUTTONS_CAN_ID
#define MCP2515_FILTER4_ID BUTTONS_CAN_ID
#define MCP2515_FILTER5_ID BUTTONS_CAN_ID

#define MCP2515_MASK0_ID   BUTTONS_CAN_ID
#define MCP2515_MASK1_ID   BUTTONS_CAN_ID

#define NEXT_BUTTON_DATA 0x07
#define PREV_BUTTON_DATA 0x08
#define VOICE_BUTTON_DATA 0x09
//-------------------------------------------------------
#define GREEN_LED_PIN  B,1

#define BLUE_LED_PIN  B,0

#define RX1BF_PIN     D,3

#define RX0BF_PIN     D,4

#define MCP2515_INT     D,2

#define DOWN_PIN D,5

#define UP_PIN D,6

#define START_STOP_PIN D,7

//-------------------------------------------------------
#define	PIN_RESET(x)		_XRS(x)
#define	PIN_SET(x)			_XS(x)
#define	PIN_TOGGLE(x)		_XT(x)
#define	PIN_SET_OUTPUT(x)	_XSO(x)
#define	PIN_SET_INPUT(x)	_XSI(x)
#define	PIN_SET_INPUT_PULLUP(x)	  _XSIP(x)
#define	PIN_IS_SET(x)		_XR(x)

#define	PORT(x)			_port2(x)
#define	DDR(x)			_ddr2(x)
#define	PIN(x)			_pin2(x)

#define	_XRS(x,y)	PORT(x) &= ~(1<<y)
#define	_XS(x,y)	PORT(x) |= (1<<y)
#define	_XT(x,y)	PORT(x) ^= (1<<y)

#define	_XSO(x,y)	DDR(x) |= (1<<y)
#define	_XSI(x,y)	DDR(x) &= ~(1<<y)
#define _XSIP(x,y)  PORT(x) |= (1<<y)

#define	_XR(x,y)	((PIN(x) & (1<<y)) != 0)

#define	_port2(x)	PORT ## x
#define	_ddr2(x)	DDR ## x
#define	_pin2(x)	PIN ## x

#define	true	1
#define	false	0

#define	True	1
#define	False	0

//typedef	_Bool bool;
//typedef	boolean Bool;

//-------------------------------------------------------
#endif /* BOARD_PIN_H_ */
