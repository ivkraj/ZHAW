/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Module      : SPI Library
 * --
 * -- $Id: hal_spi.h 1942 2020-07-31 20:01:59Z ruan $
 * ------------------------------------------------------------------
 */

#ifndef _SPI_H
#define _SPI_H

#include <stdint.h>
#include "hal_common.h"


/**
 *  \enum   hal_spi_mode_t
 *  \brief  Controller mode.
 */
typedef enum {
    HAL_SPI_MODE_SLAVE = 0x0,       /** < Slave mode. */
    HAL_SPI_MODE_MASTER = 0x1       /** < Master mode. */
} hal_spi_mode_t;


/**
 *  \enum   hal_spi_dir_t
 *  \brief  Transfer direction.
 */
typedef enum {
    HAL_SPI_DIR_BOTH = 0x0,         /** < Read and write. */
    HAL_SPI_DIR_READ = 0x2,         /** < Read only. */
    HAL_SPI_DIR_WRITE = 0x3         /** < Write only. */
} hal_spi_dir_t;


/**
 *  \enum   hal_spi_size_t
 *  \brief  Size of the data frame.
 */
typedef enum {
    HAL_SPI_SIZE_8B = 0u,           /** < Data size 8 bit. */
    HAL_SPI_SIZE_16B = 1u           /** < Data size 16 bit. */
} hal_spi_size_t;


/**
 *  \enum   hal_spi_nss_t
 *  \brief  Control of slave select pin.
 */
typedef enum {
    HAL_SPI_NSS_HW = 0x0,           /** < Pin controled by hardware. */
    HAL_SPI_NSS_SW = 0x3,           /** < Pin controled by software. */
} hal_spi_nss_t;


/**
 *  \enum   hal_spi_prescaler_t
 *  \brief  Baude rate control.
 */
typedef enum {
    HAL_SPI_PRE_2 = 0u,             /** < APB clock (42 MHz) / 2. */
    HAL_SPI_PRE_4 = 1u,             /** < APB clock (42 MHz) / 4. */
    HAL_SPI_PRE_8 = 2u,             /** < APB clock (42 MHz) / 8. */
    HAL_SPI_PRE_16 = 3u,            /** < APB clock (42 MHz) / 16. */
    HAL_SPI_PRE_32 = 4u,            /** < APB clock (42 MHz) / 32. */
    HAL_SPI_PRE_64 = 5u,            /** < APB clock (42 MHz) / 64. */
    HAL_SPI_PRE_128 = 6u,           /** < APB clock (42 MHz) / 128. */
    HAL_SPI_PRE_256 = 7u            /** < APB clock (42 MHz) / 256. */
} hal_spi_prescaler_t;


/**
 *  \enum   hal_spi_frame_t
 *  \brief  Defines SPI frame format.
 */
typedef enum {
    HAL_SPI_FRAME_MOTOROLLA = 0x0,      /** < Motorolla type format. */
    HAL_SPI_FRAME_TI = 0x1              /** < Texas Instruments type format. */
} hal_spi_frame_t;


/**
 *  \struct hal_spi_init_t
 *  \brief  Initialization structure for the serial peripheral interface.
 */
typedef struct {
    hal_spi_mode_t mode;                /**< Master or slave mode. */
    hal_spi_dir_t direction;            /**< Transmit direction. */
    hal_spi_size_t data_size;           /**< Size of packet. */
    hal_spi_nss_t nss_mode;             /**< NSS set by sw or hw. */
    hal_spi_prescaler_t prescaler;      /**< Baudrate prescaler. */
    hal_spi_frame_t frame_type;         /**< Motorolla or TI. */
    hal_bool_t hw_crc;                  /**< ENABLE/DISABLE CRC calculation. */
} hal_spi_init_t;


/**
 * \brief Initialize SPI1 Interface
 * \param *spi: Pointer to the Peripheral adress
 * \param init: Defines the properties of the peripheral
 */
void hal_spi_init(reg_spi_t *spi, hal_spi_init_t init);


/**
 * \brief Read/Write data via SPI1 on Port P5
 *
 * PINs:
 * - CS:    PA4
 * - SCK:   PA5
 * - MISO:  PA6
 * - MOSI:  PA7
 *
 * \param uint8_t nr_of_bytes: Total number of Bytes which have to be transfered
 *        Example: nr_of_bytes = send 1 Bytes + receive 3 Bytes = 4 Bytes
 * \param uint8_t *tx_buffer:  Pointer to an array which
 *        contain the data to be sent
 * \param uint8_t *rx_buffer:  Pointer to an array on which
 *        the received bytes will be stored
 */
void hal_spi_read_write_bytes(uint16_t nr_of_bytes, uint8_t *tx_buffer,
                              uint8_t *rx_buffer, reg_spi_t *spi);

/**
 * \brief Set a read and write request of SPI to the DMA
 */
void set_spi_read_write_request(reg_spi_t *spi);

/**
 * \brief Clear the read and write request of SPI to the DMA
 */
void clear_spi_read_write_request(reg_spi_t *spi);
#endif    /* _SPI_H */
