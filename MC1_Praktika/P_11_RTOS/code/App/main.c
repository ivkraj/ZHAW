/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Main program
 *
 *  $Id: main.c 1125 2016-12-06 09:45:53Z feur $
 * ------------------------------------------------------------------------- */


/* Standard includes */
#include <cmsis_os.h>

/* User includes */
#include "threads.h"


/* -- M A I N
 * ------------------------------------------------------------------------- */
 
int main() 
{
    
    osKernelInitialize();                  /* Initialize CMSIS-RTOS.         */
    threads_init();                        /* Create threads.                */
    osKernelStart();                       /* Start thread execution.        */

    while(1);
}
