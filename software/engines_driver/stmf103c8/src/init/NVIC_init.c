/*
 * NVIC_init.c
 *
 *  Created on: 11.11.2019
 *      Author: fet
 */

#include"NVIC_init.h"
//#include "GPIO.h"
#include"stm32f10x.h"

void nvic_init(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

	// ustawienie modelu grupowego przerwan 2grupy po 8 podgrup
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	// struktura potrzebna do ustawieñ
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	// ustawienie i w³¹czenie przerwañ od timera1
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// przekazaie ustawieñ
	NVIC_Init(&NVIC_InitStructure);
//----------------------------- PRZERWANIA KLAWISZY -----------------------------


	//ustawienie i w³¹czenie przerwan zewnecznych od przycisków
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//przekazanie ustawien
	NVIC_Init(&NVIC_InitStructure);



	// okreslenie jak i na ktorej linni beda generowane przerwana
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;			// linia zerowa (A0,B0... itp)
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  // przerwanie
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // na opadajacym zboczu
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				// w³aczenie
	//przekazanie ustawieñ
	EXTI_Init(&EXTI_InitStructure);

//	//ustawienie i w³¹czenie przerwan zewnecznych od usart
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//przekazanie ustawien
	NVIC_Init(&NVIC_InitStructure);

	// ustawienie przerwanñ od dma
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//przekazanie ustawien
	NVIC_Init(&NVIC_InitStructure);


}
