/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 lab - energy efficient sensors
 * -- Description : This module handles the io operations on the file
 * --               system. It creates files on the SD card and writes
 * --               the data to it and reads from it.
 * --
 * -- $Revision: 1950 $
 * -- $Id: accelerometer.c 1950 2020-08-04 08:50:37Z ruan $
 * --------------------------------------------------------------- */
 
/* defines the error codes on file handling */
typedef enum {
    FILE_IO_OK = 0,
    FILE_IO_MOUNT_ERROR,
    FILE_IO_OPEN_ERROR,
    FILE_IO_WRITE_ERROR,
    FILE_IO_CLOSE_ERROR
} FILE_IO_STATUS;

FILE_IO_STATUS file_io_register_filesystem(void);

FILE_IO_STATUS file_io_create_file(void);

FILE_IO_STATUS file_io_writeline(const char* line);
