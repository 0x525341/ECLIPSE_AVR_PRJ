#ifndef ADC_ATMEGA8_H_
#define ADC_ATMEGA8_H_
#include <stdint.h> //uint
//-------------------------------------------------------
void ADC_Init(void);
uint16_t ADC_convert (uint8_t adc_channel);
//-------------------------------------------------------
#endif /* ADC_ATMEGA8_H_ */
