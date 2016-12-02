/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "tm_stm32f4_i2c.h"

/* Private variables */
static uint32_t TM_I2C_Timeout;
static uint32_t TM_I2C_INT_Clocks[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

/* Private defines */
#define I2C_TRANSMITTER_MODE   0
#define I2C_RECEIVER_MODE      1
#define I2C_ACK_ENABLE         1
#define I2C_ACK_DISABLE        0

/* Private functions */
void TM_I2C1_INT_InitPins(TM_I2C_PinsPack_t pinspack);
void TM_I2C2_INT_InitPins(TM_I2C_PinsPack_t pinspack);
void TM_I2C3_INT_InitPins(TM_I2C_PinsPack_t pinspack);

void TM_I2C_Init(I2C_TypeDef* I2Cx, TM_I2C_PinsPack_t pinspack, uint32_t clockSpeed) {
	I2C_InitTypeDef I2C_InitStruct;
	
	if (I2Cx == I2C1) {
		/* Enable clock */
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
		
		/* Enable pins */
		TM_I2C1_INT_InitPins(pinspack);
		
		/* Check clock, set the lowest clock your devices support on the same I2C but */
		if (clockSpeed < TM_I2C_INT_Clocks[0]) {
			TM_I2C_INT_Clocks[0] = clockSpeed;
		}
		
		/* Set values */
		I2C_InitStruct.I2C_ClockSpeed = TM_I2C_INT_Clocks[0];
		I2C_InitStruct.I2C_AcknowledgedAddress = TM_I2C1_ACKNOWLEDGED_ADDRESS;
		I2C_InitStruct.I2C_Mode = TM_I2C1_MODE;
		I2C_InitStruct.I2C_OwnAddress1 = TM_I2C1_OWN_ADDRESS;
		I2C_InitStruct.I2C_Ack = TM_I2C1_ACK;
		I2C_InitStruct.I2C_DutyCycle = TM_I2C1_DUTY_CYCLE;
		} else if (I2Cx == I2C2) {
		/* Enable clock */
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
		
		/* Enable pins */
		TM_I2C2_INT_InitPins(pinspack);
		
		/* Check clock, set the lowest clock your devices support on the same I2C but */
		if (clockSpeed < TM_I2C_INT_Clocks[1]) {
			TM_I2C_INT_Clocks[1] = clockSpeed;
		}
		
		/* Set values */
		I2C_InitStruct.I2C_ClockSpeed = TM_I2C_INT_Clocks[1];
		I2C_InitStruct.I2C_AcknowledgedAddress = TM_I2C2_ACKNOWLEDGED_ADDRESS;
		I2C_InitStruct.I2C_Mode = TM_I2C2_MODE;
		I2C_InitStruct.I2C_OwnAddress1 = TM_I2C2_OWN_ADDRESS;
		I2C_InitStruct.I2C_Ack = TM_I2C2_ACK;
		I2C_InitStruct.I2C_DutyCycle = TM_I2C2_DUTY_CYCLE;
	} else if (I2Cx == I2C3) {
		/* Enable clock */
		RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
		
		/* Enable pins */
		TM_I2C3_INT_InitPins(pinspack);
		
		/* Check clock, set the lowest clock your devices support on the same I2C but */
		if (clockSpeed < TM_I2C_INT_Clocks[2]) {
			TM_I2C_INT_Clocks[2] = clockSpeed;
		}
		
		/* Set values */
		I2C_InitStruct.I2C_ClockSpeed = TM_I2C_INT_Clocks[2];
		I2C_InitStruct.I2C_AcknowledgedAddress = TM_I2C3_ACKNOWLEDGED_ADDRESS;
		I2C_InitStruct.I2C_Mode = TM_I2C3_MODE;
		I2C_InitStruct.I2C_OwnAddress1 = TM_I2C3_OWN_ADDRESS;
		I2C_InitStruct.I2C_Ack = TM_I2C3_ACK;
		I2C_InitStruct.I2C_DutyCycle = TM_I2C3_DUTY_CYCLE;
	}
	
	/* Disable I2C first */
	I2Cx->CR1 &= ~I2C_CR1_PE;
	
	/* Initialize I2C */
	I2C_Init(I2Cx, &I2C_InitStruct);
	
	/* Enable I2C */
	I2Cx->CR1 |= I2C_CR1_PE;
}

uint8_t TM_I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg) {
	uint8_t d;
	TM_I2C_Start(I2C1, address, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE);
	TM_I2C_WriteData(I2C1, reg);
	TM_I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	d = TM_I2C_ReadNack(I2C1);													//recieving latest register value
	I2C_GenerateSTOP(I2C1, ENABLE);
	return (d);
}


