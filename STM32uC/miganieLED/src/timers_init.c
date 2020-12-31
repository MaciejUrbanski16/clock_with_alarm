/*
 * timers_init.c
 *
 *  Created on: 31.12.2020
 *      Author: Admin
 */


#include "timers_init.h"
#include "stm32f1xx.h"



TIM_HandleTypeDef tim2; // trzeba co 1s przerwanie od timera
						//w tym przerwaniu ziekszymy wartoœc sekund o 1

TIM_HandleTypeDef tim3; //przerwanie od timera co 50ms w celu sprawdzenia stanu
						//przycisku konfiguracyjnego (eliminacja drgañ stykow)

TIM_HandleTypeDef tim4; //przerwanie od timera co 50ms w celu sprawdzenia stanu
						//przycisku konfiguracyjnego (eliminacja drgañ stykow)
void tim2Init(void){

	//konfiguracja timera TIM2 tak aby co 1s wywo³ywa³ przerwanie
	tim2.Instance = TIM2;
	tim2.Init.Period = 1000 - 1;
	tim2.Init.Prescaler = 8000 - 1;
	tim2.Init.ClockDivision = 0;
	tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2.Init.RepetitionCounter = 0;
	tim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim2);

	HAL_NVIC_EnableIRQ(TIM2_IRQn);//odblokowanie przerwan od timera TIM2
	HAL_TIM_Base_Start_IT(&tim2);

}
void tim3Init(void){

	//konfiguracja timera TIM3 tak aby co 200ms sprawdza³ stan przyciskow (eliminacja drgan stykow) i  w zale¿noœci
	//od trybu inkrementowa³ odpowiedni¹ wartoœc.
	tim3.Instance = TIM3;
	tim3.Init.Period = 200-1; //doliczy do 200ms po czym sprawdzi stan przyciskow
	tim3.Init.Prescaler = 8000-1;
	tim3.Init.ClockDivision = 0;
	tim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim3.Init.RepetitionCounter = 0;
	tim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim3);

	HAL_NVIC_EnableIRQ(TIM3_IRQn);//odblokowanie przerwan od timera TIM3
	HAL_TIM_Base_Start_IT(&tim3);


}
void tim4Init(void){

	//konfiguracja timera TIM4 tak aby co 40ms sprawdza³ stan przyciskow (eliminacja drgan stykow) i  w zale¿noœci
	//od trybu inkrementowa³ odpowiedni¹ wartoœc.
	tim4.Instance = TIM4;
	tim4.Init.Period = 40-1; //doliczy do 40ms po czym sprawdzi stan przyciskow
	tim4.Init.Prescaler = 8000-1;
	tim4.Init.ClockDivision = 0;
	tim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim4.Init.RepetitionCounter = 0;
	tim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim4);

	HAL_NVIC_EnableIRQ(TIM4_IRQn);//odblokowanie przerwan od timera TIM4
	HAL_TIM_Base_Start_IT(&tim4);

}
