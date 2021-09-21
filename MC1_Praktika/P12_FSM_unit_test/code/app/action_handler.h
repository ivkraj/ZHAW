/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Interface of module action_handler.
 * --
 * -- Sets or clears pins on outport.
 * --
 * -- $Id: action_handler.h 2690 2015-11-18 15:37:30Z fert $
 * ------------------------------------------------------------------------- */

/* re-definition guard */
#ifndef _ACTION_HANDLER_H
#define _ACTION_HANDLER_H

/* standard includes */
#include <stdint.h>


/* -- Type definitions
 * ------------------------------------------------------------------------- */

typedef enum {
    SIGNAL_CAR_W = 0u,
    SIGNAL_CAR_S = 2u,
    SIGNAL_CAR_E1 = 4u,
    SIGNAL_CAR_E2 = 6u,
    SIGNAL_PED_W = 8u,
    SIGNAL_PED_E = 10u
} signal_t;


typedef enum {
    COLOR_NO = 0x0,
    COLOR_RED = 0x1,
    COLOR_YELLOW = 0x2,
    COLOR_GREEN = 0x3
} color_t;


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/*
 * Initializes the outport.
 */
void action_handler_init(void);


/*
 * Control the signal, i.e. set the indicated signal to the indicated color
 */
void ah_set_signal(signal_t signal, color_t color);


/*
 * Writes a string at the given position.
 */
void ah_lcd_write(char text[]);
#endif
