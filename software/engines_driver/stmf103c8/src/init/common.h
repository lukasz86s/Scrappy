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
#include "engine.h"
#include <string.h>


#define START_OF_FRAME ':'

#define CALCULATE_TIMER1_PERIOD(x) ((x)*500) - 1
// timer period in ms , put integer values
#define TIMER1_PERIOD CALCULATE_TIMER1_PERIOD(1)

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
	SYS_TIM1,			// przyporz¹dkowanie odpowiednich nazw dla nomerow
	SYS_TIM2,			// w tablicy licznikow
	SYS_TIM3,
	MAX_TIMERS			// ilosc licznikow
};

volatile uint16_t sys_tims[MAX_TIMERS];

#endif /* INIT_COMMON_H_ */
