#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx.h>
void gpio_init()
{
	    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		GPIO_InitTypeDef GPIO_Initstructure;
		GPIO_Initstructure.GPIO_Pin = GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
		GPIO_Initstructure.GPIO_Mode=GPIO_Mode_OUT;
		GPIO_Initstructure.GPIO_Speed=GPIO_Speed_100MHz;
		GPIO_Initstructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
		GPIO_Initstructure.GPIO_OType=GPIO_OType_PP;

		GPIO_Init(GPIOD, &GPIO_Initstructure);
 }
void delay(uint32_t n)
{
	while(n);
	n--;
}
int main()
{
	gpio_init();

 while(1)
	{
	  gpio_init();
       GPIO_SetBits(GPIOD, GPIO_Pin_12);
      delay(0xFFFFFF);
   GPIO_ResetBits(GPIOD, GPIO_Pin_All);
   GPIO_SetBits(GPIOD, GPIO_Pin_13);
   delay(0xFFFFFF);
   GPIO_ResetBits(GPIOD, GPIO_Pin_All);
   GPIO_SetBits(GPIOD, GPIO_Pin_14);
   delay(0xFFFFFF);
   GPIO_ResetBits(GPIOD, GPIO_Pin_All);
   GPIO_SetBits(GPIOD, GPIO_Pin_15);
   delay(0xFFFFFF);
   GPIO_ResetBits(GPIOD, GPIO_Pin_All);
	}
}


