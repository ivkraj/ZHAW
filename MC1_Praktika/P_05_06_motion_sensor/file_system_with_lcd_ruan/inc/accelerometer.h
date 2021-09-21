/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 lab - Motion Sensor
 * -- Description : This modul handles the commands of the Accelerometer
 *                  of the Type LSM6DSL. The access to the peripherals is
 *                  executed trough the file hal_acc.h.
 * -- $Revision: 2067 $
 * -- $Id: accelerometer.h 2067 2020-10-05 11:23:44Z ruan $
 * --------------------------------------------------------------- */

#ifndef _ACCELEROMETER
#define _ACCELEROMETER

#include <stdint.h>

/**
 *  \brief  defines the error codes for the accelerometer module
 *  \enum   mode
 */
typedef enum {
    ACC_OK = 0,
    ACC_DMA_INIT_ERROR,
    ACC_DMA_START_ERROR,
    ACC_DMA_POLL_ERROR
} ACC_STATUS;

/**
 *  \brief  defines modes: DMA, continous and FIFO
 *  \enum   mode
 */
typedef enum {
    SPI_SINGLE,              /* SPI-Polling without extra sensor-functions */
    SPI_DMA,                 /* SPI-DMA without extra sensor-functions     */
    SPI_SINGLE_LSM_FIFO,     /* SPI-Polling with FIFO mode on sensor       */
    SPI_DMA_LSM_FIFO         /* SPI-DMA with FIFO mode on sensor           */
} mode_t;

/**
 *  \brief  Setup SPI, GPIOs and DMA for the Accelerometer LSM6DSL,all registers
 *          are initialized and a measurement is started
 *          The sensor is initialized with the following settings
 *          (values in lsm6dsl_reg.h):
 *          Measurement:                Acceleration
 *          Range:                      +/- 4 G
 *          Measure Freq(ODR):          208 Hz
 *          Interupt 1:                 Acceleration data ready
 *
 *  \param  mode: Indicates the uses of FIFO and DMA
 */
void accelerometer_init(mode_t mode);

/**
 *  \brief  Reads the Acceleration Data of the LSM6DSL.
 *          Determines automatically (depending on parameter transmission_mode)
 *          if dma or continous mode is necessary. can be used for both.
 *  \param  acceleration: pointer to an array with space for the whole fifo
            fifo-size is defined in FIFO_CTRL1 and FIFO_CTRL2 register
 *  \param  mode: Indicates the uses of FIFO and DMA
 *  \return number of samples read by the function and stored in the array
 */
uint16_t accelerometer_read_acceleration(int16_t *acceleration, mode_t mode);

/**
 *  \brief  Reads the id of accelerometer with SPI
 *  \return "who am i" - ID of the Sensor
 */
uint8_t accelerometer_who_am_i(void);

#endif
