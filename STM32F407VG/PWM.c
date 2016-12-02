/*
#include <stm32f4xx.h>				//	without this lib also the code will run coz this lib is included in below attached libraries
#include <stm32f4xx_gpio.h>			//	include the main file
#include <stm32f4xx_rcc.h>			// include the clock library
#include <stm32f4xx_tim.h>			// include the timer library

void GPIO_PWM_initialization()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);		//GPIOB Periph clock enable
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;					// initialize Alternative Functions
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
}

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
//in avr 8 bit mc operating at 16MHz, 16000000 ticks are counted and counter is updated by 1 and this happens till counter value is 255 after which it overflows
// in stm at 84MHz prescaled to 13125, 13125 ticks are counted and counter is incremented by 1 till counter reaches 500(period) after which it will overflow. 

void TIMER_enable()
	{
	// TIME enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 6400;
	TIM_TimeBaseInitStruct.TIM_Period = 500;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //c file 566 line
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
    TIM_Cmd(TIM4, ENABLE);
	}

		//    To get proper duty cycle, you have simple equation
		//    pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1
		//    where DutyCycle is in percent, between 0 and 100%
		//    Remember: if pulse_length is larger than TIM_Period, you will have output HIGH all the time

void PWM_enable()
	{
	TIM_OCInitTypeDef TIM_OCStruct;
	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1; // h file 234
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable; // WTF??
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC2Init(TIM4, &TIM_OCStruct);

}

int main(void)
{
	GPIO_PWM_initialization();
	TIMER_enable();
	PWM_enable();
	while(1)
	{
		TIM_SetCompare2(TIM4,150);
		Delay(100000);
		TIM_SetCompare2(TIM4,300);
		Delay(100000);
		TIM_SetCompare2(TIM4,500);
		Delay(100000);
	}
}
*/
