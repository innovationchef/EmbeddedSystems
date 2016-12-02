
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
void GPIO_D_output()
	{
			GPIO_InitTypeDef  GPIO_InitStructure;
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);				//GPIOD Periph clock enable
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;// Configure Pins in output pushpull mode
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						 // OD se nahi chala
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 						// down, up, nopull teeno me chal gaya
			GPIO_Init(GPIOD, &GPIO_InitStructure);
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
			USART_InitStructure.USART_StopBits = USART_StopBits_1_5; //operates at all 0_5, 1_5, 2, 1
			USART_InitStructure.USART_Parity = USART_Parity_Odd ; //operates at odd, even as well as no
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // works in rtx, cts, rts_cts
			//Write USART1 parameters
			USART_Init(USART1, &USART_InitStructure);
			//Enable USART1

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
			GPIO_D_output();
			GPIO_USART_peripheral();
			USART_enable();
			while(1)
			{
					if(recieve_my_name()=='a')
					{
							//PB8 and PB12 to be toggled
							GPIO_SetBits(GPIOD, GPIO_Pin_12);
							GPIO_SetBits(GPIOD, GPIO_Pin_13);
							GPIO_ResetBits(GPIOD, GPIO_Pin_14);
							GPIO_ResetBits(GPIOD, GPIO_Pin_15);
							Delay(1000000);
					}
					else
					{
							GPIO_SetBits(GPIOD, GPIO_Pin_14);
							GPIO_SetBits(GPIOD, GPIO_Pin_15);
							GPIO_ResetBits(GPIOD, GPIO_Pin_12);
							GPIO_ResetBits(GPIOD, GPIO_Pin_13);
							Delay(1000000);
					}

			}
}
