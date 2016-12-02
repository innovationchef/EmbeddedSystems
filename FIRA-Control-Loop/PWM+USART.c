
#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_tim.h>
#include <stm32f30x_misc.h>
#include <stm32f30x_usart.h>
#include <stm32f30x_EXTI.h>
#include <stm32f30x_syscfg.h>
volatile int kp=5, kd=8, ki=0;
volatile int error=0, deltaerror=0, lasterror=0, sumoferror;
volatile int tick_to_send=0 , desired_ticks=50;
volatile int recieve_flag=0;
volatile int Data=0;
volatile int flag=0;
void GPIO_USART_peripheral()
{
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
			GPIO_InitTypeDef  GPIO_InitStructure;
			//Set USART2 Tx (PA.02) as AF push-pull
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			//Set USART2 Rx (PA.03) as input floating
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_7);
}
void USART_enable()
{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
			USART_InitTypeDef USART_InitStructure;
			USART_InitStructure.USART_BaudRate = 38400;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			USART_InitStructure.USART_Parity = USART_Parity_No;
			USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			USART_Init(USART2, &USART_InitStructure);
			USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
			USART_Cmd(USART2, ENABLE);
}
void USART_interrupt_enable()
{
			NVIC_InitTypeDef NVIC_InitStruct;
			NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
			NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
			NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
			NVIC_Init(&NVIC_InitStruct);
}
void USART2_IRQHandler(void)
{
			Data = USART_ReceiveData(USART2);
			 /*switch (recieve_flag){
				    case 0:
				    {
				    	kp = Data;
				    	recieve_flag++;
				    	break;
				    }
				    case 1:
				    {
				    	kd = Data;
				    	recieve_flag++;
				    	break;
				    }
				    case 2:
					{
						desired_ticks = Data;
						recieve_flag=0;
						flag= 1;
						break;
					}
			 }*/
			flag=1;
			USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}

void pwm_to_motor()
			{
			GPIO_InitTypeDef GPIO_InitStructure;
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_2); // TIM1_CH1 -> PE09
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_2); // TIM1_CH2 -> PE11
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_2); // TIM1_CH3 -> PE13
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_2); // TIM1_CH4 -> PE14
}
void initialize_pwm(int pwm_freq)
{
			int original_ticks = 84e6;
			int pwm_period = 255;
			int prescaler = original_ticks/(pwm_period * pwm_freq)-1;
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			TIM_TimeBaseStructure.TIM_Prescaler = 1000-1;
			TIM_TimeBaseStructure.TIM_Period = pwm_period;
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;
			TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
			TIM_OCInitTypeDef  TIM_OCInitStructure;
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
			//TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
			//Set up 4 channels
			TIM_OC1Init(TIM4,&TIM_OCInitStructure);
			TIM_OC2Init(TIM4,&TIM_OCInitStructure);
			TIM_OC3Init(TIM4,&TIM_OCInitStructure);
			TIM_OC4Init(TIM4,&TIM_OCInitStructure);
			TIM_Cmd(TIM4,ENABLE);
			TIM_SetCompare1(TIM4,0);
			TIM_SetCompare3(TIM4,0);
			TIM_SetCompare2(TIM4,0);
			TIM_SetCompare4(TIM4,0);
}
void send_ticks(int channel,int ticks)
{
			switch (channel)
				{
				case 1: TIM_SetCompare1(TIM4,ticks); break;
				case 2: TIM_SetCompare2(TIM4,ticks); break;
				case 3: TIM_SetCompare3(TIM4,ticks); break;
				case 4: TIM_SetCompare4(TIM4,ticks); break;
				}
}
volatile int count_motor_1_ticks=0;
volatile int flag_1=0, flag_2=0;
void GPIO_LED_peripheral()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE,ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure2;
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure2.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure2);
}
void enable_input_for_interrupt_1()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD,ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure_EXTI1;
	GPIO_InitStructure_EXTI1.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure_EXTI1.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure_EXTI1.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure_EXTI1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure_EXTI1.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOD, &GPIO_InitStructure_EXTI1);
}
void EXT_int_setting_motor_1()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource5);
	EXTI_InitTypeDef  EXTI_InitStructure_EXTI1;
	EXTI_InitStructure_EXTI1.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure_EXTI1.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure_EXTI1.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure_EXTI1.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure_EXTI1);
}
void EXT_int_enable_motor_1()
{
	NVIC_InitTypeDef NVIC_InitStructure_EXTI1;
	NVIC_InitStructure_EXTI1.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure_EXTI1.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStructure_EXTI1.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure_EXTI1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure_EXTI1);
}
void EXTI9_5_IRQHandler()
{

	   if (EXTI_GetITStatus(EXTI_Line5) != RESET&&(flag_1==0))
	         {
	    		EXTI_ClearITPendingBit(EXTI_Line5);
	               flag_1=1;
	           	GPIO_ResetBits(GPIOE,GPIO_Pin_8);
	           }
	    if (EXTI_GetITStatus(EXTI_Line5) != RESET&&(flag_1==1))
	           {
	    		EXTI_ClearITPendingBit( EXTI_Line5);
	                flag_1=0;
	            	GPIO_SetBits(GPIOE,GPIO_Pin_8);
	           }
	    EXTI_ClearITPendingBit(EXTI_Line5);
}
void enable_input_for_interrupt_2()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure_EXTI2;
	GPIO_InitStructure_EXTI2.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure_EXTI2.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure_EXTI2.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure_EXTI2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure_EXTI2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure_EXTI2);
}
void EXT_int_setting_motor_2()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource10);
	EXTI_InitTypeDef  EXTI_InitStructure_EXTI2;
	EXTI_InitStructure_EXTI2.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure_EXTI2.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure_EXTI2.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure_EXTI2.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure_EXTI2);
}
void EXT_int_enable_motor_2()
{
	NVIC_InitTypeDef NVIC_InitStructure_EXTI2;
	NVIC_InitStructure_EXTI2.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure_EXTI2.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStructure_EXTI2.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure_EXTI2.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure_EXTI2);
}
void EXTI15_10_IRQHandler()
{
		count_motor_1_ticks++;
	    EXTI_ClearITPendingBit(EXTI_Line10);
}
volatile int Store_ticks[200];
volatile int i=0,j=0;
volatile int flag_timer_interrupt=0;
volatile int flags = 0;
void TIMER_enable()
{
			// TIME enable
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			TIM_TimeBaseInitStruct.TIM_Period = 31;
			TIM_TimeBaseInitStruct.TIM_Prescaler = 3599;
			TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //c file 566 line
			TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
}

