
#include <stm32f30x.h>
#include <stm32f30x_tim.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_misc.h>
#include <stm32f30x_EXTI.h>
#include <stm32f30x_syscfg.h>
#include <stm32f30x_usart.h>
volatile int Data;
volatile int flag;
void GPIO_USART_peripheral()
{
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
			GPIO_InitTypeDef  GPIO_InitStructure;
			//Set USART2 Tx (PA.02) as AF push-pull
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			//Set USART2 Rx (PA.03) as input floating
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_7);
}
void USART_enable()
{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
			USART_InitTypeDef USART_InitStructure;
			USART_InitStructure.USART_BaudRate = 115200;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			USART_InitStructure.USART_StopBits = USART_StopBits_1; //works for all values of stop bits
			USART_InitStructure.USART_Parity = USART_Parity_No;	// works for all parity
			USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // works forf all values
			//Write USART2 parameters
			USART_Init(USART2, &USART_InitStructure);
			USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
			//USART_ITConfig(USART2,USART_IT_TXE,ENABLE);
			USART_Cmd(USART2, ENABLE);
}
void USART_interrupt_enable()
{
			NVIC_InitTypeDef NVIC_InitStruct;
			NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
			NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
			NVIC_Init(&NVIC_InitStruct);
}
void USART2_IRQHandler(void)
{
	    Data = USART_ReceiveData(USART2);
	    flag=1;
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}


volatile int flag_1=0, flag_2=0;
void GPIO_LED_peripheral()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE,ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure2;
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure2.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure2);
}
void enable_input_for_interrupt_1()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD,ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure_EXTI1;
	GPIO_InitStructure_EXTI1.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure_EXTI1.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure_EXTI1.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure_EXTI1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure_EXTI1.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOD, &GPIO_InitStructure_EXTI1);
}
void EXT_int_setting_motor_1()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource5);
	EXTI_InitTypeDef  EXTI_InitStructure_EXTI1;
	EXTI_InitStructure_EXTI1.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure_EXTI1.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure_EXTI1.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure_EXTI1.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure_EXTI1);
}
void EXT_int_enable_motor_1()
{
	NVIC_InitTypeDef NVIC_InitStructure_EXTI1;
	NVIC_InitStructure_EXTI1.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure_EXTI1.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStructure_EXTI1.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure_EXTI1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure_EXTI1);
}
void EXTI9_5_IRQHandler()
{

	   if (EXTI_GetITStatus(EXTI_Line5) != RESET&&(flag_1==0))
	         {
	    		EXTI_ClearITPendingBit(EXTI_Line5);
	               flag_1=1;
	           	GPIO_ResetBits(GPIOE,GPIO_Pin_8);
	           }
	    if (EXTI_GetITStatus(EXTI_Line5) != RESET&&(flag_1==1))
	           {
	    		EXTI_ClearITPendingBit( EXTI_Line5);
	                flag_1=0;
	            	GPIO_SetBits(GPIOE,GPIO_Pin_8);
	           }
	    EXTI_ClearITPendingBit(EXTI_Line5);
}
void enable_input_for_interrupt_2()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure_EXTI2;
	GPIO_InitStructure_EXTI2.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure_EXTI2.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure_EXTI2.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure_EXTI2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure_EXTI2.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure_EXTI2);
}
void EXT_int_setting_motor_2()
{
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource10);
	EXTI_InitTypeDef  EXTI_InitStructure_EXTI2;
	EXTI_InitStructure_EXTI2.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure_EXTI2.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure_EXTI2.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure_EXTI2.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure_EXTI2);
}
void EXT_int_enable_motor_2()
{
	NVIC_InitTypeDef NVIC_InitStructure_EXTI2;
	NVIC_InitStructure_EXTI2.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure_EXTI2.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStructure_EXTI2.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure_EXTI2.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure_EXTI2);
}
void EXTI15_10_IRQHandler()
{

	   if (EXTI_GetITStatus(EXTI_Line10) != RESET&&(flag_2==0))
	         {
	    		EXTI_ClearITPendingBit(EXTI_Line10);
	               flag_2=1;
	           	GPIO_ResetBits(GPIOE,GPIO_Pin_9);
	           }
	    if (EXTI_GetITStatus(EXTI_Line10) != RESET&&(flag_2==1))
	           {
	    		EXTI_ClearITPendingBit( EXTI_Line10);
	                flag_2=0;
	            	GPIO_SetBits(GPIOE,GPIO_Pin_9);
	           }
	    EXTI_ClearITPendingBit(EXTI_Line10);
}
int main(void)
{
	GPIO_LED_peripheral();
	enable_input_for_interrupt_1();
	EXT_int_setting_motor_1();
	EXT_int_enable_motor_1();
	enable_input_for_interrupt_2();
	EXT_int_setting_motor_2();
	EXT_int_enable_motor_2();
	GPIO_USART_peripheral();
	USART_enable();
	USART_interrupt_enable();
	while(1)
	{
		if(flag == 1)
		{
			TIM_Cmd(TIM4, ENABLE);
			TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
			send_ticks(1, Data);
			send_ticks(2, Data);
			flag = 0;
		}
	}
}
