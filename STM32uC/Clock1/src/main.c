/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "stm32f1xx.h"


#include "lcd_init.h"
#include "timers_init.h"


//containers to storage data of time and alarm

char outHour[9];
char outDate[9];
char alarmTime[9];

//modes of clock's work

enum{
	RUNNING_MODE,
	SETTING_HOUR_MODE,
	SETTING_DATE_MODE,
	SETTING_ALARM_MODE
};


volatile uint8_t hour = 23;
volatile uint8_t min = 59;
volatile uint8_t sec = 57;
volatile uint8_t day = 31;
volatile uint8_t month = 7;
volatile uint8_t year = 20;

volatile uint8_t hourOfAlarm = 5;
volatile uint8_t minOfAlarm = 30;
volatile uint8_t secOfAlarm = 0;



 //current mode of clock's work
 volatile uint8_t mode = RUNNING_MODE;

 //variables that sign beeps after changing mode
volatile uint8_t beepAfterChanging = 0;
volatile uint8_t counterOfBeep = 0;


 //flag to check if alarm is set
 volatile uint8_t alarmIsSet= 0;

 //variables that are responsible for alarm trigger
 volatile uint16_t automaticSilenceOfAlarm = 0;
 volatile uint8_t alarmTriggered =0;
 volatile uint8_t alarmIsSetToShow=0;

 //flag that is responsible for triggering beeps
 volatile uint16_t beepsOfAlarm= 0;

 //flag that is responsible for state of beeps (first four fast beeps and then
 //silence during the same time as all four fast beeps
 volatile uint8_t directionOfBeeps = 0;



//variable to check how long button is pressed
volatile uint8_t pushes = 0;


//variable to configure inputs/outputs of microcontroller
GPIO_InitTypeDef gpio;



void buttonsInit(void){

	//configuration buttons to setting hour of alarm, actual hour and actual date
	gpio.Mode = GPIO_MODE_INPUT;//pins as inputs
	gpio.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_8|GPIO_PIN_9;
	//pins 5,6,8 will be responsible for setting:
	//time of alarm in SETTING_ALARM_MODE,
	//actual time in SETTING_HOUR_MODE,
	//actual date in SETTING_DATE_MODE,
	//pin number 9 will be responsible for switching between modes
	gpio.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &gpio);

	gpio.Pin = GPIO_PIN_10; //pin number 10 as output to buzzer
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &gpio);
}


//preparing values of alarm time to displaying depend on actual mode
void alarmTimeToDisplay(){

	//when alarm is inactive, there is no time of alarm
	if(alarmIsSet == 0 && mode == RUNNING_MODE){
		alarmTime[0] = '-';
		alarmTime[1] = '-';
		alarmTime[2] = ':';
		alarmTime[3] = '-';
		alarmTime[4] = '-';
		alarmTime[5] = ':';
		alarmTime[6] = '-';
		alarmTime[7] = '-';
		//alarmTime[8] = '-';
	}

	//when alarm is active, there is time of alarm
	else if(alarmIsSet == 1 && mode == RUNNING_MODE){

		//converting values of time to hh:mm::ss format
		uint8_t jednH = hourOfAlarm%10;
		uint8_t dzH = (hourOfAlarm -jednH)/10;

		uint8_t jednMin = minOfAlarm%10;
		uint8_t dzMin = (minOfAlarm -jednMin)/10;

		uint8_t jednSek = secOfAlarm%10;
		uint8_t dzSek = (secOfAlarm -jednSek)/10;

	    alarmTime[0] = (dzH%10) +48;
	    alarmTime[1] = (jednH%10) +48;
	    alarmTime[2] = ':';
	    alarmTime[3] =  (dzMin%10) + 48;
	    alarmTime[4] = (jednMin%10) +48;
	    alarmTime[5] = ':';
	    alarmTime[6] = (dzSek%10)+48;
	    alarmTime[7] = (jednSek%10) + 48;
	}
	else if(mode == SETTING_ALARM_MODE){
		uint8_t jednH = hourOfAlarm%10;
		uint8_t dzH = (hourOfAlarm -jednH)/10;

		uint8_t jednMin = minOfAlarm%10;
		uint8_t dzMin = (minOfAlarm -jednMin)/10;

		uint8_t jednSek = secOfAlarm%10;
		uint8_t dzSek = (secOfAlarm -jednSek)/10;

	    alarmTime[0] = (dzH%10) +48;
	    alarmTime[1] = (jednH%10) +48;
	    alarmTime[2] = ':';
	    alarmTime[3] =  (dzMin%10) + 48;
	    alarmTime[4] = (jednMin%10) +48;
	    alarmTime[5] = ':';
	    alarmTime[6] = (dzSek%10)+48;
	    alarmTime[7] = (jednSek%10) + 48;
	}

}



