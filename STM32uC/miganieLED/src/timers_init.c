/*
 * timers_init.c
 *
 *  Created on: 31.12.2020
 *      Author: Admin
 */


#include "timers_init.h"
#include "stm32f1xx.h"



TIM_HandleTypeDef tim2; //interrupt every 1s

TIM_HandleTypeDef tim3; //interrupt every 200ms

TIM_HandleTypeDef tim4; //interrupt every 40ms
void tim2Init(void){

	//configuration timer 2 to call interrupt every 1s
	tim2.Instance = TIM2;
	tim2.Init.Period = 1000 - 1; //1s to overflow counter
	tim2.Init.Prescaler = 8000 - 1;
	tim2.Init.ClockDivision = 0;
	tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2.Init.RepetitionCounter = 0;
	tim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim2);

	HAL_NVIC_EnableIRQ(TIM2_IRQn);//enable timer 2 interrupts
	HAL_TIM_Base_Start_IT(&tim2);

}
void tim3Init(void){

	//configuration of timer 3 to call interrupt every 200ms
	tim3.Instance = TIM3;
	tim3.Init.Period = 200-1; //after 200ms overflow and interrupt
	tim3.Init.Prescaler = 8000-1;
	tim3.Init.ClockDivision = 0;
	tim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim3.Init.RepetitionCounter = 0;
	tim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim3);

	HAL_NVIC_EnableIRQ(TIM3_IRQn);//enable interrupts from timer 3
	HAL_TIM_Base_Start_IT(&tim3);


}
void tim4Init(void){

	//configuration of timer 4 to call interrupt every 40ms
	tim4.Instance = TIM4;
	tim4.Init.Period = 40-1; //after 40ms overflow and interruapt
	tim4.Init.Prescaler = 8000-1;
	tim4.Init.ClockDivision = 0;
	tim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim4.Init.RepetitionCounter = 0;
	tim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim4);

	HAL_NVIC_EnableIRQ(TIM4_IRQn);//enable interrupts from timer 4
	HAL_TIM_Base_Start_IT(&tim4);

}
