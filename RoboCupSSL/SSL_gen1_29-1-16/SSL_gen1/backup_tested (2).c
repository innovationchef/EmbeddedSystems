#include "defines.h"
#include "tm_stm32f4_nrf24l01.h"
#include "tm_stm32f4_nrf24l01.c"
#include "tm_stm32f4_spi.h"
#include "tm_stm32f4_spi.c"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>
#include <misc.h>
#include "stm32f4xx_MPU6050.h"
#include "semihosting.h"
#include "stm32f4xx_conf.h"
#include <stm32f4xx_tim.h>
#define TRUE 1
#define FALSE 0
#define CHIP_KICKER 0
#define STRAIGHT_KICKER 1
#define KICKER_LEVEL_1 0
#define KICKER_LEVEL_2 1
#define KICKER_LEVEL_3 2
#define pld 7
uint8_t xbee_dataIn;
uint8_t MyAddress[]={0xC2,0xC2,0xC2,0xC2,0xC2};
uint8_t TxAddress[]={0xE7,0xE7,0xE7,0xE7,0xE7};
volatile uint8_t dataIn[]={0};
volatile uint8_t dataSaved[7];
volatile uint8_t mpuInterrupt = 0,dmpReady = 0;
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64];
int32_t q[4];
void savedataIn(uint8_t* data)
{
	volatile int i,ct=1;
	for (i=0;i<pld;++i){
		dataSaved[i]+= data[i];
}
return(ct);
}
void nrf_dataIn_interrupt_gpio()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    NVIC_InitTypeDef nvic_Struct;
    nvic_Struct.NVIC_IRQChannel=EXTI15_10_IRQn;
    nvic_Struct.NVIC_IRQChannelPreemptionPriority= 1;
    nvic_Struct.NVIC_IRQChannelSubPriority= 0;
    nvic_Struct.NVIC_IRQChannelCmd= ENABLE;
    NVIC_Init(&nvic_Struct);

    EXTI_InitTypeDef EXTI_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
    EXTI_InitStruct.EXTI_Line = EXTI_Line13;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);


}
void EXTI15_10_IRQHandler()
{
	if((EXTI->IMR & EXTI_IMR_MR13) && (EXTI->PR & EXTI_PR_PR13))
	{
		TM_NRF24L01_GetData(dataIn);
		TM_NRF24L01_PowerUpRx();
		savedataIn(dataIn);
		EXTI_ClearITPendingBit(EXTI_Line13);
		NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	}
}

