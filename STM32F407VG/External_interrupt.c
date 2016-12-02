/*
#include <stm32f4xx.h>				//	without this lib also the code will run coz this lib is included in below attached libraries
#include <stm32f4xx_gpio.h>			//	include the gpio file
#include <stm32f4xx_rcc.h>			//	include the clock library
#include <stm32f4xx_exti.h>			//	include the external interrupt library
#include <misc.h>					//	include NVIC
#include <stm32f4xx_syscfg.h>		// for input source pin
void GPIO_initialization()
{
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
			GPIO_InitTypeDef  GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void INPUT_pins_enable()
{
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			GPIO_InitTypeDef  GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void EXTI_enable()
{
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); //Tell system that you will use PD0 for EXTI_Line0
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
			EXTI_InitTypeDef EXTI_InitStruct;
		    EXTI_InitStruct.EXTI_Line = EXTI_Line0;						//PD0 is connected to EXTI_Line0
			EXTI_InitStruct.EXTI_LineCmd = ENABLE;						//Enable interrupt
			EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;			//Interrupt mode
		    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//Triggers on rising and falling edge
			EXTI_Init(&EXTI_InitStruct);								//Add to EXTI

}
void NVIC_enable()
{
			NVIC_InitTypeDef NVIC_InitStruct;
			//Add IRQ vector to NVIC
			NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;			//PD0 is connected to EXTI_Line0, which has EXTI0_IRQn vector
			NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;	//Set priority
			NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;			//Set sub priority
			NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			//Enable interrupt
			NVIC_Init(&NVIC_InitStruct);	//Add to NVIC
}
// Set interrupt handlers

void EXTI0_IRQHandler(void) 											//Handle PD0 interrupt
{
			if (EXTI_GetITStatus(EXTI_Line0) != RESET) 					//Make sure that interrupt flag is set
			{
				GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
				EXTI_ClearITPendingBit(EXTI_Line0);						// Clear interrupt flag
			}
}

void main()
{
			GPIO_initialization();
			INPUT_pins_enable();
			EXTI_enable();
			NVIC_enable();
			while(1)
			{}
}

*/
