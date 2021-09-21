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
 * -- $Revision: 2067 $
 * -- $Id: file_io.c 2067 2020-10-05 11:23:44Z ruan $
 * --------------------------------------------------------------- */

#include "file_io.h"
#include "ff.h"
#include "ff_gen_drv.h" // FATFS_LinkDriver
#include "sd_diskio.h"  // defines SD_Driver as external */
#include <string.h>
#include <stdio.h>

static char SDPath[4];   /* SD logical drive path */
static FATFS SDFatFS;    /* File system object for SD logical drive */
static const char avg_path[] = "average_values.csv\0";
static const char raw_path[] = "raw_values.csv\0";

/**
 * see header file
 */
void MX_FATFS_Init(void)
{
    /* Link the SD driver */
    //retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
    FATFS_LinkDriver(&SD_Driver, SDPath);
}

/**
 * see header file
 */
FILE_IO_STATUS file_io_register_filesystem(void)
{
    /* Exercise 4.1: Register filesystem object to the fatfs module */
    /* Use the predefined module-wide variables SDFatFS and SDPath */
    /// STUDENTS: To be programmed
    
    if (f_mount(&SDFatFS, SDPath, 1) != FR_OK) {
        return FILE_IO_MOUNT_ERROR;
    } else {
        return FILE_IO_OK;
    }
    
    /// END: To be programmed
    /* End of Exercise 4.1 */
}

/**
 * see header file
 */
FILE_IO_STATUS file_io_unregister_filesystem(void)
{
    /* Exercise 4.1: Unregister filesystem object to the fatfs module */
    /// STUDENTS: To be programmed
    
    if (f_mount(NULL, SDPath, 1) != FR_OK) {
        return FILE_IO_MOUNT_ERROR;
    } else {
        return FILE_IO_OK;
    }


    /// END: To be programmed
    /* End of Exercise 4.1 */
}

/**
 * see header file
 */
FILE_IO_STATUS file_io_create(FILE_IO_FILE_TYPE type)
{
    /* Exercise 4.2: Create file object and write header */
    
    /* Use the following steps
     *      - f_open() with predefined paths -->  error handling
     *      - write header to file with f_write() --> error handling
     *      - close file with f_close()  --> error handling
     */
    
    /// STUDENTS: To be programmed
    FIL file; 
    FRESULT fr;
    UINT bw;
    static const char buff[] = "Average Sample Nr.;Acceleration X;Acceleration Y;Acceleration Z;\0";
    
    if (type == FILE_IO_AVERAGE) {
        fr = f_open(&file, avg_path, FA_CREATE_ALWAYS | FA_WRITE);
    } else {
        fr = f_open(&file, raw_path, FA_CREATE_ALWAYS | FA_WRITE);
    }
    
    if (fr == FR_OK) {
        fr = f_write(&file, buff, sizeof(buff)/sizeof(buff[0]), &bw);
        if (fr == FR_OK) {
            fr = f_close(&file);
            if (fr == FR_OK) {
                return FILE_IO_OK;
            } else {
                return FILE_IO_CLOSE_ERROR;
            }
        } else {
            return FILE_IO_WRITE_ERROR;
        }
    } else {
        return FILE_IO_OPEN_ERROR;
    }  

    /// END: To be programmed
    /* End of Exercise 4.2 */
}

/**
 * see header file
 */
FILE_IO_STATUS file_io_write_avg_to_sd_card(int16_t *acceleration)
{
    /* Exercise 4.3: Write average acceleration */
    
    /* Use the following steps (all with appropriate error handling)
     *      - open file
     *      - append line to file containing average acceleration
     *      - close file
     */
    
    /// STUDENTS: To be programmed
    
    FIL file; 
    FRESULT fr;
    UINT bw;
    static uint32_t sample_nr = 1;
    static char buff[100];

    fr = f_open(&file, avg_path, FA_OPEN_APPEND | FA_WRITE);
    
    if (fr == FR_OK) {
        snprintf(buff, 100, "\n%u;%i;%i;%i", sample_nr, acceleration[0], acceleration[1], acceleration[2]);
        fr = f_write(&file, buff, strlen(buff), &bw);
        sample_nr++;
        if (fr == FR_OK) {
            fr = f_close(&file);
            if (fr == FR_OK) {
                return FILE_IO_OK;
            } else {
                return FILE_IO_CLOSE_ERROR;
            }
        } else {
            return FILE_IO_WRITE_ERROR;
        }
    } else {
        return FILE_IO_OPEN_ERROR;
    }


    /// END: To be programmed
    /* End of Exercise 4.3 */
}

/**
 * see header file
 */
FILE_IO_STATUS file_io_write_raw_to_sd_card(int16_t *acceleration,
                                            uint16_t nr_of_samples)
{
    /* Exercise 4.4: Write raw acceleration*/
    
    /* Use the following steps (all with appropriate error handling)
     *      - open file
     *      - a for loop (based on nr_of_samples) appending lines with the raw
     *        acceleration values to the file
     *      - close file
     */
    
    /// STUDENTS: To be programmed
    
    FIL file; 
    FRESULT fr;
    UINT bw;
    static uint32_t sample_nr = 1;
    static char buff[100];

    fr = f_open(&file, raw_path, FA_OPEN_APPEND | FA_WRITE);
    
    if (fr == FR_OK) {
        for(uint16_t i = 0; i < nr_of_samples; i++){
            snprintf(buff, 100, "\n%u;%i;%i;%i", sample_nr, acceleration[3*i], acceleration[3*i+1], acceleration[3*i+2]);
            fr = f_write(&file, buff, strlen(buff), &bw);
            sample_nr++;
            if(fr != FR_OK){
                return FILE_IO_WRITE_ERROR;
            }
        }
        if (fr == FR_OK) {
            fr = f_close(&file);
            if (fr == FR_OK) {
                return FILE_IO_OK;
            } else {
                return FILE_IO_CLOSE_ERROR;
            }
        } else {
            return FILE_IO_WRITE_ERROR;
        }
    } else {
        return FILE_IO_OPEN_ERROR;
    }



    /// END: To be programmed
    /* End of Exercise 4.4 */
}
