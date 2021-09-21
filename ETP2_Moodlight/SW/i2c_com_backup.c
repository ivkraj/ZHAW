
#include "em_i2c.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_device.h"
#include <stdbool.h>

#define I2C_RXBUFFER                 10

// Buffers++
//uint8_t i2c_txBuffer[] = "Gecko";
//uint8_t i2c_txBufferSize = sizeof(i2c_txBuffer);
//uint8_t i2c_rxBuffer[I2C_RXBUFFER];
//uint8_t i2c_rxBufferIndex;


I2C_TypeDef i2c;
I2C_TransferReturn_TypeDef I2C_Status;

// Transmission flags
//volatile bool i2c_rxInProgress;
//volatile bool i2c_startTx;


void I2C_init(void) {

	CMU_ClockEnable(cmuClock_I2C0, true);

	 /* Using PD6 (SDA) and PD7 (SCL) */
	GPIO_PinModeSet(gpioPortD, 7, gpioModeWiredAnd, 1);
	GPIO_PinModeSet(gpioPortD, 6, gpioModeWiredAnd, 1);

	I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
	//i2cInit.refFreq = 1000;

	/* Enable pins at location 1 */
	I2C0->ROUTE = I2C_ROUTE_SDAPEN |I2C_ROUTE_SCLPEN |(1 << _I2C_ROUTE_LOCATION_SHIFT);

	  /* Initializing the I2C */
	I2C_Init(I2C0, &i2cInit);

	NVIC_ClearPendingIRQ(I2C0_IRQn);
	NVIC_EnableIRQ(I2C0_IRQn);

	//I2C0->CTRL |= I2C_CTRL_SLAVE | I2C_CTRL_AUTOACK | I2C_CTRL_AUTOSN;

}

/**************************************************************************//**
 * @brief I2C Interrupt Handler.
 *        The interrupt table is in assembly startup file startup_efm32.s
 *****************************************************************************/
void I2C0_IRQHandler(void) {

	NVIC_ClearPendingIRQ(I2C0_IRQn);
	I2C_Status = I2C_Transfer(I2C0);
}


uint16_t I2C_SetReg(uint16_t I2C_ADDR, uint8_t *I2C_REG, uint16_t I2C_RXBUFFER_SIZE) {
  /* Transfer structure */
  I2C_TransferSeq_TypeDef i2cTransfer;

  /* Setting pin to indicate transfer */
  GPIO_PinOutSet(gpioPortC, 0);

  /* Initializing I2C transfer */
  i2cTransfer.addr          = I2C_ADDR;
  i2cTransfer.flags         = I2C_FLAG_WRITE_READ;
  i2cTransfer.buf[0].data   = I2C_REG;
  i2cTransfer.buf[0].len    = 8;
  //i2cTransfer.buf[1].data   = i2c_rxBuffer;
  i2cTransfer.buf[1].len    = I2C_RXBUFFER_SIZE;

  I2C_TransferInit(I2C0, &i2cTransfer);

  /* Sending data */
  while (I2C_Transfer(I2C0) == i2cTransferInProgress){;}

  /* Clearing pin to indicate end of transfer */
  //GPIO_PinOutClear(gpioPortC, 0);
  //enableI2cSlaveInterrupts();

  return *i2cTransfer.buf[1].data;
}

uint8_t * I2C_ReceiveData(uint16_t I2C_ADDR) {
  /* Transfer structure */
  I2C_TransferSeq_TypeDef i2cTransfer;
  static uint8_t i2c_rxBuffer[2];


  /* Setting pin to indicate transfer */
  GPIO_PinOutSet(gpioPortC, 0);

  /* Initializing I2C transfer */
  i2cTransfer.addr          = (I2C_ADDR)<<1;
  i2cTransfer.flags         = I2C_FLAG_WRITE_READ;
  i2cTransfer.buf[0].data   = 0x00;
  i2cTransfer.buf[0].len    = 1;
  i2cTransfer.buf[1].data   = i2c_rxBuffer;
  i2cTransfer.buf[1].len    = 2;

  I2C_TransferInit(I2C0, &i2cTransfer);

  /* Sending data */
  while (I2C_Transfer(I2C0) == i2cTransferInProgress){;}

  return i2c_rxBuffer;
}

uint16_t I2C_GetTemp(void) {
	uint8_t TEMP_ADDR = 0x48;
	uint8_t *val = I2C_ReceiveData(TEMP_ADDR);
	return val[0];
}

void TEMPSENS_RegisterSet () {

}

