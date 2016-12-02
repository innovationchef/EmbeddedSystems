/*
#include <stm32f4xx.h>				//	without this lib also the code will run coz this lib is included in below attached libraries
#include <stm32f4xx_gpio.h>					//	include the main file
#include <stm32f4xx_rcc.h>					// include the clock library
#include <stm32f4xx_usart.h>				// usart library

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}

void GPIO_USART_peripheral()
{
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		//GPIOD Periph clock enable
			GPIO_InitTypeDef  GPIO_InitStructure;						// initialize Alternative Functions

			//Set USART1 Tx (PB.06) as AF push-pull
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			//Set USART1 Rx (PB.07) as input floating
			//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

}
void USART_enable()
{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //for USART1 and USART6
			USART_Cmd(USART1, ENABLE);
			USART_InitTypeDef USART_InitStructure;
			USART_InitStructure.USART_BaudRate = 38400;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			USART_InitStructure.USART_Parity = USART_Parity_No ;
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			//Write USART1 parameters
			USART_Init(USART1, &USART_InitStructure);
			//Enable USART1

}


void send_message(char ch)
{

			  //Loop until the end of transmission
			  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			  {
			  }
			  USART_SendData(USART1, ch);
}
uint8_t recieve_message(void)
{
			 while ( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
				return (uint8_t)USART_ReceiveData(USART1);
}

char recieve_my_name()
{
			uint8_t ch;
			ch=recieve_message();
			return ch;
}
void main()
{
			char ch;
			GPIO_USART_peripheral();
			USART_enable();
			while(1)
			{
				ch=recieve_my_name();
				Delay(1000);
				send_message(ch);
				GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
				GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
			}

}
*/
