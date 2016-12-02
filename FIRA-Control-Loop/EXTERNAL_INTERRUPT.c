/*#include <stm32f30x.h>
#include <stm32f30x_tim.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_misc.h>
#include <stm32f30x_EXTI.h>
#include <stm32f30x_syscfg.h>
#include <stm32f30x_usart.h>

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
	GPIO_InitTypeDef  GPIO_InitStructure_EXTI2;
	GPIO_InitStructure_EXTI2.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure_EXTI2.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure_EXTI2.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure_EXTI2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure_EXTI2.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure_EXTI2);
}
void EXT_int_setting_motor_2()
{
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
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

	   if (EXTI_GetITStatus(EXTI_Line10) != RESET&&(flag_2==0))
	         {
	    		EXTI_ClearITPendingBit(EXTI_Line10);
	               flag_2=1;
	           	GPIO_ResetBits(GPIOE,GPIO_Pin_9);
	           }
	    if (EXTI_GetITStatus(EXTI_Line10) != RESET&&(flag_2==1))
	           {
	    		EXTI_ClearITPendingBit( EXTI_Line10);
	                flag_2=0;
	            	GPIO_SetBits(GPIOE,GPIO_Pin_9);
	           }
	    EXTI_ClearITPendingBit(EXTI_Line10);
}
int main(void)
{
	GPIO_LED_peripheral();
				enable_input_for_interrupt_1();
				EXT_int_setting_motor_1();
				EXT_int_enable_motor_1();
				enable_input_for_interrupt_2();
								EXT_int_setting_motor_2();
								EXT_int_enable_motor_2();
				while(1){}
}
*/
