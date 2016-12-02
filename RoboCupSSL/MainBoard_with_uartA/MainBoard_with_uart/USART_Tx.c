/*#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

void USART(){
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=38400;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_Mode=USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}
void GPIO(){
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
	GPIO_Init(GPIOD, &GPIO_InitStruct);

}

int main(){
	GPIO();
	USART();


while(1){


	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	  {
		USART_SendData(USART1, 'a');

	  }
	}
}
*/
