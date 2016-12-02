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



/* My address */
uint8_t TxAddress[] = {
    0xC2,
    0xC2,
    0xC2,
    0xC2,
    0xC2
};
/* Receiver address */
uint8_t MyAddress[] = {
    0xE7,
    0xE7,
    0xE7,
    0xE7,
    0xE7
};

uint8_t dataOut[32]= {'a','b','c','d','a','b','c','d','a','b','c','d','a','b','c','d','a','b','c','d','a','b','c','d','a','b','c','d','a','b','c','d'};


int main(void) {
    TM_NRF24L01_Transmit_Status_t transmissionStatus;

    /* Initialize system */
    SystemInit();
    TM_DELAY_Init();
    /* Initialize onboard leds */
    TM_DISCO_LedInit();

    TM_NRF24L01_Init(1,32);

    /* Set 2MBps data rate and -18dBm output power */
    TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_1M, TM_NRF24L01_OutputPower_M18dBm);

    /* Set my address, 5 bytes */
    TM_NRF24L01_SetMyAddress(MyAddress);
    /* Set TX address, 5 bytes */
    TM_NRF24L01_SetTxAddress(TxAddress);


    while (1) {

            /* Transmit data, goes automatically to TX mode */
            TM_NRF24L01_Transmit(dataOut);

            do {
                transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
            } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);


            /* Check transmit status */
            if (transmissionStatus == TM_NRF24L01_Transmit_Status_Ok) {
            	TM_DISCO_LedOn(LED_BLUE);
            	TM_DISCO_LedOff(LED_GREEN);
            	TM_DISCO_LedOff(LED_RED);
            	TM_DISCO_LedOff(LED_ORANGE);
                /* Transmit went OK */

            } else if (transmissionStatus == TM_NRF24L01_Transmit_Status_Lost) {
            	TM_DISCO_LedOn(LED_RED);
               	TM_DISCO_LedOff(LED_GREEN);
               	TM_DISCO_LedOff(LED_BLUE);
               	TM_DISCO_LedOff(LED_ORANGE);
                /* Message was LOST */

            } else {
            	TM_DISCO_LedOn(LED_ORANGE);
            	TM_DISCO_LedOff(LED_GREEN);
            	TM_DISCO_LedOff(LED_RED);
            	TM_DISCO_LedOff(LED_BLUE);
                /* This should never happen */

            }

            TM_DELAY_SetTime(0);
            while(TM_DELAY_Time()<50000);

    }
}