//check if values of hours and date are correct,
//change them when overflow will occur
void checkIfCorrectValuesOfDate(){
	if(sec>=60){
		sec = 0;
		min++;
	}
	if(min>=60){
		min = 0;
		hour++;
	}
	if(hour >= 24){
		hour = 0;
		day++;
	}
	if(month == 2){
		if(year%4 == 0 && day == 30){
			month++;
			day = 1;
		}
		else if(year%4 !=0 && day == 29){
			month++;
			day = 1;
		}
	}
	else if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10){
		if(day >= 32){
			month++;
			day= 1;
		}
	}
	else if(month == 4 || month == 6 || month == 9 || month == 11){
		if(day >= 31){
			month++;
			day = 1;
		}
	}
	else if(month >= 12){
		if(day >= 32){
			day = 1;
			month = 1;
			year++;
		}
	}
	if(year >= 100){
		year = 0;
	}

}
void checkIfCorrectValuesAfterSetting(){

	//check values of time and date after setting them by user
	//change them when overflow will occur

	if(sec >= 60)sec = 0;
	if(min >= 60)min = 0;
	if(hour >= 24)hour = 0;
	if(month >= 13) month = 0;
	if(month == 2 && year%4 == 0 && day == 30) day = 0;
	if(month == 2 && year%4 != 0 && day == 29) day = 0;
	if((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day == 32)
		day = 0;
	if((month == 4 || month == 6 || month == 9 || month == 11) && day == 31)
		day = 0;
	if(year >= 100) year = 0;
	if(secOfAlarm >=60) secOfAlarm = 0;
	if(minOfAlarm >= 60) minOfAlarm = 0;
	if(hourOfAlarm >=24) hourOfAlarm = 0;
}

void dateToDisplay()
{
	//preparing date to display in dd.mm.yy format
    uint8_t jednD = day%10;
    uint8_t dzD = (day-jednD)/10;

    uint8_t jednM = month %10;
    uint8_t dzM = (month - jednM)/10;

    uint8_t jednY = year %10;
    uint8_t dzY = (year - jednY)/10;

    outDate[0] = (dzD%10) +48;
    outDate[1] = (jednD%10) + 48;
    outDate[2] = '.';
    outDate[3] = (dzM%10)+48;
    outDate[4] = (jednM%10) + 48;
    outDate[5] = '.';
    outDate[6] = (dzY%10)+48;
    outDate[7] = (jednY%10) +48;
}

void hourToDisplay(){

	//preparing actual time to display in hh:mm:ss format
	uint8_t jednH = hour%10;
	uint8_t dzH = (hour -jednH)/10;

	uint8_t jednMin = min%10;
	uint8_t dzMin = (min -jednMin)/10;

	uint8_t jednSek = sec%10;
	uint8_t dzSek = (sec -jednSek)/10;

    outHour[0] = (dzH%10) +48;
    outHour[1] = (jednH%10) +48;
    outHour[2] = ':';
    outHour[3] =  (dzMin%10) + 48;
    outHour[4] = (jednMin%10) +48;
    outHour[5] = ':';
    outHour[6] = (dzSek%10)+48;
    outHour[7] = (jednSek%10) + 48;
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
}

//interrupt service routine from timer 2
void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim2);
}

//interrupt service routine from timer 3
void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim3);
}

//interrupt service routine from timer 4
void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim4);
}

