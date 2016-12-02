/*#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

void USART(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_Mode=USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);
}

void GPIO(){
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 ;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_USART3);
	GPIO_Init(GPIOD, &GPIO_InitStruct);
}

int main(){
	GPIO();
	USART();
	char c;
while(1)
	{
	while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE)==SET)
	{
		c=USART_ReceiveData(USART3);
	}
	if(c=='a'){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
	}

}
}
*/
