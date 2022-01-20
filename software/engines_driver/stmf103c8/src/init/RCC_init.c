/*
 * RCC_init.c
 *
 *  Created on: 02.11.2019
 *      Author: fet
 */
#include "stm32f10x.h"
#include "RCC_init.h"
void RCC_init(void)
{
	// zminenna opisujaca rezultat uruchomienia HSE
	ErrorStatus HSEStartUpStatus;

	//reset ustawien RCC
	RCC_DeInit();

	// w³acz HSE
	RCC_HSEConfig(RCC_HSE_ON);

	//odczekaj na gotowsc HSE
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if(HSEStartUpStatus == SUCCESS)
	{
		// ustaw zwloke dla pamieci Flash
		//zaleznie od taktowania rdzenia:
		// 0 gdy< 24MHz, 1 gdy 24 - 48MHz i 2 powyrzej
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		FLASH_SetLatency(FLASH_Latency_2);

		//ustaw HCLK=SYSCLK
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		//ustaw PCLK2=HCLK
		RCC_PCLK2Config(RCC_HCLK_Div1);

		//ustaw PLCK1 = HCLK/2
		RCC_PCLK1Config(RCC_HCLK_Div2);

		//ustaw PLLCLK = HSE*9 czyli 8MHz *9 = 72MHz
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

		//wlacz PLL
		RCC_PLLCmd(ENABLE);

		//odczekaj na potwierdzenie uruchomienia PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

		//ustaw PLL jako zrodlo sygnalu zeagarowego
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		// odczekaj az PLL bedzie sygnalem systemu
		while(RCC_GetSYSCLKSource() != 0x08);

		//wklej tu kod do konfiguracji peryferii
		//wlacz taktowanie portu GPIO A, B, C i timera1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO|RCC_APB2Periph_TIM1|RCC_APB2Periph_USART1, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	}else
	{
		// mozna dodac obsluge jezeli by hse nie dalo gotowosci
	}

}

