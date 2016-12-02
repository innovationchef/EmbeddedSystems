
#include <stm32f30x.h>				//	without this lib also the code will run coz this lib is included in below attached libraries
#include <stm32f30x_gpio.h>			//	include the gpio file
#include <stm32f30x_rcc.h>			//include the clock library
#include <stm32f30x_tim.h>			// include timer library
#include <stm32f30x_exti.h>			//	include the external interrupt library
#include <stm32f30x_syscfg.h>		// for input source pin
#include <stm32f30x_misc.h>
int count_motor_1 = 0, count_motor_2 = 0;
void GPIO_initialization()
{
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
			GPIO_InitTypeDef  GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_Init(GPIOE, &GPIO_InitStructure);
}

// motor 1 to pin A0 and A1
void enable_input_for_interrupt()
{
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
			GPIO_InitTypeDef  GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void EXTI_setting_motor_1()
{
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
			EXTI_InitTypeDef EXTI_InitStruct;
		    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
			EXTI_InitStruct.EXTI_LineCmd = ENABLE;
			EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
		    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;	// Falling not working on stm
			EXTI_Init(&EXTI_InitStruct);

}
void NVIC_motor_1()
{
			NVIC_InitTypeDef NvicStructure;
			NvicStructure.NVIC_IRQChannel = EXTI0_IRQn;
			NvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NvicStructure.NVIC_IRQChannelSubPriority = 0;
			NvicStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NvicStructure);
}
void EXTI0_IRQHandler(void) 											//Handle PD0 interrupt
{
			if (EXTI_GetITStatus(EXTI_Line0) != RESET) 					//Make sure that interrupt flag is set
			{
				count_motor_1++;
				GPIO_SetBits(GPIOE, GPIO_Pin_8);
				EXTI_ClearITPendingBit(EXTI_Line0);						// Clear interrupt flag
			}
}

// motor 2 to pin A4 and A5
/*void EXTI_setting_motor_2()
{
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource4);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
			EXTI_InitTypeDef EXTI_InitStruct;
		    EXTI_InitStruct.EXTI_Line = EXTI_Line4;
			EXTI_InitStruct.EXTI_LineCmd = ENABLE;
			EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
		    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;	// Falling not working on stm
			EXTI_Init(&EXTI_InitStruct);
}
void NVIC_motor_2()
{
			NVIC_InitTypeDef NvicStructure;
			NvicStructure.NVIC_IRQChannel = EXTI4_IRQn;
			NvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NvicStructure.NVIC_IRQChannelSubPriority = 1;
			NvicStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NvicStructure);
}

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

}*/


void main()
{
			GPIO_initialization();
			enable_input_for_interrupt();
			EXTI_setting_motor_1();
			NVIC_motor_1();
			//EXTI_setting_motor_2();
			//NVIC_motor_2();
			while(1)
			{}
}
