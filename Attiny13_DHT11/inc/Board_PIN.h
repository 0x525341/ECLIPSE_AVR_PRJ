#ifndef BOARD_PIN_H_
#define BOARD_PIN_H_
#include <avr/io.h>
//-------------------------------------------------------
#define DHT_PIN      B,3
#define TRIAC_PIN    B,4
//-------------------------------------------------------
#define	PIN_RESET(x)		_XRS(x)
#define	PIN_SET(x)			_XS(x)
#define	PIN_TOGGLE(x)		_XT(x)
#define	PIN_SET_OUTPUT(x)	_XSO(x)
#define	PIN_SET_INPUT(x)	_XSI(x)
#define	PIN_SET_INPUT_PULLUP(x)	  _XSIP(x)
#define	PIN_SET_INPUT_HIZ(x)	  _XSIZ(x)
#define	PIN_IS_SET(x)		_XR(x)
#define	PIN_STATE(x)		_XRI(x)

#define	PORT(x)			_port2(x)
#define	DDR(x)			_ddr2(x)
#define	PIN(x)			_pin2(x)

#define	_XRS(x,y)	PORT(x) &= ~(1<<y)
#define	_XS(x,y)	PORT(x) |= (1<<y)
#define	_XT(x,y)	PORT(x) ^= (1<<y)

#define	_XSO(x,y)	DDR(x) |= (1<<y)
#define	_XSI(x,y)	DDR(x) &= ~(1<<y)
#define _XSIP(x,y)  PORT(x) |= (1<<y)
#define _XSIZ(x,y)  PORT(x) &= ~(1<<y)

#define	_XR(x,y)	((PIN(x) & (1<<y)) != 0)
#define _XRI(x,y)   ((PIN(x) & (1<<y)))

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
