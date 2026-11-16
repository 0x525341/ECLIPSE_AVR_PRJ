/*
 * DHT11.h
 *
 *  Created on: 26 янв. 2022 г.
 *      Author: r.sibgatullin
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include <avr/io.h>
#include <util/delay.h>

#ifndef TDHT_DDR_REG
#define TDHT_DDR_REG DDRB
#endif
#ifndef TDHT_PORT_REG
#define TDHT_PORT_REG PORTB
#endif
#ifndef TDHT_PIN_REG
#define TDHT_PIN_REG PINB
#endif
#ifndef TDHT_ONE_DELAY_US
#define TDHT_ONE_DELAY_US 30
#endif
#ifndef TDHT_READ_ATTEMPTS
#define TDHT_READ_ATTEMPTS 2
#endif

#define _tdhtPinLow(pin) TDHT_PORT_REG &= ~(1 << pin)
#define _tdhtPinHigh(pin) TDHT_PORT_REG |= (1 << pin)
#define _tdhtPinToOut(pin) TDHT_DDR_REG |= (1 << pin)
#define _tdhtPinToIn(pin) TDHT_DDR_REG &= ~(1 << pin)
#define _tdhtGetPinValue(pin) (TDHT_PIN_REG & (1 << pin))

static inline void _tdhtPinChangeWait(uint8_t pin, uint8_t currentState) {
  uint8_t counter = 0;
  while ((!_tdhtGetPinValue(pin)) != currentState && ++counter < 255) {
    _delay_us(1);
  }
}

static inline uint8_t _tdhtReadRawData(uint8_t pin, uint16_t sensorDelay, uint8_t *raw_data_buffer) {
  _delay_ms(sensorDelay);

  _tdhtPinToOut(pin);
  _tdhtPinLow(pin);
  _delay_ms(20);

  _tdhtPinToIn(pin);
  _tdhtPinHigh(pin);

  _tdhtPinChangeWait(pin, 1);
  _tdhtPinChangeWait(pin, 0);
  _tdhtPinChangeWait(pin, 1);

  for (uint8_t i = 0; i < 5; i++) {
    for (uint8_t j = 0; j < 8; j++) {
      _tdhtPinChangeWait(pin, 0);
      raw_data_buffer[i] <<= 1;
      _delay_us(TDHT_ONE_DELAY_US);
          if (_tdhtGetPinValue(pin)) {
            raw_data_buffer[i] |= 1;
            _tdhtPinChangeWait(pin, 1);
          }
    }
  }

  return (raw_data_buffer[4] == (uint8_t) (raw_data_buffer[0] + raw_data_buffer[1] + raw_data_buffer[2] + raw_data_buffer[3]));
}

static inline uint8_t tdhtReadDataDHT11(uint8_t pin, uint8_t *temperature, uint8_t *humidity) {
  uint8_t raw_data[5] = {0, 0, 0, 0, 0};
  for (uint8_t i = 0; i < TDHT_READ_ATTEMPTS; i++) {
    if (_tdhtReadRawData(pin, 1500, raw_data)) {
      *temperature = (uint8_t) raw_data[2];
      *humidity = (uint8_t) raw_data[0];
      if (i > 0) return 1;
    }
  }
  return 0;
}

static inline uint8_t tdhtReadDataDHT22(uint8_t pin, int16_t *temperature, uint16_t *humidity) {
  uint8_t raw_data[5] = {0, 0, 0, 0, 0};
  for (uint8_t i = 0; i < TDHT_READ_ATTEMPTS; i++) {
    if (_tdhtReadRawData(pin, 2000, raw_data)) {
      *temperature = ((uint16_t) raw_data[2] << 8) | raw_data[3];
      *humidity = ((uint16_t) raw_data[0] << 8) | raw_data[1];
      if (i > 0) return 1;
    }
  }
  return 0;
}
#endif /* INC_DHT11_H_ */