void xbee_dataIn_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	USART_Cmd(USART3, ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART3,USART_IT_TXE,ENABLE);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}
void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)      //Check if interrupt was because data is received
    {
    	xbee_dataIn = USART_ReceiveData(USART3);
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}
void MPU6050_External_Interrupt_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource4);
	GPIO_InitTypeDef GPIO_C;
	GPIO_C.GPIO_Mode = GPIO_Mode_IN;
	GPIO_C.GPIO_Pin = GPIO_Pin_4;
	GPIO_C.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_C.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_C.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC,&GPIO_C);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource4);

	EXTI_InitTypeDef EXTI_PC4;
	EXTI_PC4.EXTI_Line = EXTI_Line4;
	EXTI_PC4.EXTI_LineCmd = ENABLE;
	EXTI_PC4.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_PC4.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_PC4);

	NVIC_InitTypeDef NVIC_PC4;
	NVIC_PC4.NVIC_IRQChannel =  EXTI4_IRQn;
	NVIC_PC4.NVIC_IRQChannelCmd = ENABLE;
	NVIC_PC4.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_PC4.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_PC4);
}
void EXTI4_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
		{
		mpuInterrupt = 1;
		EXTI_ClearITPendingBit(EXTI_Line4);
		}
}
void GPIO_Motors_Init()
{
	GPIO_InitTypeDef  GPIOB;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIOB.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_0 | GPIO_Pin_3;
	GPIOB.GPIO_Mode = GPIO_Mode_OUT;
	GPIOB.GPIO_OType = GPIO_OType_PP;
	GPIOB.GPIO_Speed = GPIO_Speed_100MHz;
	GPIOB.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIOB);

	GPIO_InitTypeDef  GPIOE;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIOE.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIOE.GPIO_Mode = GPIO_Mode_OUT;
	GPIOE.GPIO_OType = GPIO_OType_PP;
	GPIOE.GPIO_Speed = GPIO_Speed_100MHz;
	GPIOE.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIOE);
}
void setFWDbit(int motorID, int value)
{
	switch (motorID){
		case 1:
			if(value==1){
				GPIO_SetBits(GPIOB, GPIO_Pin_0);
			}else if(value==0){
				GPIO_ResetBits(GPIOB, GPIO_Pin_0);
			}break;
		case 2:
			if(value==1){
				GPIO_SetBits(GPIOE, GPIO_Pin_7);
			}else if(value==0){
				GPIO_ResetBits(GPIOE, GPIO_Pin_7);
			}break;
		case 3:
			if(value==1){
				GPIO_SetBits(GPIOE, GPIO_Pin_9);
			}else if(value==0){
				GPIO_ResetBits(GPIOE, GPIO_Pin_9);
			}break;
		case 4:
			if(value==1){
				GPIO_SetBits(GPIOE, GPIO_Pin_11);
			}else if(value==0){
				GPIO_ResetBits(GPIOE, GPIO_Pin_11);
			}break;
		case 5:
			if(value==1){
				GPIO_SetBits(GPIOE, GPIO_Pin_13);
			}else if(value==0){
				GPIO_ResetBits(GPIOE, GPIO_Pin_13);
			}break;
	}
}
void setBKDbit(int motorID, int value)
{
	switch (motorID){
		case 1:
			if(value==1){
				GPIO_SetBits(GPIOB, GPIO_Pin_1);
			}else if(value==0){
				GPIO_ResetBits(GPIOB, GPIO_Pin_1);
			}break;
		case 2:
			if(value==1){
				GPIO_SetBits(GPIOE, GPIO_Pin_8);
			}else if(value==0){
				GPIO_ResetBits(GPIOE, GPIO_Pin_8);
			}break;
		case 3:
			if(value==1){
				GPIO_SetBits(GPIOE, GPIO_Pin_10);
			}else if(value==0){
				GPIO_ResetBits(GPIOE, GPIO_Pin_10);
			}break;
		case 4:
			if(value==1){
				GPIO_SetBits(GPIOE, GPIO_Pin_12);
			}else if(value==0){
				GPIO_ResetBits(GPIOE, GPIO_Pin_12);
			}break;
		case 5:
			if(value==1){
				GPIO_SetBits(GPIOE, GPIO_Pin_14);
			}else if(value==0){
				GPIO_ResetBits(GPIOE, GPIO_Pin_14);
			}break;
	}
}
void pwmToWheels_init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 2047;
	TIM_TimeBaseStructure.TIM_Prescaler = 9;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);
}
void pwmToWheels_set(int channel, int duty_cycle)
{
	switch (channel){
		case 1: TIM4->CCR1 = duty_cycle; break;
		case 2: TIM4->CCR2 = duty_cycle; break;
		case 3: TIM4->CCR3 = duty_cycle; break;
		case 4: TIM4->CCR4 = duty_cycle; break;
	}
}
void pwmToDribbler_init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_TIM2);
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 2047;
	TIM_TimeBaseStructure.TIM_Prescaler = 9;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
}
void pwmToDribbler_set(int duty_cycle)
{
	TIM2->CCR3 = duty_cycle;
}
void motorInterrupt_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource5);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource6);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource7);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource8);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource9);

	GPIO_InitTypeDef GPIO_PC5;
	GPIO_PC5.GPIO_Mode = GPIO_Mode_IN;
	GPIO_PC5.GPIO_Pin = GPIO_Pin_5;
	GPIO_PC5.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_PC5.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_PC5.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC,&GPIO_PC5);

	GPIO_InitTypeDef GPIO_PC6;
	GPIO_PC6.GPIO_Mode = GPIO_Mode_IN;
	GPIO_PC6.GPIO_Pin = GPIO_Pin_6;
	GPIO_PC6.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_PC6.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_PC6.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC,&GPIO_PC6);

	GPIO_InitTypeDef GPIO_PC7;
	GPIO_PC7.GPIO_Mode = GPIO_Mode_IN;
	GPIO_PC7.GPIO_Pin = GPIO_Pin_7;
	GPIO_PC7.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_PC7.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_PC7.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC,&GPIO_PC7);

	GPIO_InitTypeDef GPIO_PC8;
	GPIO_PC8.GPIO_Mode = GPIO_Mode_IN;
	GPIO_PC8.GPIO_Pin = GPIO_Pin_8;
	GPIO_PC8.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_PC8.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_PC8.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC,&GPIO_PC8);

	GPIO_InitTypeDef GPIO_PC9;
	GPIO_PC9.GPIO_Mode = GPIO_Mode_IN;
	GPIO_PC9.GPIO_Pin = GPIO_Pin_8;
	GPIO_PC9.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_PC9.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_PC9.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC,&GPIO_PC9);

	EXTI_InitTypeDef EXTI_PC5;
	EXTI_PC5.EXTI_Line = EXTI_Line5;
	EXTI_PC5.EXTI_LineCmd = ENABLE;
	EXTI_PC5.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_PC5.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_PC5);

	EXTI_InitTypeDef EXTI_PC6;
	EXTI_PC6.EXTI_Line = EXTI_Line6;
	EXTI_PC6.EXTI_LineCmd = ENABLE;
	EXTI_PC6.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_PC6.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_PC6);

	EXTI_InitTypeDef EXTI_PC7;
	EXTI_PC7.EXTI_Line = EXTI_Line7;
	EXTI_PC7.EXTI_LineCmd = ENABLE;
	EXTI_PC7.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_PC7.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_PC7);

	EXTI_InitTypeDef EXTI_PC8;
	EXTI_PC8.EXTI_Line = EXTI_Line8;
	EXTI_PC8.EXTI_LineCmd = ENABLE;
	EXTI_PC8.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_PC8.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_PC8);

	EXTI_InitTypeDef EXTI_PC9;
	EXTI_PC9.EXTI_Line = EXTI_Line9;
	EXTI_PC9.EXTI_LineCmd = ENABLE;
	EXTI_PC9.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_PC9.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_PC9);

	NVIC_InitTypeDef NVIC_PC5;
	NVIC_PC5.NVIC_IRQChannel =  EXTI9_5_IRQn;
	NVIC_PC5.NVIC_IRQChannelCmd = ENABLE;
	NVIC_PC5.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_PC5.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_PC5);
}
void EXTI9_5_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
		{
		pwmToWheels_set(2, 614);
		EXTI_ClearITPendingBit(EXTI_Line5);
		}
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
		{
		pwmToWheels_set(2, 614);
		EXTI_ClearITPendingBit(EXTI_Line6);
		}

	if(EXTI_GetITStatus(EXTI_Line7) != RESET)
		{
		pwmToWheels_set(2, 614);
		EXTI_ClearITPendingBit(EXTI_Line7);
		}

	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
		{
		pwmToWheels_set(2, 614);
		EXTI_ClearITPendingBit(EXTI_Line8);
		}

	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
		{
		pwmToWheels_set(2, 614);
		EXTI_ClearITPendingBit(EXTI_Line9);
		}
}
void kicker_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void selectKickerMode(int mode)
{
	if(mode == 0)
	{
		GPIO_SetBits(GPIOD, GPIO_PIN_0);
	}
	else if (mode == 1)
	{
		GPIO_ResetBits(GPIOD, GPIO_PIN_0);
	}
}
void selectKickerLevel(int mode)
{
	if(mode == 0)
	{
		GPIO_SetBits(GPIOD, GPIO_PIN_1);
		GPIO_ResetBits(GPIOD, GPIO_PIN_2);
	}
	else if (mode == 1)
	{
		GPIO_ResetBits(GPIOD, GPIO_PIN_1);
		GPIO_SetBits(GPIOD, GPIO_PIN_2);
	}
	else if (mode == 2)
	{
		GPIO_SetBits(GPIOD, GPIO_PIN_1);
		GPIO_SetBits(GPIOD, GPIO_PIN_2);
	}
}
void kickerOption(int mode)
{
	if(mode == 0)
	{
		GPIO_ResetBits(GPIOD, GPIO_PIN_3);
	}
	else if (mode == 1)
	{
		GPIO_SetBits(GPIOD, GPIO_PIN_3);
	}
}
int main(void)
{
	SystemInit();
	nrf_dataIn_interrupt_gpio();
	TM_NRF24L01_Init(1,pld);
	TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_1M, TM_NRF24L01_OutputPower_M18dBm);
	TM_NRF24L01_SetMyAddress(MyAddress);
	TM_NRF24L01_SetTxAddress(TxAddress);
	//xbee_dataIn_init();
	GPIO_Motors_Init();
	kicker_init();
	pwmToWheels_init();
	pwmToDribbler_init();
	motorInterrupt_init();
	MPU6050_Initialize();
	MPU6050_External_Interrupt_Init();
	Init_led();
	devStatus = MPU6050_dmpInitialize();
	MPU6050_setXGyroOffset(220);
	MPU6050_setYGyroOffset(76);
	MPU6050_setZGyroOffset(-85);
	MPU6050_setZAccelOffset(1788);
	if (devStatus == 0)
	{
		MPU6050_setDMPEnabled(1);
		MPU6050_External_Interrupt_Init();
		mpuIntStatus = MPU6050_getIntStatus();
		dmpReady = 1;
		packetSize = MPU6050_dmpGetFIFOPacketSize();
	}
	else
	{
	}
	while(1)
	{
		if (!dmpReady) return(0);
		while (!mpuInterrupt && fifoCount < packetSize);
		mpuInterrupt = 0;
		mpuIntStatus = MPU6050_getIntStatus();
		fifoCount = MPU6050_getFIFOCount();
		if ((mpuIntStatus & 0x10) || fifoCount == 1024)
		{
			MPU6050_resetFIFO();
		}
		else if (mpuIntStatus & 0x02)
		{
			while (fifoCount < packetSize) fifoCount = MPU6050_getFIFOCount();
			MPU6050_getFIFOBytes(fifoBuffer, packetSize);
			fifoCount -= packetSize;
			MPU6050_dmpGetQuaternion(q, fifoBuffer);
			printf("%d", q[0]);
			SH_SendChar('\n');
			printf("%d", q[1]);
			SH_SendChar('\n');
			printf("%d", q[2]);
			SH_SendChar('\n');
			printf("%d", q[3]);
			SH_SendChar('\n');
		}
	}
}
