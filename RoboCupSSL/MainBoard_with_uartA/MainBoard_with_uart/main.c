/*#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
}
void delay(uint32_t n){
	while(n)
		n--;
}
int main(void)
{
	GPIO_Config();
	GPIO_SetBits(GPIOD, GPIO_Pin_12);
    while(1)
    {
    	GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
    	delay(0xFFFFFF);

    }
}
*/
