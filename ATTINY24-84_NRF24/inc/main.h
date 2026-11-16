/*
 * main.h
 *
 *  Created on: 6 дек. 2023 г.
 *      Author: Tramp
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

uint8_t addr_0[RF24_addr_size] = {0xC0, 0xB0, 0xA0};//Базовая станция
uint8_t addr_1[RF24_addr_size] = {0xB1, 0xB1, 0xA1};
#define data_size_addr_1 			4
uint8_t addr_2[RF24_addr_size] = {0xC2, 0xB1, 0xA1};//Датчик температуры-влажности улица
#define data_size_addr_2 			4
uint8_t addr_3[RF24_addr_size] = {0xD3, 0xB1, 0xA1};//вентилятор балкон
#define data_size_addr_3 			4
uint8_t addr_4[RF24_addr_size] = {0xE4, 0xB1, 0xA1};//Вентилятор кухня
#define data_size_addr_4 			4
uint8_t addr_5[RF24_addr_size] = {0xF5, 0xB1, 0xA1};//вентилятор ванная
#define data_size_addr_5 			4//состояние, температура, влажность, установка

#endif /* INC_MAIN_H_ */
