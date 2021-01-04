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
//#include "alarm.h"

//conteners to storage data of time and alarm

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


//aktualny tryb pracy zegara
/*extern volatile uint8_t mode;// = RUNNING_MODE;

//zmienne do sygnalizacji dzwiekowej tuz po zmianie trybu
extern volatile uint8_t beepAfterChanging;// = 0;
extern volatile uint8_t counterOfBeep;// = 0;


extern volatile uint8_t hourOfAlarm;// = 5;
extern volatile uint8_t minOfAlarm;// = 30;
extern volatile uint8_t secOfAlarm;// = 0;



//flaga sprawdzajaca czy alarm jest aktualnie ustawiony
extern volatile uint8_t alarmIsSet;// = 0;
extern volatile uint16_t automaticSilenceOfAlarm;// = 0;
extern volatile uint8_t alarmTriggered;// =0;
extern volatile uint8_t alarmIsSetToShow;// =0;

//flagi do wyzwolenia pikania w alarmie
extern volatile uint16_t beepsOfAlarm;// = 0;
extern volatile uint8_t directionOfBeeps;// = 0;*/

volatile uint8_t hour = 23;
volatile uint8_t min = 59;
volatile uint8_t sec = 57;
volatile uint8_t day = 31;
volatile uint8_t month = 7;
volatile uint8_t year = 20;

volatile uint8_t hourOfAlarm = 5;
volatile uint8_t minOfAlarm = 30;
volatile uint8_t secOfAlarm = 0;



 //aktualny tryb pracy zegara
 volatile uint8_t mode = RUNNING_MODE;

 //zmienne do sygnalizacji dzwiekowej tuz po zmianie trybu
volatile uint8_t beepAfterChanging = 0;
volatile uint8_t counterOfBeep = 0;


 //flaga sprawdzajaca czy alarm jest aktualnie ustawiony
 volatile uint8_t alarmIsSet= 0;
 volatile uint16_t automaticSilenceOfAlarm = 0;
 volatile uint8_t alarmTriggered =0;
 volatile uint8_t alarmIsSetToShow=0;

 //flagi do wyzwolenia pikania w alarmie
 volatile uint16_t beepsOfAlarm= 0;
 volatile uint8_t directionOfBeeps = 0;


//funkcja do obslugi przerwnia zewnetrznego od klawiszy pinow 2,3,4,5


volatile uint8_t pushes = 0;//zmienna sprawzaj¹ca ile czasu trwa juz wcisniecie przycisku



GPIO_InitTypeDef gpio;

void buttonsInit(void){
	//konfiguracja przyciskow do ustawiania godziny i daty i alarmu (umiejscowione na porcie B
	gpio.Mode = GPIO_MODE_INPUT;//piny jako wejœcia
	gpio.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_8|GPIO_PIN_9; //piny 5,6,8 pos³u¿a do ustawiania czasu, (pin 9 do zmiany stanu
	gpio.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &gpio);

	gpio.Pin = GPIO_PIN_10; //konfiguracja pinu jako wyjscia dla buzzera
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &gpio);
}


void alarmTimeToDisplay(){
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
	else if(alarmIsSet == 1 && mode == RUNNING_MODE){
		//zamiana pszczegolnych wartoœci godziny w ci¹g znakow w formacie hh:mm:ss
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



//sprawdza poprawnosc aktualnej godziny i daty oraz modyfikuje odpowiednie wartoœci adekwatnie do obecnego czaasu
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

	//sprawdzenie cxzy w czasie ustawiania godziny i daty przez uzytkownika
	//nie zosta³y przekroczone zakresy poszczególnych danych

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
	//zamiana poszczególnych wartoœci daty na ci¹g znaków w formacie dd.mm.rr
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

	//zamiana pszczegolnych wartoœci godziny w ci¹g znakow w formacie hh:mm:ss
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

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim2);
}

void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim3);
}

void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim4);
}

//procedura obs³ugi przerwañ od timerów
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){


	//push = 0;

	if(htim->Instance == TIM2){
		//tutaj bedzie implementacja funkcji wywo³ywanej co przerwanie timera TIM2 czyli co 1s zwieksza liczbe sekund o 1

		sec++;

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


		if(alarmIsSet == 1 &&hour == hourOfAlarm &&min == minOfAlarm && sec == secOfAlarm){

			//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
			alarmTriggered = 1;
		}

		if(alarmTriggered == 1 && HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5) ==GPIO_PIN_RESET ){

			alarmTriggered = 0;
			//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
		}



	}

	//refactor po testach!!!
	else if(htim->Instance == TIM3){


		 if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5) == GPIO_PIN_RESET){

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
		//sprawdzenie wcisniêcia konkretnego przycisku co 200ms w trakcie przerwania od TIM4
		//i nastawa odpowiednich wartoœci przy odpowiednim naciœniêtym przycisku
		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9) == GPIO_PIN_RESET){
			//prze³aczanie trybów za pomoca 4. przycisku PC8
			pushes++;
			if(pushes == 20){
				//przytrzymanie przycisku
				//jesli wcisniecie przycisku 4. trwa juz20*40ms= 800ms to zmien odpowiednio tryb

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
				//krotkie wcisniecie powoduje natychmiastowy powrot do trybu wyswietlania godziny
				mode = RUNNING_MODE;
				pushes = 0;
			}

		}
		//wyzwolenie dzwieku oznaczaj¹cego zmiane trybu
		//sprawdzenie aby nie bylo kolizji z sygnalem generowanym w momencie alarmu
		if(beepAfterChanging ==1 && counterOfBeep <=10 &&alarmTriggered == 0){
			//tuz po zmianie trybu sygnalizacja dzwiekowa zminionego trybu na 10*20ms = 200ms
			counterOfBeep++;
			//jakby coc to tu zakomentowac
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
		}
		if(counterOfBeep == 10 && beepAfterChanging == 1&& alarmTriggered == 0){
			beepAfterChanging = 0;
			counterOfBeep = 0;
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
		}


		//wyzwolenie alarmu jako pikanie
		//sparawdzanie aby nie bylo kolizji z sygnalem wyzwalanym w momencie zmiany trybu
		if(alarmTriggered == 1 && directionOfBeeps == 0 &&beepAfterChanging ==0 ){
			if(beepsOfAlarm %2 == 0){//zeby zbyt szybko nie pika³ to pikniecie ustawione na co drugie wystapienie przerwania od timera
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_10);
			}
			beepsOfAlarm++;
			if(beepsOfAlarm == 16){
				directionOfBeeps = 1;//cisza
				//0->pikanie
			}
		}
		//
		//odpowiednia flaga kierunku(czy wystepuja pipipipi czy jest ------->pauza)dla sygna³u z alarmu
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


	SystemCoreClock = 8000000; // taktowanie 8Mhz

	HAL_Init();

	//inicjalizacja zegarów dla poszczególnych modu³ów
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE();


	//inicjalizacja pinów i interface'u SPI dla wyswietlacza lcd
	lcdInit();

	lcd_setup();

	//w³aczenie timera 2 z przerwaniem co 1s
	tim2Init();

	//wlaczenie timera 3 z przerwaniem co 200ms
	tim3Init();

	//wlaczenie timera 4 z przerwaniem co 40ms
	tim4Init();


	buttonsInit();

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);//buzzer wy³aczony

	while (1)
	{
		//
	}
}


