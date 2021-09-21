/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module threads_uart.
 * 
 *  $Id: threads_uart.c 1319 2017-12-08 10:20:00Z kjaz $
 * ------------------------------------------------------------------------- */


/* Standard includes */ 
#include <cmsis_os.h>
#include <stdio.h>

/* User includes */
#include "threads.h"
#include "uart.h"

/* -- Macros
 * ------------------------------------------------------------------------- */

#define MUTEX_ENABLE        (1)
#define HALF_SECOND         (0x7fffff)


/* -- Variables with global scope
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed

osThreadId id_writeUart1, id_writeUart2;
osMutexId id_uart;
osMutexDef_t m_uart;

/// END: To be programmed


/* -- Variables with module wide scope
 * ------------------------------------------------------------------------- */

static uint32_t count = 0u;


/* -- Local function declarations
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed

void write_uart1 (void const *argument);
void write_uart2 (void const *argument);


/// END: To be programmed
static void wait_blocking(uint32_t value);


/* -- Thread / mutex definition
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed

osThreadDef(write_uart1, osPriorityNormal, 1, 0);
osThreadDef(write_uart2, osPriorityNormal, 1, 0);
osMutexDef(m_uart);


/// END: To be programmed


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void threads_init(void)
{
    uart_init();
    
    /// STUDENTS: To be programmed    
    
    id_writeUart1 = osThreadCreate(osThread(write_uart1), NULL);
    id_writeUart2 = osThreadCreate(osThread(write_uart2), NULL);
    id_uart = osMutexCreate(osMutex (m_uart));
    if (id_writeUart1 == NULL) {
        printf("Creation of Thread write_uart1 was not successful\n\r");
    } else {
        printf("Creation of Thread write_uart1 was successful\n\r");
    }

    if (id_writeUart2 == NULL) {
        printf("Creation of Thread write_uart2 was not successful\n\r");
    } else {
        printf("Creation of Thread write_uart2 was successful\n\r");
    }
    
    if (id_uart == NULL) {
        printf("Creation of Mutex uart was not successful\n\r");
    } else {
        printf("Creation of Mutex uart was successful\n\r");
    }     

    /// END: To be programmed
}


/* -- Local function definitions
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed

void write_uart1 (void const *argument) {
    while (1) {
        osStatus status;
        status = osMutexWait(id_uart, osWaitForever);
        if (status == osOK) {
            count++;
            printf("Counter increased by Thread1: %d\n\r", count);
            wait_blocking(HALF_SECOND);
            osMutexRelease(id_uart);            
        }   
    }
}

void write_uart2 (void const *argument) {
    while (1) {
        osStatus status;
        status = osMutexWait(id_uart, osWaitForever);
        if (status == osOK) {
            count++;
            printf("Counter increased by Thread2: %d\n\r", count);
            wait_blocking(HALF_SECOND);
            osMutexRelease(id_uart);            
        }
    }
}

/// END: To be programmed

/**
 *  \brief  Wait loop.
 *  \param  value : Iterations to wait. Blocks execution of other threads.
 */
static void wait_blocking(uint32_t value)
{
    for (; value > 0u; value--);
}

