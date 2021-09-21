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
 * -- Description : This modul handles the SPI communication to the sensor
 * -- 
 * -- $Revision: 2067 $
 * -- $Id: spi.h 2067 2020-10-05 11:23:44Z ruan $
 * --------------------------------------------------------------- */
 
#ifndef __spi_H
#define __spi_H

#include <stdint.h>

/* defines the error codes for the spi module */
typedef enum {
    SPI_OK = 0,
    SPI_INIT_ERROR,
    SPI_WRITE_ERROR //only concerns non-DMA read/writes
} SPI_STATUS;

/**
 *  \brief  initialize SPI2
 *  \return SPI status code
 */
SPI_STATUS MX_SPI_Init(void);
    
 
/**
 * \brief Read/Write via SPI4
 *
 * SPI4 GPIO Configuration
 *          PE11     ------> SPI4_NSS
 *          PE12     ------> SPI4_SCK
 *          PE13     ------> SPI4_MISO
 *          PE14     ------> SPI4_MOSI
 *
 * \param nr_of_bytes: Total number of Bytes which have to be transfered
 *                     Example: nr_of_bytes = 
 *                                send 1 Bytes + receive 3 Bytes = 4 Bytes
 *
 * \param *tx_buffer:  Pointer to an array which contains the data  to be sent
 *        
 * \param *rx_buffer:  Pointer to an array in which the received bytes will be 
 *                     stored
 *
 * \return SPI status code
 */
SPI_STATUS hal_acc_spi_read_write(uint16_t nr_of_bytes, uint8_t *tx_buffer,
                            uint8_t *rx_buffer);

#endif /*__ spi_H */

