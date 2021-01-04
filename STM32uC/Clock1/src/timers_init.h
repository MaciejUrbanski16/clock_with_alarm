/*
 * timers_init.h
 *
 *  Created on: 31.12.2020
 *      Author: Admin
 */

#ifndef TIMERS_INIT_H_
#define TIMERS_INIT_H_

#include "stm32f1xx.h"

extern TIM_HandleTypeDef tim2; //interrupt every 1s

extern TIM_HandleTypeDef tim3; //interrupt every 200ms

extern TIM_HandleTypeDef tim4; //interrupt every 40ms

//initialization of timers
void tim2Init(void);
void tim3Init(void);
void tim4Init(void);

#endif /* TIMERS_INIT_H_ */
