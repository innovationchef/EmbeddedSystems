//#include <stm32f4xx_gpio.h>
#include "stm32f4xx.h"
#include "defines.h"
#include "tm_stm32f4_nrf24l01.h"
#include "tm_stm32f4_nrf24l01.c"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_disco.c"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_delay.c"
#include "tm_stm32f4_spi.h"
#include "tm_stm32f4_spi.c"
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>
#include <misc.h>
#define DATA_SIZE 32
#define X 100
TM_NRF24L01_Transmit_Status_t transmissionStatus;
uint8_t TxAddress[] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
uint8_t MyAddress[] = {0xE7, 0xE7, 0xE7, 0xE7,  0xE7};
uint8_t dataOut[32]= {1,X,X,X,X,0,50,100,50,100,0,50,100,50,100,0,50,100,50,100,0,50,100,50,100,0};
/**************************************************************************/
/*void LED_glow()
	{
			GPIO_InitTypeDef  GPIO_InitStructure;
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);				//GPIOD Periph clock enable
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;// Configure Pins in output pushpull mode
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						 // OD se nahi chala
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 						// down, up, nopull teeno me chal gaya
			GPIO_Init(GPIOD, &GPIO_InitStructure);
	}*/
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
			USART_InitStructure.USART_StopBits = USART_StopBits_1; //works for all values of stop bits
			USART_InitStructure.USART_Parity = USART_Parity_No ;	// works for all parity
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // works forf all values
			//Write USART1 parameters
			USART_Init(USART1, &USART_InitStructure);
			USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
			//Enable USART1
}
void USART_interrupt_enable()
{

			NVIC_InitTypeDef NVIC_InitStruct;
			NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
			NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
			NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
			NVIC_Init(&NVIC_InitStruct);
}
/***************************************************************************/
void USART_puts(USART_TypeDef* USARTx, volatile char *s){
	int i;
	for(i=0; i<DATA_SIZE; i++){
		// wait until data register is empty
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx, *s);
		s++;
	}
}
/***************************************************************************/
volatile char received_string[DATA_SIZE];
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)      //Check if interrupt was because data is received
    {

		static uint8_t cnt = 0; // this counter is used to determine the string length
		char t = USART_ReceiveData(USART1); // the character from the USART1 data register is saved in t
		if(cnt <  DATA_SIZE){
			received_string[cnt] = t;
			cnt++;
		}
		if(cnt == DATA_SIZE) {
			//USART_puts(USART1, received_string);
			 TM_NRF24L01_Transmit(dataOut);
			            do {
			                transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
			            } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);

			            if (transmissionStatus == TM_NRF24L01_Transmit_Status_Ok) {
			            	TM_DISCO_LedOn(LED_BLUE);
			            	TM_DISCO_LedOff(LED_GREEN);
			            	TM_DISCO_LedOff(LED_RED);
			            	TM_DISCO_LedOff(LED_ORANGE);
			                /* Transmit went OK */
			            } else if (transmissionStatus == TM_NRF24L01_Transmit_Status_Lost) {
			            	TM_DISCO_LedOn(LED_RED);
			               	TM_DISCO_LedOff(LED_GREEN);
			               	TM_DISCO_LedOff(LED_BLUE);
			               	TM_DISCO_LedOff(LED_ORANGE);
			                /* Message was LOST */
			            } else {
			            	TM_DISCO_LedOn(LED_ORANGE);
			            	TM_DISCO_LedOff(LED_GREEN);
			            	TM_DISCO_LedOff(LED_RED);
			            	TM_DISCO_LedOff(LED_BLUE);
			                /* This should never happen */
			            }
			            TM_DELAY_SetTime(0);
			            while(TM_DELAY_Time()<50000);
			cnt = 0;
		}
		//
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }

}
int main(void) {
    TM_DELAY_Init();
    TM_DISCO_LedInit();
    TM_NRF24L01_Init(1,32);
    TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_250k, TM_NRF24L01_OutputPower_0dBm);
    TM_NRF24L01_SetMyAddress(MyAddress);
    TM_NRF24L01_SetTxAddress(TxAddress);
    while (1) {
    	TM_NRF24L01_Transmit(dataOut);
    				            do {
    				                transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
    				            } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);

    				            if (transmissionStatus == TM_NRF24L01_Transmit_Status_Ok) {
    				            	TM_DISCO_LedOn(LED_BLUE);
    				            	TM_DISCO_LedOff(LED_GREEN);
    				            	TM_DISCO_LedOff(LED_RED);
    				            	TM_DISCO_LedOff(LED_ORANGE);
    				                /* Transmit went OK */
    				            } else if (transmissionStatus == TM_NRF24L01_Transmit_Status_Lost) {
    				            	TM_DISCO_LedOn(LED_RED);
    				               	TM_DISCO_LedOff(LED_GREEN);
    				               	TM_DISCO_LedOff(LED_BLUE);
    				               	TM_DISCO_LedOff(LED_ORANGE);
    				                /* Message was LOST */
    				            } else {
    				            	TM_DISCO_LedOn(LED_ORANGE);
    				            	TM_DISCO_LedOff(LED_GREEN);
    				            	TM_DISCO_LedOff(LED_RED);
    				            	TM_DISCO_LedOff(LED_BLUE);
    				                /* This should never happen */
    				            }
               }
}
