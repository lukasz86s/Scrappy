/*
 * GPIO.c
 *
 *  Created on: 05.11.2019
 *      Author: fet
 */
#include "stm32f10x.h"
#include "GPIO.h"
#include "engine.h"

void gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// configuration of motor control pins
	GPIO_InitStructure.GPIO_Pin = ALL_ENGINES_PINS|ALL_DIRS_PINS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(ENGINES_PORT, &GPIO_InitStructure);


	// configuration of servo motor control pins (PWM MODE)
	GPIO_InitStructure.GPIO_Pin = SERVO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	//save settings for SETVO PORT
	GPIO_Init(SERVO_PORT, &GPIO_InitStructure);


//----------------------------------- configuration of UART pins
	// pin 9 out (tx)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	//save settings
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// pin 10 in (rx)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//save settings
	GPIO_Init(GPIOA, &GPIO_InitStructure);




}
