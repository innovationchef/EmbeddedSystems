
#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_tim.h>
#include <stm32f30x_exti.h>
#include <stm32f30x_syscfg.h>
#include <stm32f30x_misc.h>
void GPIO_LED_peripheral()
{
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
			GPIO_InitTypeDef  GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOE, &GPIO_InitStructure);
}
/*********************************************************************************************************************/
/*********************************** External interrupt and GPIO on motor 1******************************************/
/*********************************************************************************************************************/
// motor 1 to pin A0 and A1. We are reading rising edge from pin0 and incrementing count of motor 1.
void enable_input_for_interrupt()
{
			GPIO_SetBits(GPIOE, GPIO_Pin_13);
			GPIO_InitTypeDef  GPIO_InitStructure;
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void EXT_int_setting_motor_1()
{
			GPIO_SetBits(GPIOE, GPIO_Pin_12);
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource5);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
			EXTI_InitTypeDef EXTI_InitStruct;
		    EXTI_InitStruct.EXTI_Line = EXTI_Line5;
			EXTI_InitStruct.EXTI_LineCmd = ENABLE;
			EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
		    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
			EXTI_Init(&EXTI_InitStruct);
}
void EXT_int_enable_motor_1()
{
			GPIO_SetBits(GPIOE, GPIO_Pin_11);
			NVIC_InitTypeDef NvicStructure;
			NvicStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
			NvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NvicStructure.NVIC_IRQChannelSubPriority = 0;
			NvicStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NvicStructure);
}
volatile int flag=0;
void EXTI9_5_IRQHandler()
{
			EXTI_ClearITPendingBit(EXTI_Line5);
			if (EXTI_GetITStatus(EXTI_Line5) != RESET&&(flag==0))
	        {
				EXTI_ClearITPendingBit(EXTI_Line5);
				flag=1;
	           	GPIO_ResetBits(GPIOE,GPIO_Pin_8);
	         }
			if (EXTI_GetITStatus(EXTI_Line5) != RESET&&(flag==1))
	        {
				EXTI_ClearITPendingBit( EXTI_Line5);
                flag=0;
            	GPIO_SetBits(GPIOE,GPIO_Pin_8);
	        }
}
void main()
{
			GPIO_LED_peripheral();
			enable_input_for_interrupt();
			EXT_int_setting_motor_1();
			EXT_int_enable_motor_1();
			while(1){}
}
