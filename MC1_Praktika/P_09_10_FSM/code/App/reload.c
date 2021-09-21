/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Implementation of module reload.
 * 
 *  $Id: reload.c 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */
 

/* User includes */
#include "reload.h"


/* -- Macros
 * ------------------------------------------------------------------------- */
 
#define RELOAD_SECONDS  (10u)
#define RELOAD_MINUTES  (0u)


/* -- Variables with module-wide scope
 * ------------------------------------------------------------------------- */

static time_t reload_value = { RELOAD_MINUTES, RELOAD_SECONDS };


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 *  See header file
 */
void reload_inc_seconds(void)
{
    time_seconds_inc(&reload_value);
}


/*
 *  See header file
 */
void reload_inc_minutes(void) 
{
    time_minutes_inc(&reload_value);
}


/*
 *  See header file
 */
void reload_dec_seconds(void)
{
    time_seconds_dec(&reload_value);
}


/*
 *  See header file
 */
void reload_dec_minutes(void)
{
    time_minutes_dec(&reload_value);
}


/*
 * See header file
 */
void reload_get_output(seg7_output_t *output)
{
    output->value = time_get_bcd(&reload_value);
    output->dots = (0x1 << 2u);     // Set dot on DS2
}


/*
 * See header file
 */
time_t reload_get_time(void)
{
    return reload_value;
}
