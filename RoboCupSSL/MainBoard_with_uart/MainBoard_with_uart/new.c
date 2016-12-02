/*#include "stm32f4xx.h"
	#include "stm32f4xx_gpio.h"
	#include "stm32f4xx_rcc.h"
	#include "misc.h"
	#include "stm32f4xx_tim.h"
	#define RC_MAX_VALUE 40
	#define RC_MIN_VALUE 20
	void GPIO(void);
	void PWM(void);
	void Delay(__IO uint32_t nCount);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	uint16_t CCR1_Val = 0;
	uint16_t PrescalerValue = 0;
	void PWM(void)
		{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
		PrescalerValue =(uint16_t) (SystemCoreClock  / 8000000)-1;//3
		// TIMER TEMEL AYARLAMALAR
		TIM_TimeBaseStructure.TIM_Period=400;
		TIM_TimeBaseStructure.TIM_Prescaler=839;//PrescalerValue;

		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_ClockDivision=0;
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
		//PWM canal 1
		TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse=CCR1_Val;//CCR1_Val;
		TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
		TIM_OC1Init(TIM4,&TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM4,ENABLE);
			TIM_Cmd(TIM4,ENABLE);
		}

			void GPIO(void)
			{
			GPIO_InitTypeDef GPIO_InitStructure;
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
			 GPIOC Configuration: TIM3 CH1 (PC6), TIM3 CH2 (PC7), TIM3 CH3 (PC8) and TIM3 CH4 (PC9)
			GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
			GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
			GPIO_Init(GPIOD,&GPIO_InitStructure);
			//Configuration
			GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
			}
			int axe1;

					int main()
					{
						GPIO();
						PWM();
						while(1)
								{
									for (axe1 = 20; axe1 < 40000; axe1=axe1+500) {

											Delay(0x003FFFF);
											TIM4->CCR1 = axe1;

										}
									}
								}


								void Delay(__IO uint32_t nCount)
								{
								  while(nCount--)
								  {
								  }
								}*/
