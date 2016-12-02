
#include <stm32f30x.h>				//	without this lib also the code will run coz this lib is included in below attached libraries
#include <stm32f30x_gpio.h>			//	include the gpio file
#include <stm32f30x_rcc.h>			//include the clock library
#include <stm32f30x_tim.h>			// include timer library
#include <stm32f30x_misc.h>

void GPIO_initialization()
{
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_8;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void TIMER_enable()
{
			// TIME enable
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			TIM_TimeBaseInitStruct.TIM_Period = 32;
			TIM_TimeBaseInitStruct.TIM_Prescaler = 8400;
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
			NvicStructure.NVIC_IRQChannelSubPriority = 0;
			NvicStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NvicStructure);
}
volatile int flag=0;
volatile int flag_interrupt=0;
void TIM4_IRQHandler()
{
			if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET&&(flag==0))
				{
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
				 flag=1;
				   	GPIO_ResetBits(GPIOE,GPIO_Pin_8);
				}
		   if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET&&(flag==1))
				{

			   TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
				                flag=0;
				            	GPIO_SetBits(GPIOE,GPIO_Pin_8);
				}

}

void main()
{
			GPIO_initialization();
			TIMER_enable();
			TIMER_interrupt_enable();
			GPIO_SetBits(GPIOE,GPIO_Pin_10);
			while(1){
			}
}