void TIMER_interrupt_enable()
{
			NVIC_InitTypeDef NvicStructure;
			NvicStructure.NVIC_IRQChannel = TIM3_IRQn;
			NvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NvicStructure.NVIC_IRQChannelSubPriority = 0;
			NvicStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NvicStructure);
}

void TIM3_IRQHandler()
{

	if (TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET)
				{
					flag_timer_interrupt=1;
					TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

				}

}
int main()
{
					TIMER_enable();
					TIMER_interrupt_enable();
					pwm_to_motor();
					initialize_pwm(2000);
					enable_input_for_interrupt_1();
					EXT_int_setting_motor_1();
					EXT_int_enable_motor_1();
					enable_input_for_interrupt_2();
					EXT_int_setting_motor_2();
					EXT_int_enable_motor_2();
					GPIO_USART_peripheral();
					USART_enable();
					USART_interrupt_enable();
			while(1)
			{
				if(flag==1)
				{

					TIM_Cmd(TIM3, ENABLE);
					TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
					/*EXTI_InitTypeDef  EXTI_InitStructure_EXTI2;
					EXTI_InitStructure_EXTI2.EXTI_LineCmd = ENABLE;
					EXTI_Init(&EXTI_InitStructure_EXTI2);*/
					count_motor_1_ticks=0;
					error=0;
					lasterror=0;
					deltaerror=0;
					TIM_SetCompare1(TIM4,Data);
					//TIM_SetCompare1(TIM4, Data);
					//send_ticks(1, Data);
					flag=0;
				}
				if(flag_timer_interrupt==1)
				{
					error=Data-count_motor_1_ticks;
					deltaerror=error-lasterror;
					//sumoferror+=error;
					tick_to_send+=kp*error+kd*deltaerror+ki*sumoferror;
					lasterror=error;
					if(tick_to_send>255)
					{
						tick_to_send=255;
					}
					else if(tick_to_send<-255)
					{
						tick_to_send=-255;
					}
					if(tick_to_send>=0)
					{
						TIM_SetCompare1(TIM4, tick_to_send);
						TIM_SetCompare3(TIM4, 0);
					}
					else
					{
						TIM_SetCompare1(TIM4, 0);
						TIM_SetCompare3(TIM4, -tick_to_send);
					}
					//TIM_SetCompare1(TIM4, Data);
					Store_ticks[j]=count_motor_1_ticks;
					count_motor_1_ticks=0;
					flag_timer_interrupt=0;
					i++;j++;
				}
				if(i>200)
				{
					//TIM_Cmd(TIM4, DISABLE);
					TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
					TIM_Cmd(TIM3, DISABLE);
					TIM_SetCompare1(TIM4,0);
					TIM_SetCompare3(TIM4,0);

					//send_ticks(1,0);
					count_motor_1_ticks=0;
					for(j=0;j<200;j++)
					{
						while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
							USART_SendData(USART2, Store_ticks[j]);
					}
					i=0;j=0;
				}
			}
}
