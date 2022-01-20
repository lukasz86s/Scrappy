/*
 * GPIO.c
 *
 *  Created on: 05.11.2019
 *      Author: fet
 */
#include "stm32f10x.h"
#include "GPIO.h"
void gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// konfiguracja portów i pinów
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	// konfiguracja pionow jako wejscie (do przerwan)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	//zapisanie ustawien dla portu A
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// poinformuj uc o zródle przerwan
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

//----------------------------------- konfuguraca pinow obslugujacych USART
	// konfiguracja pinu 9 jako wyjscie usart tx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	//zapisanie ustawien dla portu B
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// konfiguracja pinu 10 jako wejscie usart rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// konfiguracja pionow jako wejscie (do przerwan)
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//zapisanie ustawien dla portu B
	//GPIO_Init(GPIOB, &GPIO_InitStructure);

	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);


}
