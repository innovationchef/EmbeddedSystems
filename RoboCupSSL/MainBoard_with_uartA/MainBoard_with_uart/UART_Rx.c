#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

void usart_config()
{
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	 GPIO_InitTypeDef GPIO_InitStruct;
	 USART_InitTypeDef USART_InitStructure;

	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	 GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	 GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
	 GPIO_Init(GPIOB, &GPIO_InitStruct);

	 USART_InitStructure.USART_BaudRate = 38400;\
	 USART_InitStructure.USART_Mode = USART_Mode_Rx;
	 USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	 USART_Init(USART1, &USART_InitStructure);
}

void GPIO()
{
	GPIO_InitTypeDef GPIO_Initstruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_12 ;
	GPIO_Initstruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_Initstruct);
}

int main()
{
	usart_config();
	GPIO();
	char c;
while(1)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET);
	{
		c=USART_ReceiveData(USART1);
	}
	if(c=='a'){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
	}
}
}

