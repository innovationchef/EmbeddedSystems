#include "stm32f4xx.h"
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_adc.h"
#include <stm32f4xx_tim.h>
#include <stdio.h>
#include "semihosting.h"
#include "stm32f4xx_conf.h"

void GPIO_A()
	{
			GPIO_InitTypeDef  GPIO_InitStructure;
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_1| GPIO_Pin_0;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
void GPIO_B()
	{
			GPIO_InitTypeDef  GPIO_InitStructure;
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_1| GPIO_Pin_0;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
void GPIO_C()
	{
			GPIO_InitTypeDef  GPIO_InitStructure;
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8 | GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOC, &GPIO_InitStructure);
	}
void GPIO_D()
	{
			GPIO_InitTypeDef  GPIO_InitStructure;
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3 | GPIO_Pin_7 | GPIO_Pin_8;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
void GPIO_E()
	{
			GPIO_InitTypeDef  GPIO_InitStructure;
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOE, &GPIO_InitStructure);
	}
void set_pwm()
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
		/* Time base configuration */
		TIM_TimeBaseStructure.TIM_Period = 2047;
		TIM_TimeBaseStructure.TIM_Prescaler = 9;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* PWM1 Mode configuration: Channel2 */
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* PWM1 Mode configuration: Channel3 */
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* PWM1 Mode configuration: Channel4 */
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM4, ENABLE);
		/* TIM4 enable counter */
		TIM_Cmd(TIM4, ENABLE);
}

void give_pwm(int channel, int duty_cycle)
{
	switch (channel){
		case 1: TIM4->CCR1 = duty_cycle; break;
		case 2: TIM4->CCR2 = duty_cycle; break;
		case 3: TIM4->CCR3 = duty_cycle; break;
		case 4: TIM4->CCR4 = duty_cycle; break;
	}
}

int main(void) {
	char str[15];
	SystemInit();
	TM_DELAY_Init();
	TM_ADC_Init(ADC1, ADC_Channel_0);
	TM_ADC_Init(ADC1, ADC_Channel_3);
	GPIO_B();
	GPIO_C();
	GPIO_D();
	GPIO_E();
	set_pwm();

	uint16_t x;
	while (1) {
		give_pwm(1,614);
//		x = TM_ADC_Read(ADC1, ADC_Channel_0);
//		if(x>0)
//		{
//			give_pwm(2,614);
//		}
//		Delayms(100);
		printf("%d", 56);
			SH_SendChar('\n');

	}
}
