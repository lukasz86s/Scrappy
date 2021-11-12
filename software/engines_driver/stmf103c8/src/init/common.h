/*
 * common.h
 *
 *  Created on: 16.11.2019
 *      Author: fet
 */

#ifndef INIT_COMMON_H_
#define INIT_COMMON_H_
#include "stm32f10x.h"
#include "USART_init.h"
#include <string.h>
#include "engine.h"

#define START_OF_FRAME ':'

uint8_t command_recived;
char command[3];
uint8_t value;
//licznik danych
extern volatile uint8_t data_cnt;
volatile uint8_t data_position;

// stuktura operujaca na silnikach
Engine engines;


volatile void serva_set(void);

//----------------- probne timery-----------------------------
enum{
	sys_tim1,			// przyporz¹dkowanie odpowiednich nazw dla nomerow
	sys_tim2,			// w tablicy licznikow
	sys_tim3,
	sys_tim4,
	sys_tim5,
	max_timers			// ilosc licznikow
};

volatile uint16_t sys_tims[max_timers];

#endif /* INIT_COMMON_H_ */
