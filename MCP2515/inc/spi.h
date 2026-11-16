
//-------------------------------------------------<                   axlib v1.1                    >----------------------------------------------------
//-------------------------------------------------<       Библиотека для работы с шиной SPI         >----------------------------------------------------
//-------------------------------------------------<    Кузнецов Алексей 2015 http://www.avrki.ru    >----------------------------------------------------


#ifndef SPI_H_
#define SPI_H_

#include <avr/interrupt.h>
#include <util/delay.h>
//#define SPI_SLAVE_MODE	// Режим Slave
#define FULL	0xFF
#ifdef SPDR

#if !defined (NULL)
#define NULL 0
#endif

// Включить SPI
#define SPI_ON			0x40

// Выключить SPI
#define SPI_OFF			0

// Режим Мастер
#define SPI_MASTER		0x10

// Режим Слейв
#define SPI_SLAVE		0

// Направление передачи (Младший разряд вперед)
#define SPI_DIRECT_LSB	0x20

// Направление передачи (Старший разряд вперед)
#define SPI_DIRECT_MSB	0

// Разрешить прерывание
#define SPI_INT_ON		0x80

// Запретить прерывание
#define SPI_INT_OFF		0

// Положительный тактовый сигнал
#define SPI_CPOL_LOW	0

// Отрицательный тактовый сигнал
#define SPI_CPOL_HIGHT	0x08

// Наростающий фронт сигнала
#define SPI_CPHA_1EDGE	0
// Спадающий фронт сигнала
#define SPI_CPHA_2EDGE	0x04

// Предделитель частоты (настройка скорости передачи для мастера)
#define SPI_PRESCALER_2		0
#define SPI_PRESCALER_4		1
#define SPI_PRESCALER_8		2
#define SPI_PRESCALER_16	3
#define SPI_PRESCALER_32	4
#define SPI_PRESCALER_64	5
#define SPI_PRESCALER_128	6
#define SPI_PRESCALER_NO	7

#if defined (__AVR_ATmega8__) // Если MEGA8

#define SPI_M_PORT		(DDRB |= (1 << 3) | (1 << 5) | (1 << 2))
#define SPI_M_SS_ON		(PORTB &= ~(1 << 2))
#define SPI_M_SS_OFF	(PORTB |= (1 << 2))

#define SPI_S_PORT		(DDRB |= (1 << 4))
#define SPI_S_SS_PIN	2

#elif defined (__AVR_ATmega328P__) // Если MEGA328P

#define SPI_M_PORT		(DDRB |= (1 << 3) | (1 << 5) | (1 << 2))
#define SPI_M_SS_ON		(PORTB &= ~(1 << 2))
#define SPI_M_SS_OFF	(PORTB |= (1 << 2))

#define SPI_S_PORT		(DDRB |= (1 << 4))
#define SPI_S_SS_PIN	2

#else // Для других МК

#define SPI_M_PORT		(DDRB |= ((1 << 5) | (1 << 7) | (1 << 4))
#define SPI_M_SS_ON		(PORTB &= ~(1 << 4))
#define SPI_M_SS_OFF	(PORTB |= (1 << 4))

#define SPI_S_PORT		(DDRB |= (1 << 6))
#define SPI_S_SS_PIN	4
#endif


// Флаг наличия данных в режиме Slave
#define FLAG_DATA	0
#define FLAG_OUT	1

// Объявление структур
typedef struct
{
	int8_t SPI_set;			// Включить/выключить SPI
	int8_t SPI_Mode;			// Выбор Мастер или Слейв
	int8_t SPI_Direct;		// Выбор направления передачи байта
	int8_t SPI_Prescaler;		// Выбор претделителя
	int8_t SPI_Polaric;		// Выбор полярности тактового сигнала
	int8_t SPI_Phase;			// Выбор фазы тактового сигнала
	
}SPI_InitTypeDef;

// Объявление функций
void SPI_M_Start(void);
void SPI_M_Stop(void);
// Инициализация SPI
void SPI_init(SPI_InitTypeDef *spi);

// Чтение байта и передача по шине для мастер устройства
uint8_t SPI_M_byte_io(uint8_t data);

// Чтение байта из шины для слейв устройства
uint8_t SPI_S_byte_io(uint8_t data, uint8_t timeout);

#else
#error "This is not the microcontroller SPI"
#endif // SPDR

#endif /* SPI_H_ */
