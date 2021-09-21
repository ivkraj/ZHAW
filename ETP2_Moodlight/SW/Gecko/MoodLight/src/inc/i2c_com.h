/** ***************************************************************************
 * @file
 * @brief See i2c_com.c
 *****************************************************************************/

#ifndef I2C_COM_H_
#define I2C_COM_H_

#include "em_chip.h"


/******************************************************************************
 * Functions
 *****************************************************************************/

void I2C_init(void);
void I2C_ReceiveData(uint16_t, uint16_t, uint8_t *);
void I2C_WriteWriteData(uint16_t, uint8_t *, uint8_t *);
void I2C_WriteData(uint16_t , uint16_t , uint8_t *);
void I2C_WriteReadData(uint16_t , uint8_t *, uint8_t *);

#endif
