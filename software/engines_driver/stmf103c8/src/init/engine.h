/*
 * engine.h
 *
 *  Created on: 28.01.2021
 *      Author: fet
 */

#ifndef INIT_ENGINE_H_
#define INIT_ENGINE_H_

#include "stm32f10x.h"


//if you want to use 5 engines
//#define ADDITIONAL_STEPER_MOTOR

#define E0_PIN GPIO_Pin_0
#define E1_PIN GPIO_Pin_1
#define E2_PIN GPIO_Pin_3
#define E3_PIN GPIO_Pin_4


#define DIR0_PIN GPIO_Pin_6
#define DIR1_PIN GPIO_Pin_7
#define DIR2_PIN GPIO_Pin_8
#define DIR3_PIN GPIO_Pin_9

#define ENGINES_PORT GPIOB

#define SERVO_PIN GPIO_Pin_6
#define SERVO_PORT GPIOA


#ifdef ADDITIONAL_STEPER_MOTOR

#define E4_PIN GPIO_Pin_5
#define DIR4_PIN GPIO_Pin_12
#define NR_OF_ENGINS 5

#define ALL_ENGINES_PINS GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_
#define ALL_DIRS_PINS GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12

#else

#define NR_OF_ENGINS 4

#define ALL_ENGINES_PINS GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4
#define ALL_DIRS_PINS GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9

#endif





typedef
		struct engine{
			volatile uint16_t engine_counter[NR_OF_ENGINS];
			volatile uint16_t engine_speed[NR_OF_ENGINS];
			volatile uint8_t engine_continuous_work[NR_OF_ENGINS] ;
			uint16_t *engine_dir_pins ;
}Engine;




void init_engines(Engine *wsk);

#endif /* INIT_ENGINE_H_ */
