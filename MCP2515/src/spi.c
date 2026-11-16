/*
 * spi.c
 *
 *  Created on: 13 янв. 2022 г.
 *      Author: r.sibgatullin
 */
#include "spi.h"

volatile uint8_t SPI_DATA = 0;
volatile uint8_t SPI_FLAG = 0;

#if defined(SPI_SLAVE_MODE)
	// Преывания по окончанию передачи для ведущего
	ISR(SPI_STC_vect)
	{
		SPI_DATA = SPDR;
		SPI_FLAG = 1;
	}
	#endif

void SPI_init(SPI_InitTypeDef *spi)
{
	if(spi == NULL)
	{
		return;
	}
	if(spi->SPI_Mode == SPI_MASTER)
	{
		SPI_M_PORT;
		SPI_M_SS_OFF;
		SPCR = (spi->SPI_set | spi->SPI_Mode | spi->SPI_Direct | spi->SPI_Polaric | spi->SPI_Phase);
	}
	else
	{
		SPI_S_PORT;
		SPCR = (spi->SPI_set | spi->SPI_Mode | spi->SPI_Direct | spi->SPI_Polaric | spi->SPI_Phase | (1 << SPIE));
		sei();
	}

	switch (spi->SPI_Prescaler)
	{
		case 0: // 2
		SPSR |= (1 << SPI2X);
		SPCR &= ~(1 << SPR0) | ~(1 << SPR1);
		break;

		case 1: // 4
		SPCR &= ~(1 << SPR0) | ~(1 << SPR1);
		break;

		case 2: // 8
		SPSR |= (1 << SPI2X);
		SPCR |= (1 << SPR0);
		SPCR &= ~(1 << SPR1);
		break;

		case 3: // 16
		SPCR |= (1 << SPR0);
		SPCR &= ~(1 << SPR1);
		break;

		case 4: // 32
		SPSR |= (1 << SPI2X);
		SPCR &= ~(1 << SPR0);
		SPCR |= (1 << SPR1);
		break;

		case 5: // 64
		SPCR &= ~(1 << SPR0);
		SPCR |= (1 << SPR1);
		break;

		case 6: //128
		SPCR |= 0x03;
		break;

		case 7: //SLAVE
		SPCR &= 0xFC;
		break;
	}
}

void SPI_M_Start(void)
{
	SPI_M_SS_ON;
}

void SPI_M_Stop(void)
{
	SPI_M_SS_OFF;
}

// Чтение байта и передача по шине для мастер устройства
uint8_t SPI_M_byte_io(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

void SPI_M_bytes_io(uint8_t datasize, uint8_t *data)
{
   while(datasize--){
      SPDR = *data;
      while(!(SPSR & (1<<SPIF)));
      *data++ = SPDR;
   }
}

// Чтение/запись байта из шины для слейв устройства
uint8_t SPI_S_byte_io(uint8_t data, uint8_t timeout)
{
	// Ожидания единицы на входе SS
	while(!(PINB & (1 << SPI_S_SS_PIN)));
	// Загрузка байта для передачи Мастеру
	SPDR = data;
	// Ожидания завершения обмена
	while(SPI_FLAG == 0)
	{
		if(!timeout)
		{
			// Выход с записью в регистр данных 0xFF по таймауту
			SPDR = 0xFF;
			return FULL;
		}
		_delay_ms(1);
		timeout--;
	}
	// Сброс флага обмена
	SPI_FLAG = 0;
	// Возврат значения принятого от Мастера
	return SPI_DATA;
}
