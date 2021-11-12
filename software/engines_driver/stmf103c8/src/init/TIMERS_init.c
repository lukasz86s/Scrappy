/*
 * TIM1_init.c
 *
 *  Created on: 11.11.2019
 *      Author: fet
 */
#include "stm32f10x.h"
#include "stm32f10x_it.h"

#define SysTick_Fre 72000000		//czestotliwosc 9MHz jesli dzielnik 8 (72/8 = 9)
__IO uint16_t CC1 = 32768;


void tim1_init(void)
{
//--------------------- iniczjalizaja sys_tick --------------------------------------



	TIM_TimeBaseInitTypeDef TIM_TimBaseStructure;

	// ustweiwnia podstawy czasu
	TIM_TimBaseStructure.TIM_Period = 65535;
	TIM_TimBaseStructure.TIM_Prescaler = 72; // preskaler 72Mhz/72 = 1000khz
	TIM_TimBaseStructure.TIM_ClockDivision = 0;
	TIM_TimBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	 // inicjalizacja wybranych ustawien
	TIM_TimeBaseInit(TIM1, &TIM_TimBaseStructure);


	TIM_OCInitTypeDef TIM_OCInitStructure;
	// konfiguracja kanalu 1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1000;  				// ustawienia licznika na 1ms co daje 1000khz/ 1000 = 1khz
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);

	//wlacz przerwania od kana³ow
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
	// wlaczenie timera
	TIM_Cmd(TIM1, ENABLE);



}
