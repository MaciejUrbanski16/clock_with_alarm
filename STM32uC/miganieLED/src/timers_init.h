/*
 * timers_init.h
 *
 *  Created on: 31.12.2020
 *      Author: Admin
 */

#ifndef TIMERS_INIT_H_
#define TIMERS_INIT_H_

#include "stm32f1xx.h"

extern TIM_HandleTypeDef tim2; // trzeba co 1s przerwanie od timera
						//w tym przerwaniu ziekszymy wartoœc sekund o 1

extern TIM_HandleTypeDef tim3; //przerwanie od timera co 50ms w celu sprawdzenia stanu
						//przycisku konfiguracyjnego (eliminacja drgañ stykow)

extern TIM_HandleTypeDef tim4; //przerwanie od timera co 50ms w celu sprawdzenia stanu
						//przycisku konfiguracyjnego (eliminacja drgañ stykow)

void tim2Init(void);
void tim3Init(void);
void tim4Init(void);

#endif /* TIMERS_INIT_H_ */
