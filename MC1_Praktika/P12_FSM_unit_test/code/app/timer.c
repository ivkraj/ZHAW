/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module timer.
 * --
 * -- $Id: timer.c 2690 2015-11-18 15:37:30Z fert $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>

/* user includes */
#include "timer.h"


/* Local variables
 * ------------------------------------------------------------------------- */

static uint16_t timer_count = 0u;



/* Public function definitions
 * ------------------------------------------------------------------------- */
 
 /*
 * Initialize hardware timer;  Content removed for unit test lab
 */
void timer_init(void)
{     
}

/*
 * See header file
 */
void timer_start(uint16_t duration)
{
    timer_count = duration;
}

/*
 * See header file
 */
void timer_stop(void)
{
    timer_count = 0u;
}

/*
 * See header file
 */
void timer_decrement(void)
{
    if (timer_count > 0u) {
        timer_count--;
    }
}

/*
 * See header file
 */
uint16_t timer_read(void)
{
    return timer_count;
}

/*
 * See header file. Content removed for unit test lab
 */
void timer_wait_for_tick(void)
{
}
