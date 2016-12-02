/*
#include <stm32f4xx_gpio.h>					//	include the main file
#include <stm32f4xx_rcc.h>					// include the clock library
#include <stm32f4xx_tim.h>					// include timer library
#include <misc.h>							// include interrupt library

void GPIO_initialization()
{
			//GPIOB Periph clock enable
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
			GPIO_InitTypeDef GPIO_InitStructure;
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
			TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
			TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
			TIM_Cmd(TIM4, ENABLE);
			TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}

void TIMER_interrupt_enable()
{
			NVIC_InitTypeDef NvicStructure;
			NvicStructure.NVIC_IRQChannel = TIM4_IRQn;
			NvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NvicStructure.NVIC_IRQChannelSubPriority = 1;
			NvicStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NvicStructure);
}


void TIM4_IRQHandler()
{
			if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
				{
					TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
					GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
				}
}

void main()
{

			GPIO_initialization();

			TIMER_enable();

			TIMER_interrupt_enable();

			while(1){
			}
}
*/