uint8_t TM_I2C_Read_Bits(I2C_TypeDef* I2Cx,uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length) {
	uint8_t b;
	b = TM_I2C_Read(I2Cx, devAddr, regAddr);

	uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
	b &= mask;
	b >>= (bitStart - length + 1);

	return (b);
}


void TM_I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
	uint8_t i;
	TM_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE);
	TM_I2C_WriteData(I2Cx, reg);
	TM_I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	for (i = 0; i < count; i++) {
		if (i == (count - 1)) {
			/* Last byte */
			data[i] = TM_I2C_ReadNack(I2Cx);
		} else {
			data[i] = TM_I2C_ReadAck(I2Cx);
		}
	}
	I2C_GenerateSTOP(I2C1, ENABLE);
}

uint8_t TM_I2C_ReadNoRegister(I2C_TypeDef* I2Cx, uint8_t address) {
	uint8_t data;
	TM_I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	/* Also stop condition happens */
	data = TM_I2C_ReadNack(I2Cx);
	return data;
}

void TM_I2C_ReadMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count) {
	uint8_t i;
	TM_I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	for (i = 0; i < count; i++) {
		if (i == (count - 1)) {
			/* Last byte */
			data[i] = TM_I2C_ReadNack(I2Cx);
		} else {
			data[i] = TM_I2C_ReadAck(I2Cx);
		}
	}
}

void TM_I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data) {

	GPIO_SetBits(GPIOD,GPIO_Pin_14);
	TM_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	TM_I2C_WriteData(I2Cx, reg);
	TM_I2C_WriteData(I2Cx, data);
	TM_I2C_Stop(I2Cx);
	GPIO_ResetBits(GPIOD,GPIO_Pin_14);
}

void TM_I2C_Write_Bit(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg,uint8_t bitNum, uint8_t data){
	uint8_t b;
	b = TM_I2C_Read(I2C1, address, reg);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	return TM_I2C_Write(I2C1,address, reg, b);
}

uint8_t TM_I2C_Write_Bits(I2C_TypeDef* I2Cx,uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
     uint8_t b = TM_I2C_Read(I2C1, devAddr, regAddr);
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= data; // combine data with existing byte
       TM_I2C_Write(I2C1,devAddr, regAddr, b);
       return 1;
}

void TM_I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
	uint8_t i;
	TM_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	TM_I2C_WriteData(I2Cx, reg);
	for (i = 0; i < count; i++)
	{
		TM_I2C_WriteData(I2Cx, data[i]);
	}
	TM_I2C_Stop(I2Cx);
}

void TM_I2C_Write_Word(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint16_t dataw) {
	uint8_t data[2];
	data[0] = (uint8_t)(dataw >> 8);
	data[1] = (uint8_t)dataw;
	TM_I2C_WriteMulti(I2Cx,  address,  reg,  data, 2);

}
void TM_I2C_WriteNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t data) {
	TM_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	TM_I2C_WriteData(I2Cx, data);
	TM_I2C_Stop(I2Cx);
}

void TM_I2C_WriteMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count) {
	uint8_t i;
	TM_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	for (i = 0; i < count; i++) {
		TM_I2C_WriteData(I2Cx, data[i]);
	}
	TM_I2C_Stop(I2Cx);
}

/* Private functions */
int16_t TM_I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack) {
	/* Generate I2C start pulse */
	I2Cx->CR1 |= I2C_CR1_START;
	
	/* Wait till I2C is busy */
	TM_I2C_Timeout = TM_I2C_TIMEOUT;
	while (!(I2Cx->SR1 & I2C_SR1_SB)) {
		if (--TM_I2C_Timeout == 0x00) {
			return 1;
		}
	}

	/* Enable ack if we select it */
	if (ack) {
		I2Cx->CR1 |= I2C_CR1_ACK;
	}

	/* Send write/read bit */
	if (direction == I2C_TRANSMITTER_MODE) {
		/* Send address with zero last bit */
		I2Cx->DR = address & ~I2C_OAR1_ADD0;
		
		/* Wait till finished */
		TM_I2C_Timeout = TM_I2C_TIMEOUT;
		while (!(I2Cx->SR1 & I2C_SR1_ADDR)) {
			if (--TM_I2C_Timeout == 0x00) {
				return 1;
			}
		}
	}
	if (direction == I2C_RECEIVER_MODE) {
		/* Send address with 1 last bit */
		I2Cx->DR = address | I2C_OAR1_ADD0;
		
		/* Wait till finished */
		TM_I2C_Timeout = TM_I2C_TIMEOUT;
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
			if (--TM_I2C_Timeout == 0x00) {
				return 1;
			}
		}
	}
	
	/* Read status register to clear ADDR flag */
	I2Cx->SR2;
	
	/* Return 0, everything ok */
	return 0;
}

