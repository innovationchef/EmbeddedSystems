//#include <stm32f4xx_gpio.h>
#include "stm32f4xx.h"
#include "defines.h"
#include <tm_stm32f4_nrf24l01.h>
#include <tm_stm32f4_nrf24l01.c>
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_disco.c"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_delay.c"
#include "tm_stm32f4_spi.h"
#include "tm_stm32f4_spi.c"
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_usart.h>
#include <misc.h>
#define DATA_SIZE 26
#define channel 5
TM_NRF24L01_Transmit_Status_t transmissionStatus;
uint8_t TxAddress[] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
uint8_t MyAddress[] = {0xE7, 0xE7, 0xE7, 0xE7,  0xE7};
uint8_t dataOut[26]= {1,60,60,60,60,100,60,70,80,90,100,50,100,50,100,0,50,100,50,100,0,50,100,50,100,0};
/*****************************escon*********************************************/
void GPIOLED_TEST()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
void TIMER_enable()
{
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			TIM_TimeBaseInitStruct.TIM_Period = 500;
			TIM_TimeBaseInitStruct.TIM_Prescaler = 32000;
			TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //c file 566 line
			TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
			TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
			TIM_Cmd(TIM4, ENABLE);
			TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}
void TIMER_interrupt_enable()
{
			NVIC_InitTypeDef NvicStructure;
			NvicStructure.NVIC_IRQChannel = TIM4_IRQn;
			NvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NvicStructure.NVIC_IRQChannelSubPriority = 1;
			NvicStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NvicStructure);
}
int flag = 0;
void TIM4_IRQHandler()
{
			if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
				{
					TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
					GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
					//flag =1;
				}
}

int main(void) {
	TIMER_enable();
	TIMER_interrupt_enable();
    TM_DELAY_Init();
    TM_DISCO_LedInit();
    TM_NRF24L01_Init(channel ,DATA_SIZE);
    TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_250k, TM_NRF24L01_OutputPower_0dBm);
    TM_NRF24L01_SetMyAddress(MyAddress);
    TM_NRF24L01_SetTxAddress(TxAddress);
    GPIOLED_TEST();
    //GPIO_SetBits(GPIOE, GPIO_Pin_2);
    int n=100000;
    while (1) {
    //TM_NRF24L01_PowerDown();
	TM_NRF24L01_PowerUpTx();
    TM_NRF24L01_Transmit(dataOut);
    do {
    				                transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
    				            } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);

    				            if (transmissionStatus == TM_NRF24L01_Transmit_Status_Ok) {
    				            	TM_DISCO_LedOn(LED_BLUE);
    				            	TM_DISCO_LedOff(LED_GREEN);
    				            	TM_DISCO_LedOff(LED_RED);
    				            	//TM_DISCO_LedOff(LED_ORANGE);
    				                /* Transmit went OK */
    				            } else if (transmissionStatus == TM_NRF24L01_Transmit_Status_Lost) {
    				            	TM_DISCO_LedOn(LED_RED);
    				               	TM_DISCO_LedOff(LED_GREEN);
    				               	TM_DISCO_LedOff(LED_BLUE);
    				               	//TM_DISCO_LedOff(LED_ORANGE);
    				                /* Message was LOST */
    				            } else {
    				            	TM_DISCO_LedOn(LED_GREEN);
    				            	//TM_DISCO_LedOff(LED_ORANGE);
    				            	TM_DISCO_LedOff(LED_RED);
    				            	TM_DISCO_LedOff(LED_BLUE);
    				                /* This should never happen */
    				            }
    				            TM_DELAY_SetTime(0);
				                     while(TM_DELAY_Time()<50000);
               }
}
