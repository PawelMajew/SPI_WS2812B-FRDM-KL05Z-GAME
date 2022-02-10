#ifndef I2C_H
#define I2C_H

#include "MKL05Z4.h"
#define DELAY(x)   for(uint32_t ii=0;ii<(x*10000);ii++)__NOP(); 					/* wait */
/**
 * @brief I2C initialization.
 */
void I2C_Init(void);
/**
 * @brief I2C write operation.
 *
 * @param Address.
 * @param Data.
 */
uint8_t I2C_Write(uint8_t address, uint8_t data);
	
#endif /* I2C_H */
