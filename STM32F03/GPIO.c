/*
#include <stm32f30x.h>				//	without this lib also the code will run coz this lib is included in below attached libraries
#include <stm32f30x_gpio.h>			//	include the gpio file
#include <stm32f30x_rcc.h>			//include the clock library
//=================================================================================================
//								DELAY FUNCTION
//	Delay given from this function is random. For exact Timings use TIMERS
// LEDs from PORT 8 to 15
//=================================================================================================
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}

void GPIO_E_output()
	{
			GPIO_InitTypeDef  GPIO_InitStructure;
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);				//GPIOD Periph clock enable
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_8;// Configure Pins in output pushpull mode
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						 // OD se nahi chala
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 						// down, up, nopull teeno me chal gaya
			GPIO_Init(GPIOE, &GPIO_InitStructure);
	}

void main(void)
	{
		GPIO_E_output();
		while(1)
		    {
		    	  	//PB8 and PB10 to be toggled
		    	    GPIO_SetBits(GPIOE, GPIO_Pin_10);
		    	    GPIO_ResetBits(GPIOE, GPIO_Pin_8);
		    	    Delay(1000000);
		    	    GPIO_ResetBits(GPIOE, GPIO_Pin_10);
		    	    GPIO_SetBits(GPIOE, GPIO_Pin_8);
		    	    Delay(1000000);
		    }
	}

*/
