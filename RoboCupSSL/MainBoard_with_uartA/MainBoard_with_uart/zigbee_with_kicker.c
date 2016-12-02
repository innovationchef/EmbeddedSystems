#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#define pld 26
#define X 100
#define Y 200
#define TRUE 1
#define FALSE 0
volatile int k =6;
int j;
volatile int c;
volatile uint8_t dataIn[5]={0};
//volatile uint8_t dataIn1[26]= {1,X,X,X,X,Y,50,100,50,100,0,50,100,50,100,0,50,100,50,100,0,50,100,50,100,0};
volatile uint8_t dataIn1[26]=   {1,X,X,X,X,0,50,100,50,100,0,50,100,50,100,0,50,100,50,100,0,50,100,50,100,0};
//volatile uint8_t dataIn1[32]= {1,28,28,28,28,186,100,50,100,0,50,100,50,100,0,50,100,50,100,0,50,100,50,100,0};
volatile int velocity[4]={0};
volatile int dribbler=2000;
volatile int bot_id;
int i=0;
int flag=0;
int checkdataIn(uint8_t* data)
{
	volatile int i,ct=1;
	for (i=0;i<pld;++i){
		if(dataIn[i]!=data[i]){ct=0; break;}
		else ct=1;
	}
return(ct);
}
int map(int value){
   return 204+value*(1820-204)/128;
}
void init_kicker()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void init_botid()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

 	bot_id = ((GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)*8) + (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)*4) + (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)*2) + GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5));

}
void read_botid()
{
 	bot_id = ((GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)*8) + (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)*4) + (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)*2) + GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5));

}
void GPIO(){
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11 ;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
	GPIO_Init(GPIOC, &GPIO_InitStruct);

}

void USART(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitStructure.USART_BaudRate=38400;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_Mode=USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_Init(USART3, &USART_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;             // we want to configure the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         // this sets the priority group of the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // the USART2 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);                       // the properties are passed to the NVIC_Init function which takes care of the low level stuff

	USART_Cmd(USART3, ENABLE);
}
void GPIOLED_TEST()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
void GPIOB_Motors()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_0 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void GPIOE_Motors()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_7|GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
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
	NVIC_PC5.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_PC5.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_PC5);
}
void velocity_set(){
	int i = 0;
  for(i=0;i<4;i++){
	if(velocity[i]<128){
	  }
	else{
		velocity[i] = 255-velocity[i];
		velocity[i] = map(velocity[i]);
		setFWDbit(i+1, 1);
		setBKDbit(i+1, 0);
		pwmToWheels_set(i+1,velocity[i]);
	    }
  }
  pwmToDribbler_set(dribbler);
  setFWDbit(5, 0);
  setBKDbit(5, 1);
}
int main()
	{
	    init_kicker();
	    init_botid();
	    GPIO();
		USART();
		GPIOB_Motors();
		GPIOE_Motors();
		GPIOLED_TEST();
		pwmToWheels_init();
		pwmToDribbler_init();
		motorInterrupt_init();
		GPIO_SetBits(GPIOE, GPIO_Pin_1); //RED

		//GPIO_ResetBits(GPIOE, GPIO_Pin_0);   //GREEN
		//GPIO_SetBits(GPIOE, GPIO_Pin_1); //RED
		while(1)
		{
			velocity[0] = 50;
			velocity[0] = map(velocity[0]);
			setFWDbit(1, 0);
			setBKDbit(1, 1);
			pwmToWheels_set(1,velocity[0]);

			velocity[1] = 50;
			velocity[1] = map(velocity[1]);
			setFWDbit(2, 0);
			setBKDbit(2, 1);
			pwmToWheels_set(2,velocity[1]);

			velocity[2] = 50;
			velocity[2] = map(velocity[2]);
			setFWDbit(3, 0);
			setBKDbit(3, 1);
			pwmToWheels_set(3,velocity[2]);

			velocity[3] = 50;
			velocity[3] = map(velocity[3]);
			setFWDbit(4, 0);
			setBKDbit(4, 1);
			pwmToWheels_set(4,velocity[3]);
		}
	}
