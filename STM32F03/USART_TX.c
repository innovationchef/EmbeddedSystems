/*
#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_tim.h>
#include <stm32f30x_exti.h>
#include <stm32f30x_syscfg.h>
#include <stm32f30x_misc.h>
#include <stm32f30x_usart.h>
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
void main()
{
			GPIO_USART_peripheral();
			USART_enable();
			while(1)
			{
				char str[] = "hello";
				char *s;
				s = str;
				while(*s)
				{
				 while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
				 USART_SendData(USART2, *s++);

				}

			}
}


*/
