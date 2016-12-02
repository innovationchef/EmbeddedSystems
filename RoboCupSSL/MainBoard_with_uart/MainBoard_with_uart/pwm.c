	#include "stm32f4xx.h"
	#include "stm32f4xx_gpio.h"
	#include "stm32f4xx_rcc.h"
	#include "misc.h"
	#include "stm32f4xx_tim.h"


	void Delay(__IO uint32_t nCount){
		 while(nCount--)
				  {
				  }
	}

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	uint16_t CCR1_Val = 0;

	uint16_t PrescalerValue = 0;


	void PWM(void)
	{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	PrescalerValue =1280;
	TIM_TimeBaseStructure.TIM_Period=65535;
	TIM_TimeBaseStructure.TIM_Prescaler=PrescalerValue;

	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0x00000;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	//PWM canal 1
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=32767;//CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4,ENABLE);

		}

	void GPIO(void)
	{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	//GPIOC Configuration: TIM3 CH1 (PC6)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	//Configuration
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);

	}

	long int a;
	int main()
		{   TIM_Cmd(TIM4,ENABLE);
			PWM();
			GPIO();


			GPIO_SetBits(GPIOD, GPIO_Pin_13);
			Delay(0xFFFFF);
			GPIO_ToggleBits(GPIOD,  GPIO_Pin_13);

		while(1)
		{


				for (a =0; a < 65535; a+=13107) {
					if (TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) != RESET)
					    {
					      TIM_ClearFlag(TIM2, TIM_IT_Update);

					TIM4->CCR1 = a;
				}

		}
		}
		}



