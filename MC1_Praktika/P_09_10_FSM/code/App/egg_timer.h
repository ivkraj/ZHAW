/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Interface of module egg_timer.
 * 
 *  $Id: egg_timer.h 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _EGG_TIMER_H
#define _EGG_TIMER_H


/* Standard includes */
#include <stdint.h>

/* User includes */
#include "seg7_display.h"


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Loads the egg_timer with the reload_value, initiates a
 *          a one second SW timer that will decrement.
 *  \param  func_ptr : Pointer to the callback function that will be
 *                     called when the egg_timer elapses
 */
void egg_timer_start(void (*func_ptr)(void));

/**
 *  \brief  Stops the SW timer and therefore the decrementing of the
 *          egg_timer.
 */
void egg_timer_stop(void);

/**
 *  \brief  Resumes decrementing the counter, i.e. it restarts the
 *          one second SW timer.
 */
void egg_timer_resume(void);

/**
 *  \brief  Returns 7 segment output data.
 *  \param  output : Pointer to struct to store output data.
 */
void egg_timer_get_output(seg7_output_t *output);
    
#endif
