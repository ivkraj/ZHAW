/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Interface of module uart.
 * 
 *  $Id: uart.h 1124 2016-12-06 07:29:06Z feur $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _UART_H
#define _UART_H


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Initialize tasks.
 */
void uart_init(void);


/**
 *  \brief  Output char on UART.
 *  \param  ch : character to send to UART.
 */
int stdout_putchar(int ch);
    
#endif

