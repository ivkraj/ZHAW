
#include "em_i2c.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_device.h"
#include <stdbool.h>

I2C_TypeDef i2c;
I2C_TransferReturn_TypeDef I2C_Status;

/**************************************************************************//**
 * @brief I2C Interrupt Handler.
 *        The interrupt table is in assembly startup file startup_efm32.s
 *****************************************************************************/
void I2C0_IRQHandler(void) {

	NVIC_ClearPendingIRQ(I2C0_IRQn);
	I2C_Status = I2C_Transfer(I2C0);
}

void I2C_WriteReadData(uint16_t I2C_ADDR, uint8_t I2C_txBuffer[], uint8_t I2C_rxBuffer[]) {
   //Transfer structure
  I2C_TransferSeq_TypeDef i2cTransfer;

   //Setting pin to indicate transfer
  GPIO_PinOutSet(gpioPortC, 0);

   //Initializing I2C transfer
  i2cTransfer.addr          = (I2C_ADDR) << 1;
  i2cTransfer.flags         = I2C_FLAG_WRITE_READ;
  i2cTransfer.buf[0].data   = I2C_txBuffer;
  i2cTransfer.buf[0].len    = 1;
  i2cTransfer.buf[1].data   = I2C_rxBuffer;
  i2cTransfer.buf[1].len    = 2;

  I2C_TransferInit(I2C0, &i2cTransfer);

   //Sending data
  while (I2C_Transfer(I2C0) == i2cTransferInProgress){;}
}

void I2C_WriteData(uint16_t I2C_ADDR, uint16_t I2C_DataLength, uint8_t I2C_txBuffer[]) {
  /* Transfer structure */
  I2C_TransferSeq_TypeDef i2cTransfer;

  /* Setting pin to indicate transfer */
  GPIO_PinOutSet(gpioPortC, 0);

  /* Initializing I2C transfer */
  i2cTransfer.addr          = (I2C_ADDR) << 1;
  i2cTransfer.flags         = I2C_FLAG_WRITE;
  i2cTransfer.buf[0].data   = I2C_txBuffer;
  i2cTransfer.buf[0].len    = I2C_DataLength;

  I2C_TransferInit(I2C0, &i2cTransfer);

  /* Sending data */
  while (I2C_Transfer(I2C0) == i2cTransferInProgress){;}
}

void I2C_WriteWriteData(uint16_t I2C_ADDR, uint8_t i2c_txRegBuffer[], uint8_t i2c_txDataBuffer[]) {
  /* Transfer structure */
  I2C_TransferSeq_TypeDef i2cTransfer;

  /* Setting pin to indicate transfer */
  GPIO_PinOutSet(gpioPortC, 0);

  /* Initializing I2C transfer */
  i2cTransfer.addr          = (I2C_ADDR) << 1;
  i2cTransfer.flags         = I2C_FLAG_WRITE_WRITE;
  i2cTransfer.buf[0].data   = i2c_txRegBuffer;
  i2cTransfer.buf[0].len    = 1;
  i2cTransfer.buf[1].data   = i2c_txDataBuffer;
  i2cTransfer.buf[1].len    = 2;

  I2C_TransferInit(I2C0, &i2cTransfer);

  /* Sending data */
  while (I2C_Transfer(I2C0) == i2cTransferInProgress){;}
}

void I2C_ReceiveData(uint16_t I2C_ADDR, uint16_t I2C_DataLength, uint8_t i2c_rxBuffer[]) {
  /* Transfer structure */
  I2C_TransferSeq_TypeDef i2cTransfer;

  /* Setting pin to indicate transfer */
  GPIO_PinOutSet(gpioPortC, 0);

  /* Initializing I2C transfer */
  i2cTransfer.addr          = (I2C_ADDR) << 1;
  i2cTransfer.flags         = I2C_FLAG_READ;
  i2cTransfer.buf[0].data   = i2c_rxBuffer;
  i2cTransfer.buf[0].len    = I2C_DataLength;

  I2C_TransferInit(I2C0, &i2cTransfer);

  /* Sending data */
  while (I2C_Transfer(I2C0) == i2cTransferInProgress){;}
}

void I2C_init(void) {
	CMU_ClockEnable(cmuClock_I2C0, true);

	 /* Using PD6 (SDA) and PD7 (SCL) */
	GPIO_PinModeSet(gpioPortD, 7, gpioModeWiredAnd, 1);
	GPIO_PinModeSet(gpioPortD, 6, gpioModeWiredAnd, 1);

	I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

	/* Enable pins at location 1 */
	I2C0->ROUTE = I2C_ROUTE_SDAPEN |I2C_ROUTE_SCLPEN |(1 << _I2C_ROUTE_LOCATION_SHIFT);

	  /* Initializing the I2C */
	I2C_Init(I2C0, &i2cInit);

	NVIC_ClearPendingIRQ(I2C0_IRQn);
	NVIC_EnableIRQ(I2C0_IRQn);

	//I2C0->CTRL |= I2C_CTRL_SLAVE | I2C_CTRL_AUTOACK | I2C_CTRL_AUTOSN;
}



