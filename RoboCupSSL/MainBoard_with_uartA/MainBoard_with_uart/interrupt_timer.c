#include "stm32f4xx.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"

void TIM4_IRQHandler()
{
	if(TIM_GetITStatus(TIM4, TIM_FLAG_Update)!=RESET)
	{

        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
	}
}

void TIM_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitDef;

	TIM_TimeBaseInitDef.TIM_Prescaler =400-1;
	TIM_TimeBaseInitDef.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitDef.TIM_Period = 40000-1;
	TIM_TimeBaseInitDef.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitDef.TIM_RepetitionCounter = 0;
	//TIM_ITConfig(TIM4,  TIM_IT_CC1, ENABLE);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitDef);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

void GPIO()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	//GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 ;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void INTERRUPT_ENABLE(){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

int main()
{
	GPIO();
	TIM_Init();
	INTERRUPT_ENABLE();

	while(1)
	{

	}
}

