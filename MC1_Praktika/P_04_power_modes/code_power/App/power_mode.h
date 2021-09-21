/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Interface of module power_mode.
 *
 *  $Id: power_mode.h 1597 2018-10-04 09:07:09Z ruan $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _POWER_MODE_H
#define _POWER_MODE_H


/* Standard includes */
#include <stdint.h>


/* -- Type definitions
 * ------------------------------------------------------------------------- */

/**
 *  \enum   power_clk_t
 *  \brief  Defines the available system clocks.
 */
typedef enum {
    CLOCK_168MHZ,
    CLOCK_120MHZ,
    CLOCK_84MHZ,
    CLOCK_60MHZ
} power_clk_t;


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Initializes the power module
 */
void power_init(void);

/**
 *  \brief  Configure power and clocks for run mode
 */
void power_set_clock(const power_clk_t clock);

/**
 *  \brief  Let the microcontroller enter sleep mode
 */
void power_enter_sleep(void);

/**
 *  \brief  Let the microcontroller enter stop mode
 */
void power_enter_stop(void);
#endif
