/*
#include <stm32f4xx.h>				//	without this lib also the code will run coz this lib is included in below attached libraries
#include <stm32f4xx_gpio.h>			// include the main file
#include <stm32f4xx_rcc.h>			// include the clock library
#include <stm32f4xx_tim.h>			// include timer library

void GPIO_initialization()
	{
			GPIO_InitTypeDef GPIO_InitStructure;
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	//GPIOB Periph clock enable
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
void TIMER_enable()
	{
			// TIME enable
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			TIM_TimeBaseInitStruct.TIM_Period = 500;
			TIM_TimeBaseInitStruct.TIM_Prescaler = 32000;
			TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //c file 566 line
			TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x0000;
			TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
			TIM_Cmd(TIM4, ENABLE);
	}

int main(void)
	{
		GPIO_initialization();
		TIMER_enable();
		//Store current timer value in variable

		while(1)
		{
			TIM_SetCounter(TIM4, 0);
			while(TIM_GetCounter(TIM4)<=499);
			GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
		}
	}
*/
