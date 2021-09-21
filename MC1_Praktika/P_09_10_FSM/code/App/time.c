/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Implementation of module time.
 * 
 *  $Id: time.c 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */
 

/* User includes */
#include "time.h"


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 *  See header file.
 */
void time_seconds_inc(time_t *time)
{
    time->seconds++;
    
    if (time->seconds > 59u) {
        time->seconds = 0u;
        time_minutes_inc(time);
    }    
}


/*
 *  See header file.
 */
void time_seconds_dec(time_t *time)
{
    time->seconds--;
    
    if (time->seconds > 59u) {
        time->seconds = 59u;
        time_minutes_dec(time);
    }    
}


/*
 *  See header file.
 */
void time_minutes_inc(time_t *time)
{
    time->minutes++;
    
    if (time->minutes > 99u) {
        time->minutes = 0u;
    }    
}


/*
 *  See header file.
 */
void time_minutes_dec(time_t *time)
{
    time->minutes--;
    
    if (time->minutes > 99u) {
        time->minutes = 99u;
    }
}


/*
 *  See header file.
 */
uint16_t time_get_bcd(time_t *time)
{
    uint8_t temp;
    uint16_t bcd;
        
    /* Convert seconds */
    temp = time->seconds;
    bcd = temp % 10u;
    temp /= 10u;
    bcd |= (temp % 10u) << 4u;
    
    /* Convert minutes */
    temp = time->minutes;
    bcd |= (temp % 10u) << 8u;
    temp /= 10u;
    bcd |= (temp % 10u) << 12u;
    
    return bcd;
}
