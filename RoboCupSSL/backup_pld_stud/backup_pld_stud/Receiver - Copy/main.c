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

#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_exti.h"
#include "misc.h"

//int count = 0;
#define pld 32

/* Receiver address */
uint8_t MyAddress[]={0xC2,0xC2,0xC2,0xC2,0xC2};
/* My address */
uint8_t TxAddress[]={0xE7,0xE7,0xE7,0xE7,0xE7};

volatile uint8_t dataIn[]={0};
volatile uint8_t dataIn1[32]= {'a','b','c','d','a','b','c','d','a','b','c','d','a','b','c','d','a','b','c','d','a','b','c','d','a','b','c','d','a','b','c','d'};
volatile uint8_t dataIn2[20]= {'a','b','c','d','a','b','c','d','a','b','c','d','a','b','c','d','a','b','c','d'};

int checkdataIn(uint8_t* data)
{
	volatile int i,ct=1;
	for (i=0;i<pld;++i){
		if(dataIn[i]!=data[i]){ct=0; break;}
		else ct=1;
	}
return(ct);
}

void gpio_exti()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
}
void exti_interrupt()
{
    NVIC_InitTypeDef nvic_Struct;
    nvic_Struct.NVIC_IRQChannel                     = EXTI15_10_IRQn;
    nvic_Struct.NVIC_IRQChannelPreemptionPriority     = 0;
    nvic_Struct.NVIC_IRQChannelSubPriority             = 0;
    nvic_Struct.NVIC_IRQChannelCmd                     = ENABLE;
    NVIC_Init(&nvic_Struct);
}
void exti_init()
{
    EXTI_InitTypeDef EXTI_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
    EXTI_InitStruct.EXTI_Line                         = EXTI_Line13;
    EXTI_InitStruct.EXTI_Mode                         = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger                     = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd                     = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
}

void EXTI15_10_IRQHandler()
{
    //if((EXTI_GetITStatus(EXTI_Line13))!=RESET)
      if((EXTI->IMR & EXTI_IMR_MR13) && (EXTI->PR & EXTI_PR_PR13))
	    {
        TM_NRF24L01_GetData(dataIn);
        TM_NRF24L01_PowerUpRx();

            if (checkdataIn(dataIn1)) {
                         TM_DISCO_LedOn(LED_GREEN);
                         TM_DISCO_LedOff(LED_BLUE);
                         TM_DISCO_LedOff(LED_ORANGE);
                         TM_DISCO_LedOff(LED_RED);

            }
            else {
                    TM_DISCO_LedOn(LED_ORANGE);
                    TM_DISCO_LedOn(LED_RED);
                    TM_DISCO_LedOn(LED_GREEN);
                    TM_DISCO_LedOn(LED_BLUE);

                }
            EXTI_ClearITPendingBit(EXTI_Line13);
            NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
        }
}

int main(void) {
    SystemInit();

    TM_DISCO_LedInit();
    gpio_exti();
       exti_init();
       exti_interrupt();

    TM_NRF24L01_Init(1,pld);



    /* Set RF settings, Data rate to 2Mbps, Output power to -18dBm */
    TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_250k, TM_NRF24L01_OutputPower_0dBm);

    /* Set my address, 5 bytes */
    TM_NRF24L01_SetMyAddress(MyAddress);
    //NRF24L01_CE_LOW;
    //TM_NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P1, MyAddress, 5);
    //NRF24L01_CE_HIGH;
    /* Set TX address, 5 bytes */
    TM_NRF24L01_SetTxAddress(TxAddress);
    //TM_NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P0, TxAddress, 5);
    //TM_NRF24L01_WriteRegisterMulti(NRF24L01_REG_TX_ADDR, TxAddress, 5);

    while (1) {


        }
}