void TM_I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data) {
	/* Wait till I2C is not busy anymore */
	TM_I2C_Timeout = TM_I2C_TIMEOUT;
	while (!(I2Cx->SR1 & I2C_SR1_TXE) && TM_I2C_Timeout) {
		TM_I2C_Timeout--;
	}
	
	/* Send I2C data */
	I2Cx->DR = data;
	while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
	/*TM_I2C_Timeout = 50000;
	while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && TM_I2C_Timeout)
	{
		TM_I2C_Timeout--;
	}
*/
}

uint8_t TM_I2C_ReadAck(I2C_TypeDef* I2Cx) {
	uint8_t data;
	
	/* Enable ACK */
	I2Cx->CR1 |= I2C_CR1_ACK;
	
	/* Wait till not received */
	TM_I2C_Timeout = TM_I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if (--TM_I2C_Timeout == 0x00) {
			return 1;
		}
	}
	
	/* Read data */
	data = I2Cx->DR;
	
	/* Return data */
	return data;
}

uint8_t TM_I2C_ReadNack(I2C_TypeDef* I2Cx) {
	uint8_t data;
	
	/* Disable ACK */
	I2Cx->CR1 &= ~I2C_CR1_ACK;
	
	/* Generate stop */
	//I2Cx->CR1 |= I2C_CR1_STOP;
	
	/* Wait till received */
	TM_I2C_Timeout = TM_I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if (--TM_I2C_Timeout == 0x00) {
			return 1;
		}
	}

	/* Read data */
	data = I2Cx->DR;
	
	/* Return data */
	return data;
}

uint8_t TM_I2C_Stop(I2C_TypeDef* I2Cx) {
	/* Wait till transmitter not empty */
	TM_I2C_Timeout = TM_I2C_TIMEOUT;
	while (((!(I2Cx->SR1 & I2C_SR1_TXE)) || (!(I2Cx->SR1 & I2C_SR1_BTF)))) {
		if (--TM_I2C_Timeout == 0x00) {
			return 1;
		}
	}
	
	/* Generate stop */
	I2Cx->CR1 |= I2C_CR1_STOP;
	
	/* Return 0, everything ok */
	return 0;
}

uint8_t TM_I2C_IsDeviceConnected(I2C_TypeDef* I2Cx, uint8_t address) {
	uint8_t connected = 0;
	/* Try to start, function will return 0 in case device will send ACK */
	if (!TM_I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE)) {
		connected = 1;
	}
	
	/* STOP I2C */
	TM_I2C_Stop(I2Cx);
	
	/* Return status */
	return connected;
}

/* Private functions */
void TM_I2C1_INT_InitPins(TM_I2C_PinsPack_t pinspack) {
	/* Init pins */
#if defined(GPIOB)
	if (pinspack == TM_I2C_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_6 | GPIO_PIN_7, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium, GPIO_AF_I2C1);
	}
#endif
#if defined(GPIOB)
	if (pinspack == TM_I2C_PinsPack_2) {
		TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_8 | GPIO_PIN_9, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium, GPIO_AF_I2C1);
	}
#endif
#if defined(GPIOB)
	if (pinspack == TM_I2C_PinsPack_3) {
		TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_6 | GPIO_PIN_9, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium, GPIO_AF_I2C1);
	}
#endif
}

void TM_I2C2_INT_InitPins(TM_I2C_PinsPack_t pinspack) {
	/* Init pins */
#if defined(GPIOB)
	if (pinspack == TM_I2C_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_10 | GPIO_PIN_11, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium, GPIO_AF_I2C2);
	}
#endif
#if defined(GPIOF)
	if (pinspack == TM_I2C_PinsPack_2) {
		TM_GPIO_InitAlternate(GPIOF, GPIO_PIN_0 | GPIO_PIN_1, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium, GPIO_AF_I2C2);
	}
#endif
#if defined(GPIOH)
	if (pinspack == TM_I2C_PinsPack_3) {
		TM_GPIO_InitAlternate(GPIOH, GPIO_PIN_4 | GPIO_PIN_5, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium, GPIO_AF_I2C2);
	}
#endif

}

void TM_I2C3_INT_InitPins(TM_I2C_PinsPack_t pinspack) {
	/* Init pins */
#if defined(GPIOA) && defined(GPIOC)
	if (pinspack == TM_I2C_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_8, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium, GPIO_AF_I2C3);
		TM_GPIO_InitAlternate(GPIOC, GPIO_PIN_9, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium, GPIO_AF_I2C3);
	}
#endif
#if defined(GPIOH)
	if (pinspack == TM_I2C_PinsPack_2) {
		TM_GPIO_InitAlternate(GPIOH, GPIO_PIN_7 | GPIO_PIN_8, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium, GPIO_AF_I2C3);
	}
#endif

}
