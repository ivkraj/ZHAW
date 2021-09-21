/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Interface of module buttons.
 *
 *  $Id: buttons.h 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */


/* Re-definition guard */
#ifndef _BUTTONS_H
#define _BUTTONS_H


/* Standard includes */
#include "stdint.h"


/* -- Type definitions
 * ------------------------------------------------------------------------- */

/* Enumerate actions */
typedef enum {
    T0_PRESSED = 0x01,
    T1_PRESSED = 0x02,
    T2_PRESSED = 0x03,
    T3_PRESSED = 0x04
} buttons_actions_t;


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Starts the callback
 */
void buttons_init(void);

/**
 *  \brief  Register a callback function that will be called in case of a
 *          button press
 *  \param  func_ptr    Pointer to the callback function that will be registered
 *  \return 0 for success / 1 in case of error (out of space)
 */
uint32_t buttons_register_observer(void (*func_ptr)(uint32_t data));

/**
 *  \brief  Checks buttons T3-T0 for events; Function is called by scheduler
 */
void buttons_check_pressed(void);


#endif
