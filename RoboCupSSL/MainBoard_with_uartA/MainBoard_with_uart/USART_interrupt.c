/*#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

volatile char *buffer;
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

void USART(){
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=38400;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_Mode=USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;             // we want to configure the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         // this sets the priority group of the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // the USART2 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);                       // the properties are passed to the NVIC_Init function which takes care of the low level stuff

	USART_Cmd(USART1, ENABLE);
}
int main() {
	GPIO();
	USART();

	while(1){

	}
}
void USART1_IRQHandler(void){
	if( USART_GetITStatus(USART1, USART_IT_RXNE) ){
		c=USART_ReceiveData(USART1);
		if(c=='a'){
				GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		}
		 USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
*/