////interrupt service routine call when timer2, timer 3 or timer 4 will report interrupt
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){



	if(htim->Instance == TIM2){

		//every second is interrupt from timer 2 and actual time is refreshed and displayed

		sec++;

		//check if now is time to trigger the alarm
		if(mode == RUNNING_MODE){

			if((alarmIsSet==1 )){
				if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5) ==GPIO_PIN_RESET){
					alarmIsSet = 0;
				}
				else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5) ==GPIO_PIN_SET){
					alarmIsSet =  1;
				}

			}
			else if(alarmIsSet == 0) {

				if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5) ==GPIO_PIN_RESET){
				alarmIsSet = 1;
				}
				else if( HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5) ==GPIO_PIN_SET){
					alarmIsSet = 0;
				}
			}

			checkIfCorrectValuesOfDate();

			hourToDisplay();
			dateToDisplay();
			alarmTimeToDisplay();
			lcd_clear();

			lcd_draw_text(1,1,"ACTUAL TIME");
			lcd_draw_text(2,1,outHour);
			lcd_draw_text(3,1,outDate);
			lcd_draw_text(4,1,alarmTime);

			lcd_copy();

		}

		//if now is time to trigger alarm - trigger alarm
		if(alarmIsSet == 1 &&hour == hourOfAlarm &&min == minOfAlarm && sec == secOfAlarm){

			alarmTriggered = 1;
		}

		if(alarmTriggered == 1 && HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5) ==GPIO_PIN_RESET ){

			alarmTriggered = 0;
		}



	}


	else if(htim->Instance == TIM3){


		 if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5) == GPIO_PIN_RESET){
			 //depending on mode and pressed button proper value is changed
			 //and refreshed actual state of lcd display
			switch(mode){
			case SETTING_ALARM_MODE:
				hourOfAlarm++;
				checkIfCorrectValuesAfterSetting();
				hourToDisplay();
				dateToDisplay();
				alarmTimeToDisplay();
				lcd_clear();
				lcd_draw_text(1,1,"SET THE ALARM");
				lcd_draw_text(3,1,alarmTime);
				lcd_copy();
				break;
			case SETTING_HOUR_MODE:
				hour++;
				checkIfCorrectValuesAfterSetting();
				hourToDisplay();
				lcd_clear();
				lcd_draw_text(1,1,"SET THE TIME");
				lcd_draw_text(3,1,outHour);
				lcd_copy();
				break;
			case SETTING_DATE_MODE:
				day++;
				checkIfCorrectValuesAfterSetting();
				dateToDisplay();
				lcd_clear();
				lcd_draw_text(1,1,"SET THE DATE");
				lcd_draw_text(3,1,outDate);
				lcd_copy();
				break;
			default:
				break;
			}
		}
		else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6) == GPIO_PIN_RESET){

			switch(mode){
			case SETTING_ALARM_MODE:
				minOfAlarm++;
				checkIfCorrectValuesAfterSetting();
				hourToDisplay();
				dateToDisplay();
				alarmTimeToDisplay();
				lcd_clear();
				lcd_draw_text(1,1,"SET THE ALARM");
				lcd_draw_text(3,1,alarmTime);
				lcd_copy();
				break;
			case SETTING_HOUR_MODE:
				min++;
				checkIfCorrectValuesAfterSetting();
				hourToDisplay();
				lcd_clear();
				lcd_draw_text(1,1,"SET THE TIME");
				lcd_draw_text(3,1,outHour);
				lcd_copy();
				break;
			case SETTING_DATE_MODE:
				month++;
				checkIfCorrectValuesAfterSetting();
				dateToDisplay();
				lcd_clear();
				lcd_draw_text(1,1,"SET THE DATE");
				lcd_draw_text(3,1,outDate);
				lcd_copy();
				break;
			default:
				break;
			}
		}

		else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8) == GPIO_PIN_RESET){

			switch(mode){
			case SETTING_ALARM_MODE:
				secOfAlarm++;
				checkIfCorrectValuesAfterSetting();
				hourToDisplay();
				dateToDisplay();
				alarmTimeToDisplay();
				lcd_clear();
				lcd_draw_text(1,1,"SET THE ALARM");
				lcd_draw_text(3,1,alarmTime);
				lcd_copy();
				break;
			case SETTING_HOUR_MODE:
				sec++;
				checkIfCorrectValuesAfterSetting();
				hourToDisplay();
				dateToDisplay();
				lcd_clear();
				lcd_draw_text(1,1,"SET THE TIME");
				lcd_draw_text(3,1,outHour);
				lcd_copy();
				break;
			case SETTING_DATE_MODE:
				year++;
				checkIfCorrectValuesAfterSetting();
				hourToDisplay();
				dateToDisplay();
				lcd_clear();
				lcd_draw_text(1,1,"SET THE DATE");
				lcd_draw_text(3,1,outDate);
				lcd_copy();
				break;
			default:
				break;
			}
		}


	}
	else if(htim->Instance == TIM4){
	    //interrupt from timer 4 checks every 40ms if configuration button is pressed to eliminate bounce
		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9) == GPIO_PIN_RESET){

			pushes++;
			if(pushes == 20){
				//if button is hold by 20*40ms = 800ms -> change mode to next mode


				switch(mode){
				case RUNNING_MODE:
					mode = SETTING_ALARM_MODE;
					hourToDisplay();
					dateToDisplay();
					alarmTimeToDisplay();
					lcd_clear();
					lcd_draw_text(1,1,"SET THE ALARM");
					lcd_draw_text(3,1,alarmTime);
					lcd_copy();
					break;
				case SETTING_ALARM_MODE:
					mode = SETTING_HOUR_MODE;
					hourToDisplay();
					dateToDisplay();
					lcd_clear();
					lcd_draw_text(1,1,"SET THE TIME");
					lcd_draw_text(3,1,outHour);
					lcd_copy();
					break;
				case SETTING_HOUR_MODE:
					mode = SETTING_DATE_MODE;
					hourToDisplay();
					dateToDisplay();
					lcd_clear();
					lcd_draw_text(1,1,"SET THE DATE");
					lcd_draw_text(3,1,outDate);
					lcd_copy();
					break;
				case SETTING_DATE_MODE:
					mode = RUNNING_MODE;
					hourToDisplay();
					dateToDisplay();
					alarmTimeToDisplay();
					lcd_clear();

					lcd_draw_text(1,1,"ACTUAL TIME");
					lcd_draw_text(2,1,outHour);
					lcd_draw_text(3,1,outDate);
					lcd_draw_text(4,1,alarmTime);

					lcd_copy();
					break;
				default:
					break;
				}


				pushes = 0;
				beepAfterChanging = 1;

			}

			if((pushes<20) && HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9) == GPIO_PIN_SET){
				//if holding this button is shorter than 800ms -> return to ACTUAL_TIME_MODE
				mode = RUNNING_MODE;
				pushes = 0;
			}

		}
		//triggering beep after changing mode
		//checking if the alarm is not triggered now
		if(beepAfterChanging ==1 && counterOfBeep <=10 &&alarmTriggered == 0){
			//as soon as user change mode short beep will occur 10*20ms = 200ms
			counterOfBeep++;
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
		}
		if(counterOfBeep == 10 && beepAfterChanging == 1&& alarmTriggered == 0){
			beepAfterChanging = 0;
			counterOfBeep = 0;
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
		}


		//triggering alarm as four fast beeps and pause which during the same time as the four fast beeps
		//until user will not silent it or two minutes will elapse without reaction of user

		if(alarmTriggered == 1 && directionOfBeeps == 0 &&beepAfterChanging ==0 ){
			if(beepsOfAlarm %2 == 0){
				//beeps should be as long as two period interrupts from timer 4

				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_10);
			}
			beepsOfAlarm++;
			if(beepsOfAlarm == 16){
				directionOfBeeps = 1;//cisza
				//0->pikanie
			}
		}
		//
		//proper flag of "direction"(whether is pipipipi or ------->pause) for alarm's beeps
		if(directionOfBeeps == 1 && alarmTriggered == 1 &&beepAfterChanging == 0){
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
			beepsOfAlarm--;
			if(beepsOfAlarm == 0){
				directionOfBeeps = 0; //->pikanie
			}
		}
		if(alarmTriggered == 0 &&beepAfterChanging ==0){
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
			directionOfBeeps = 0;
		}
	}

}

int main(void) {


	SystemCoreClock = 8000000; // clock 8Mhz

	HAL_Init();

	//initialization of clocks for modules
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE();


	//initialization pins and interface SPI for lcd display
	lcdInit();

	lcd_setup();

	//activation of timer 2 with interruption every 1s
	tim2Init();

	//activation of timer 3 with interruption every 20ms
	tim3Init();

	//activation of timer 4 with interruption every 40ms
	tim4Init();

	//activation of pins for buttons
	buttonsInit();

	//turning off the buzzer
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);

	while (1)
	{
		//main loop of program is empty
		//every action is based on internal microcontroller's timers
		//microcontroller does not have to check any if statement every loop iteration
	}
}


