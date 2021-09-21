/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * -- Project     : MC1 lab - Motion Sensor
 * -- Module      : This modul handles the SPI communication to the sensor
 * --
 * -- $Id: spi.c 2067 2020-10-05 11:23:44Z ruan $
 * ------------------------------------------------------------------
 */

#include "spi.h"
// :TODO: remove the large unnecessary includes due to hal_conf.h
#include "stm32f4xx_hal.h"

#define TIMEOUT_VAL_SPI 50u

/* -- module wide function declarations --------------------------------------*/
static void set_ss_pin_low(void);
static void set_ss_pin_high(void);

/* -- module wide variables definitions --------------------------------------*/
static SPI_HandleTypeDef hspi;

/* public function definitions ---------------------------------------------- */

/**
 * see header file
 */
SPI_STATUS MX_SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    
    __HAL_RCC_SPI4_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    /**SPI4 GPIO Configuration
           PE11     ------> SPI4_NSS
           PE12     ------> SPI4_SCK
           PE13     ------> SPI4_MISO
           PE14     ------> SPI4_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);


    hspi.Instance = SPI4;

    hspi.Init.Mode = SPI_MODE_MASTER;
    hspi.Init.Direction = SPI_DIRECTION_2LINES;
    hspi.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi.Init.NSS = SPI_NSS_SOFT;
    hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16; // Baudrate = 1.5MHz
    hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi) != HAL_OK) {
        return SPI_INIT_ERROR;
    }
    
    return SPI_OK;
}

/**
 *  See header File
 */
SPI_STATUS hal_acc_spi_read_write(uint16_t nr_of_bytes, uint8_t *tx_buffer,
                            uint8_t *rx_buffer)
{
    /* Use the following items to implement this function
            - function HAL_SPI_TransmitReceive() from module stm32f4xx_hal_spi
            - the module-wide SPI handle (variable) defined in this file
            - the macro TIMEOUT_VAL_SPI defined in this file
            - the functions set_ss_pin_low() and set_ss_pin_high() defined in 
              this file
     */
    /// STUDENTS: To be programmed
    HAL_StatusTypeDef hal_status;
    SPI_STATUS spi_status;
    
    set_ss_pin_low();
    hal_status = HAL_SPI_TransmitReceive(&hspi, tx_buffer, rx_buffer, nr_of_bytes, TIMEOUT_VAL_SPI);
    set_ss_pin_high();
    
    switch(hal_status){
        case HAL_OK:
            spi_status = SPI_OK;
            break;
        default:
            spi_status = SPI_WRITE_ERROR;
            break;
    }
    return spi_status;

    /// END: To be programmed
}

/* internal function definitions -------------------------------------------- */

/**
 * \brief Set Slave-Select Pin (Port B, Pin 12) low
 */
static void set_ss_pin_low(void)
{
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
}

/**
 * \brief Set Slave-Select Pin (Port B, Pin 12) high
 */
static void set_ss_pin_high(void)
{
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
}
