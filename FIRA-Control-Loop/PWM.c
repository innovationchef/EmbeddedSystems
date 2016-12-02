#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_tim.h>
void pwm_to_motor()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_2); // TIM1_CH1 -> PE09
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_2); // TIM1_CH2 -> PE11
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_2); // TIM1_CH3 -> PE13
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_2); // TIM1_CH4 -> PE14
}
void initialize_pwm(int pwm_freq)
{
	int original_ticks = 84e6;
	int pwm_period = 255;
	int prescaler = original_ticks/(pwm_period * pwm_freq)-1;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = 1000-1;
	TIM_TimeBaseStructure.TIM_Period = pwm_period;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	//Set up 4 channels
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	TIM_OC2Init(TIM4,&TIM_OCInitStructure);
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);
	TIM_Cmd(TIM4,ENABLE);
	TIM_SetCompare1(TIM4,0);
	TIM_SetCompare3(TIM4,0);
	TIM_SetCompare2(TIM4,0);
	TIM_SetCompare4(TIM4,0);
}
// function to set duty cycle
void send_ticks(int channel, int ticks)
{
	switch (channel)
		{
		case 1: TIM_SetCompare1(TIM4,ticks); break;
		case 2: TIM_SetCompare2(TIM4,ticks); break;
		case 3: TIM_SetCompare3(TIM4,ticks); break;
		case 4: TIM_SetCompare4(TIM4,ticks); break;
		}
}
int main()
{
	int i=0;
	pwm_to_motor();
			initialize_pwm(200);
	while(1)
	{
		send_ticks(1, 0);
		send_ticks(2, 0);
	}
}

