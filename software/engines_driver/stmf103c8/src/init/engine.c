/*
 * engine.c
 *
 *  Created on: 28.01.2021
 *      Author: fet
 */
#include"engine.h"
#include "stm32f10x.h"

uint16_t dir[] = {DIR0_PIN, DIR1_PIN, DIR2_PIN, DIR3_PIN, DIR4_PIN};
void init_engines(Engine *wsk){
	for(uint8_t i = 0; i<NR_OF_ENGINS; i++)
	{
		wsk->engine_counter[i] = 32000;
		wsk->engine_speed[i] = 10;
		wsk->engine_continuous_work[i] = 0;
		wsk->engine_dir = dir;
	}
}

