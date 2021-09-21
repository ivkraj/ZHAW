/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 lab - Motion sensor
 * -- Description : This module handles the io operations on the file
 * --               system. It creates files on the SD card and writes
 * --               the data to it and reads from it.
 * --
 * -- $Revision: 1997 $
 * -- $Id: file_io.h 1997 2020-08-12 06:18:56Z ruan $
 * --------------------------------------------------------------- */
 
#ifndef __FILE_IO_H
#define __FILE_IO_H

#include <stdint.h>

/* defines the error codes on file handling */
typedef enum {
    FILE_IO_OK = 0,
    FILE_IO_MOUNT_ERROR,
    FILE_IO_OPEN_ERROR,
    FILE_IO_WRITE_ERROR,
    FILE_IO_CLOSE_ERROR
} FILE_IO_STATUS;

typedef enum {
    FILE_IO_AVERAGE = 0,
    FILE_IO_RAW
} FILE_IO_FILE_TYPE;

/**
 *  \brief  register filesystem object to the fatfs module
 */
FILE_IO_STATUS file_io_register_filesystem(void);

/**
 *  \brief  unregister filesystem object to the fatfs module
 */
FILE_IO_STATUS file_io_unregister_filesystem(void);

/**
 *  \brief  create file and write header to it
 *  \param  specifies which file shall be generated: avg. vs raw
 */
FILE_IO_STATUS file_io_create(FILE_IO_FILE_TYPE type);

/**
 *  \brief  write avg. acceleration data to sd card with fatfs
 *          Append the values at end of file
 *  \param  acceleration: pointer to an array with a single avg. acceleration 
 *          value (x,y,z)
 */
FILE_IO_STATUS file_io_write_avg_to_sd_card(int16_t *acceleration);

/**
 *  \brief  write raw acceleration data to sd card with fatfs
 *          Append the values at end of file
 *  \param  acceleration: pointer to an array containing multiple samples 
 *          (x,y,z) of acceleration data
 *  \param  nr_of_samples: number of samples in array
 */
FILE_IO_STATUS file_io_write_raw_to_sd_card(int16_t *acceleration,
                                            uint16_t nr_of_samples);

/**
 *  \brief  init filesystem
 */
void MX_FATFS_Init(void);

#endif
