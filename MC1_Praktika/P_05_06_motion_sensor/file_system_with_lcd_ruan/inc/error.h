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
 * -- Description : This module contains the error handlers.
 * -- 
 * -- $Revision$
 * -- $Id$
 * --------------------------------------------------------------- */
 
#ifndef __error_H
#define __error_H

#include <stdint.h>
#include "file_io.h"
#include "spi.h"
#include "accelerometer.h"

/**
 *  \brief  Halt and display SPI error
 *  
 *  Halts the programm with an endless loop and displays the error
 *  on the LEDs if the status is not SPI_OK.
 * 
 *  SPI_STATUS      | Binary Error Code
 *  ----------------|------------------
 *  SPI_INIT_ERROR  | 0001
 *  SPI_WRITE_ERROR | 0010
 *
 *  \param spi_status:  Status code to be checked and displayed
 *                      if not SPI_OK.
 */
void halt_on_spi_error(SPI_STATUS spi_status);

/**
 *  \brief  Halt and display accelerometer error
 *  
 *  Halts the programm with an endless loop and displays the error
 *  on the LEDs if the status is not ACC_OK.
 * 
 *  ACC_STATUS          | Binary Error Code
 *  --------------------|------------------
 *  ACC_DMA_INIT_ERROR  | 0100
 *  ACC_DMA_START_ERROR | 0101
 *  ACC_DMA_POLL_ERROR  | 0110
 *
 *  \param acc_status:  Status code to be checked and displayed
 *                      if not ACC_OK.
 */
void halt_on_acc_error(ACC_STATUS acc_status);

/**
 *  \brief  Halt and display file io error
 *  
 *  Halts the programm with an endless loop and displays the error
 *  on the LEDs if the status is not FILE_IO_OK.
 * 
 *  FILE_IO_STATUS      | Binary Error Code
 *  --------------------|--------------------
 *  FILE_IO_MOUNT_ERROR | 1000
 *  FILE_IO_OPEN_ERROR  | 1001
 *  FILE_IO_WRITE_ERROR | 1010
 *  FILE_IO_CLOSE_ERROR | 1011
 *
 *
 *  \param file_io_status:  Status code to be checked and displayed
 *                          if not FILE_IO_OK.
 */
void halt_on_file_io_error(FILE_IO_STATUS file_io_status);
 


#endif /* __error_H */

