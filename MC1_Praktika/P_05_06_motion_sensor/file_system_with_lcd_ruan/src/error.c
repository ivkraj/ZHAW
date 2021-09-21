/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * -- Project     : MC1 lab - Motion Sensor
 * -- Module      : This module contains the error handlers.
 * --
 * -- $Revision$
 * -- $Id$
 * ------------------------------------------------------------------
 */


#include "mc1_fs_board.h"
#include "stm32f4xx_hal.h"
#include "error.h"

#define BLINK_DELAY_MS 100u

/* -- module wide function declarations --------------------------------------*/
static void set_led(uint8_t led_mask);
static void halt_on_error(uint8_t error_code);

/* -- module wide variables definitions --------------------------------------*/

/* public function definitions ---------------------------------------------- */

/**
 * see header file
 */
void halt_on_spi_error(SPI_STATUS spi_status)
{
    if (spi_status != SPI_OK) {
        // clear all leds first
        set_led(0);
        
        switch (spi_status) {
            case SPI_INIT_ERROR:        // toggle led0
                halt_on_error(0x01);

            case SPI_WRITE_ERROR:       // toggle led1
                halt_on_error(0x02);

            default:                    // unknown error: toggle all leds
                halt_on_error(0x0F);
        }
    }
}


/**
 * see header file
 */
void halt_on_acc_error(ACC_STATUS acc_status)
{
    if (acc_status != ACC_OK) {
        // clear all leds first
        set_led(0);
        
        switch (acc_status) {
            case ACC_DMA_INIT_ERROR:    // toggle led2
                halt_on_error(0x04);

            case ACC_DMA_START_ERROR:   // toggle led0 & led2
                halt_on_error(0x05);
                
            case ACC_DMA_POLL_ERROR:    // toggle led1 & led2
                halt_on_error(0x06);

            default:                    // unknown error: toggle all leds
                halt_on_error(0x0F);
        }
    }
}


/**
 * see header file
 */
void halt_on_file_io_error(FILE_IO_STATUS file_io_status)
{
    if (file_io_status != FILE_IO_OK) {
        // clear all leds first
        set_led(0);
        
        switch (file_io_status) {
            case FILE_IO_MOUNT_ERROR:   // toggle led3
                halt_on_error(0x08);

            case FILE_IO_OPEN_ERROR:    // toggle led0 & led3
                halt_on_error(0x09);

            case FILE_IO_WRITE_ERROR:   // toggle led1 & led3
                halt_on_error(0x0A);

            case FILE_IO_CLOSE_ERROR:   // toggle led0, led1 & led3
                halt_on_error(0x0B);

            default:                    // unknown error: toggle all leds
                halt_on_error(0x0F);
        }
    }
}

/* private function definitions ---------------------------------------------- */

static void set_led(uint8_t led_mask)
{
    led_mask&0x01 ? SET_LED0_ON : SET_LED0_OFF;
    led_mask&0x02 ? SET_LED1_ON : SET_LED1_OFF;
    led_mask&0x04 ? SET_LED2_ON : SET_LED2_OFF;
    led_mask&0x08 ? SET_LED3_ON : SET_LED3_OFF;
}

static void halt_on_error(uint8_t error_code)
{
    while (1) {
        set_led(error_code);
        HAL_Delay(BLINK_DELAY_MS);
        set_led(0);
        HAL_Delay(BLINK_DELAY_MS);
    }
}



