#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_syscfg.h>
#include <misc.h>
#define motor_speed 100
volatile int hall_1_flag = 0, hall_2_flag = 0, hall_3_flag = 0;
volatile int fault = 1, otw = 1;

void pwm_port_b()
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_TIM2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_TIM2);
		/* Time base configuration */
		TIM_TimeBaseStructure.TIM_Period = 100;
		TIM_TimeBaseStructure.TIM_Prescaler = 41;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;

		TIM_OC3Init(TIM2, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
		TIM_OC4Init(TIM2, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM2, ENABLE);
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE);
}
void pwm_port_d()
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
		TIM_TimeBaseStructure.TIM_Period = 100;
		TIM_TimeBaseStructure.TIM_Prescaler = 41;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;

		TIM_OC1Init(TIM4, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* PWM1 Mode configuration: Channel2 */
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* PWM1 Mode configuration: Channel3 */
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* PWM1 Mode configuration: Channel4 */
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* PWM1 Mode configuration: Channel5 */
		TIM_ARRPreloadConfig(TIM4, ENABLE);
		/* TIM4 enable counter */
		TIM_Cmd(TIM4, ENABLE);
}

void PWM1(int duty_cycle)
{
	TIM4->CCR1 = duty_cycle;
}
void PWM2(int duty_cycle)
{
	TIM4->CCR2 = duty_cycle;
}
void PWM3(int duty_cycle)
{
	TIM4->CCR3 = duty_cycle;
}
void RESET1(int duty_cycle)
{
	TIM4->CCR4 = duty_cycle;
}
void RESET2(int duty_cycle)
{
	TIM2->CCR3 = duty_cycle;
}
void RESET3(int duty_cycle)
{
	TIM2->CCR4 = duty_cycle;
}
void motorInterrupt_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource5);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource6);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource7);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource8);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource9);

	GPIO_InitTypeDef GPIO_PC5;
	GPIO_PC5.GPIO_Mode = GPIO_Mode_IN;
	GPIO_PC5.GPIO_Pin = GPIO_Pin_5;
	GPIO_PC5.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_PC5.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_PC5.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC,&GPIO_PC5);

	GPIO_InitTypeDef GPIO_PC6;
	GPIO_PC6.GPIO_Mode = GPIO_Mode_IN;
	GPIO_PC6.GPIO_Pin = GPIO_Pin_6;
	GPIO_PC6.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_PC6.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_PC6.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC,&GPIO_PC6);

	GPIO_InitTypeDef GPIO_PC7;
	GPIO_PC7.GPIO_Mode = GPIO_Mode_IN;
	GPIO_PC7.GPIO_Pin = GPIO_Pin_7;
	GPIO_PC7.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_PC7.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_PC7.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC,&GPIO_PC7);

	GPIO_InitTypeDef GPIO_PC8;
	GPIO_PC8.GPIO_Mode = GPIO_Mode_IN;
	GPIO_PC8.GPIO_Pin = GPIO_Pin_8;
	GPIO_PC8.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_PC8.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_PC8.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC,&GPIO_PC8);

	GPIO_InitTypeDef GPIO_PC9;
	GPIO_PC9.GPIO_Mode = GPIO_Mode_IN;
	GPIO_PC9.GPIO_Pin = GPIO_Pin_8;
	GPIO_PC9.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_PC9.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_PC9.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC,&GPIO_PC9);

	EXTI_InitTypeDef EXTI_PC5;
	EXTI_PC5.EXTI_Line = EXTI_Line5;
	EXTI_PC5.EXTI_LineCmd = ENABLE;
	EXTI_PC5.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_PC5.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_PC5);

	EXTI_InitTypeDef EXTI_PC6;
	EXTI_PC6.EXTI_Line = EXTI_Line6;
	EXTI_PC6.EXTI_LineCmd = ENABLE;
	EXTI_PC6.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_PC6.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_PC6);

	EXTI_InitTypeDef EXTI_PC7;
	EXTI_PC7.EXTI_Line = EXTI_Line7;
	EXTI_PC7.EXTI_LineCmd = ENABLE;
	EXTI_PC7.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_PC7.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_PC7);

	EXTI_InitTypeDef EXTI_PC8;
	EXTI_PC8.EXTI_Line = EXTI_Line8;
	EXTI_PC8.EXTI_LineCmd = ENABLE;
	EXTI_PC8.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_PC8.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_PC8);

	EXTI_InitTypeDef EXTI_PC9;
	EXTI_PC9.EXTI_Line = EXTI_Line9;
	EXTI_PC9.EXTI_LineCmd = ENABLE;
	EXTI_PC9.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_PC9.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_PC9);

	NVIC_InitTypeDef NVIC_PC;
	NVIC_PC.NVIC_IRQChannel =  EXTI9_5_IRQn;
	NVIC_PC.NVIC_IRQChannelCmd = ENABLE;
	NVIC_PC.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_PC.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_PC);
}
int main(void)
{
	pwm_port_d();
	pwm_port_b();
	motorInterrupt_init();
	while(1)
	{
		if(hall_1_flag == 1 && hall_2_flag == 0 && hall_3_flag == 1)
		{
			PWM1(motor_speed);
			PWM2(0);
			PWM3(0);
			RESET1(motor_speed);
			RESET2(motor_speed);
			RESET3(0);
		}
		if(hall_1_flag == 1 && hall_2_flag == 0 && hall_3_flag == 0)
		{
			PWM1(motor_speed);
			PWM2(0);
			PWM3(0);
			RESET1(motor_speed);
			RESET2(0);
			RESET3(motor_speed);
		}
		if(hall_1_flag == 1 && hall_2_flag == 1 && hall_3_flag == 0)
		{
			PWM1(0);
			PWM2(motor_speed);
			PWM3(0);
			RESET1(0);
			RESET2(motor_speed);
			RESET3(motor_speed);
		}
		if(hall_1_flag == 0 && hall_2_flag == 1 && hall_3_flag == 0)
		{
			PWM1(0);
			PWM2(motor_speed);
			PWM3(0);
			RESET1(motor_speed);
			RESET2(motor_speed);
			RESET3(0);
		}
		if(hall_1_flag == 0 && hall_2_flag == 1 && hall_3_flag == 1)
		{
			PWM1(0);
			PWM2(0);
			PWM3(motor_speed);
			RESET1(motor_speed);
			RESET2(0);
			RESET3(motor_speed);
		}
		if(hall_1_flag == 0 && hall_2_flag == 0 && hall_3_flag == 1)
		{
			PWM1(0);
			PWM2(0);
			PWM3(motor_speed);
			RESET1(0);
			RESET2(motor_speed);
			RESET3(motor_speed);
		}
		if(fault == 0 && otw == 0) // overtemperature warning and occured
		{}
		if(fault == 0 && otw == 1) // overcurrent
		{}
		if(fault == 1 && otw == 0) // overtemperature warning
		{}
	}
}
void EXTI9_5_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==SET)
		{ hall_1_flag = 1; }
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==RESET)
		{ hall_1_flag = 0; }
	}
    EXTI_ClearITPendingBit(EXTI_Line5);
    NVIC_ClearPendingIRQ(EXTI9_5_IRQn);

	if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)==SET)
		{ hall_2_flag = 1; }
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)==RESET)
		{ hall_2_flag = 0; }
	}
    EXTI_ClearITPendingBit(EXTI_Line6);
    NVIC_ClearPendingIRQ(EXTI9_5_IRQn);

	if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)==SET)
		{ hall_3_flag = 1; }
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)==RESET)
		{ hall_3_flag = 0; }
	}
    EXTI_ClearITPendingBit(EXTI_Line7);
    NVIC_ClearPendingIRQ(EXTI9_5_IRQn);

    if(EXTI_GetITStatus(EXTI_Line8)!=RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)==SET)
		{ fault = 1; }
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)==RESET)
		{ fault = 0; }
	}
	EXTI_ClearITPendingBit(EXTI_Line8);
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn);

	if(EXTI_GetITStatus(EXTI_Line9)!=RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)==SET)
		{ otw = 1; }
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)==RESET)
		{ otw = 0; }
	}
	EXTI_ClearITPendingBit(EXTI_Line9);
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
}
