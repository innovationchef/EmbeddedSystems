/*
#include <stm32f4xx.h>				//	without this lib also the code will run coz this lib is included in below attached libraries
#include <stm32f4xx_gpio.h>			//	include the gpio file
#include <stm32f4xx_rcc.h>			//include the clock library
//=================================================================================================
//											DELAY FUNCTION
//	 __IO is a macro defined in the Cortex-M4.h include file.
//	Details on "answers.yahoo.com/question/index?qid=20130825125345AAHR2bx"
//	and www.geeksforgeeks.org/understanding-volatile-qualifier-in-c/
//	Delay given from this function is random. For exact Timings use TIMERS
//=================================================================================================
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

int main(void)
	{
		GPIO_D_output();
		while(1)
		    {
		    	  	//PB8 and PB12 to be toggled
		    	    GPIO_SetBits(GPIOD, GPIO_Pin_12);
		    	    GPIO_SetBits(GPIOD, GPIO_Pin_13);
		    	    GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		    	    GPIO_ResetBits(GPIOD, GPIO_Pin_15);
		    	    Delay(1000000);
		    	    GPIO_SetBits(GPIOD, GPIO_Pin_14);
		    	    GPIO_SetBits(GPIOD, GPIO_Pin_15);
		    	    GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		    	    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		    	    Delay(1000000);
		    }
	}
*/
