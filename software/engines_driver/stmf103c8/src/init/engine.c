/*
 * engine.c
 *
 *  Created on: 28.01.2021
 *      Author: fet
 */
#include"engine.h"
#include "stm32f10x.h"

uint16_t direction[] = {DIR0_PIN,
						DIR1_PIN,
						DIR2_PIN,
						DIR3_PIN,
#ifdef ADDITIONAL_STEPER_MOTOR
						DIR4_PIN
#endif
						};


void init_engines(Engine *wsk){
	// assign dir_pins array
	wsk->engine_dir_pins = direction;

	for(uint8_t i = 0; i<NR_OF_ENGINS; i++)
	{
		wsk->engine_counter[i] = 1600;
		wsk->engine_speed[i] = 1;
		wsk->engine_continuous_work[i] = 0;
		GPIO_SetBits(GPIOB,wsk->engine_dir_pins[i]) ;
	}
	// init servo_engine
	servo_dir = LEFT;
	servo_status = STOP;

}

