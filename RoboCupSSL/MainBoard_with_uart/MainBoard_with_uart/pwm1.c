/*#include "stm32f4xx.h"
	#include "stm32f4xx_gpio.h"
	#include "stm32f4xx_rcc.h"
	#include "misc.h"
	#include "stm32f4xx_tim.h"

void pwm(long int a, uint32_t GPIO_Pin_x){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitDef;

	TIM_TimeBaseInitDef.TIM_Prescaler =200-1;
	TIM_TimeBaseInitDef.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitDef.TIM_Period = 40000-1;
	TIM_TimeBaseInitDef.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitDef.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitDef);
	TIM_Cmd(TIM4, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_x ;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	TIM_OCInitTypeDef TIM_OC;
	TIM_OC.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OC.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC.TIM_Pulse = a;
	TIM_OC.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC2Init(TIM4, &TIM_OC);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
}
int main(void)
{
	pwm(0, GPIO_Pin_13);
long int b;
	while(1)
	{
		for (b =0; b <20000-1; b+=1000)
		 {
							if (TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) != RESET)
							    {

								TIM4->CCR2=b;
									 TIM_ClearFlag(TIM4, TIM_IT_Update);
						}

				}
	}

}
*/
