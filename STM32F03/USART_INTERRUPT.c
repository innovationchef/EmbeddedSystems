
#include <stm32f30x.h>				//	without this lib also the code will run coz this lib is included in below attached libraries
#include <stm32f30x_gpio.h>			//	include the gpio file
#include <stm32f30x_rcc.h>			//include the clock library
#include <stm32f30x_tim.h>			// include timer library
#include <stm32f30x_misc.h>
#include <stm32f30x_usart.h>
volatile int Data;
volatile int flag;
void GPIO_initialisation()
	{
			GPIO_InitTypeDef  GPIO_InitStructure;
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);				//GPIOD Periph clock enable
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;// Configure Pins in output pushpull mode
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						 // OD se nahi chala
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 						// down, up, nopull teeno me chal gaya
			GPIO_Init(GPIOE, &GPIO_InitStructure);
	}
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
void main()
{
			GPIO_initialisation();
			GPIO_USART_peripheral();
			USART_enable();
			USART_interrupt_enable();
			while(1)
			{
				if(flag == 1)
				{
				GPIO_SetBits(GPIOE, GPIO_Pin_12);
				flag = 0;
				}
			}
}

