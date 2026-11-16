/*
 * spi.c
 *
 *  Created on: 18 авг. 2023 г.
 *      Author: User
 */
#include "spi.h"
#include <avr/io.h>

void spi_init(void) {
    #ifdef ATMEGA
    // set SCK, MOSI, SS/CSN, CE (PB1)  as output
    DDRB |= (1<<DDB5) | (1<<DDB3) | (1<<DDB2) | (1<<DDB1);

    // enable SPI and set clock rate to fck/16
    SPCR |= (1<<SPE) | (1<<MSTR);
    #else
    //Connect MISO from RF24 to MOSI!! on muc (and vice versa)
    // SCK (PA4), CSN (PB2), CE (PB1)
    DDRA |= (1<<PA4) | (1<<PA5);

    USICR |= (1<<USIWM0) | (1<<USICS1) | (1<<USICLK);
    #endif

}

uint8_t spi_write_byte(uint8_t data) {
    #ifdef ATMEGA
    // write for data register
    SPDR = data;

    // wait for transmission to complete
    while( !(SPSR & (1<<SPIF)) ) ;

    return SPDR; // result stored in SPDR
    #else
    USIDR = data;
    USISR |= (1<<USIOIF);

    while( (USISR &  (1<<USIOIF)) == 0)
        USICR |= (1<<USITC);

    return USIDR;
    #endif
}
