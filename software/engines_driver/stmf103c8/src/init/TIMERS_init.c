/*
 * TIM1_init.c
 *
 *  Created on: 11.11.2019
 *      Author: fet
 */
#include "TIMERS_init.h"



#define SysTick_Fre 72000000		//czestotliwosc 9MHz jesli dzielnik 8 (72/8 = 9)
//__IO uint16_t CC1 = 32768;


void tim1_init(void)
{
//--------------------- iniczjalizaja sys_tick --------------------------------------



	TIM_TimeBaseInitTypeDef TIM_TimBaseStructure;

	// ustweiwnia podstawy czasu
	TIM_TimBaseStructure.TIM_Period = 65535;
	TIM_TimBaseStructure.TIM_Prescaler = 72; // preskaler 72Mhz/720 = 100khz
	TIM_TimBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	 // inicjalizacja wybranych ustawien
	TIM_TimeBaseInit(TIM1, &TIM_TimBaseStructure);

	TIM_OCInitTypeDef TIM_OCInitStructure;
	// configuring first (1) channel
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = TIMER1_PERIOD;  				// set period
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);

	// ustweiwnia podstawy czasu
	TIM_TimBaseStructure.TIM_Period = 19999ul;
	TIM_TimBaseStructure.TIM_Prescaler = 72; // preskaler 72Mhz/720 = 100khz
	TIM_TimBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	 // inicjalizacja wybranych ustawien
	TIM_TimeBaseInit(TIM3, &TIM_TimBaseStructure);

	// configuring first channel
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1000ul;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//use forth channel to connect pwm with PA6
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	//bufor dusable - configuration changes take effect immediately.
	//for enabled - changes will take place on the next update
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	//wlacz przerwania od kana³ow
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
	// wlaczenie timera
	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM3, ENABLE);



}
