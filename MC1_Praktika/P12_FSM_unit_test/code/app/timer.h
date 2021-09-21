/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Interface of module timer.
 * --
 * -- $Id: timer.h 2690 2015-11-18 15:37:30Z fert $
 * ------------------------------------------------------------------------- */

/* re-definition guard */
#ifndef _TIMER_H
#define _TIMER_H

/* standard includes */
#include <stdint.h>


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/*
 * Initialize hardware timer
 */
void timer_init(void);

/*
 * Start timer with specified duration (clocks of 100 Hz).
 */
void timer_start(uint16_t duration);

/*
 * Stop timer.
 */
void timer_stop(void);

/*
 * Decrement timer variable.
 */
void timer_decrement(void);

/*
 * Returns actual value of timer variable.
 */
uint16_t timer_read(void);

/*
 * Wait till the 100 Hz timer ticks.
 */
void timer_wait_for_tick(void);
#endif

