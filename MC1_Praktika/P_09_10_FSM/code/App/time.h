/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Interface of module time.
 * 
 *  $Id: time.h 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _TIME_H
#define _TIME_H


/* Standard includes */
#include <stdint.h>


/* -- Public type definitions
 * ------------------------------------------------------------------------- */

typedef struct {
    uint8_t minutes;
    uint8_t seconds;
} time_t;


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Increment seconds of time struct.
 *  \param  *time : Pointer to time struct.
 */
void time_seconds_inc(time_t *time);

/**
 *  \brief  Decrement seconds of time struct.
 *  \param  *time : Pointer to time struct.
 */
void time_seconds_dec(time_t *time);

/**
 *  \brief  Increment minutes of time struct.
 *  \param  *time : Pointer to time struct.
 */
void time_minutes_inc(time_t *time);

/**
 *  \brief  Decrements minutes of time struct.
 *  \param  *time : Pointer to time struct.
 */
void time_minutes_dec(time_t *time);

/**
 *  \brief  Get bcd coded, 16 bit wide representation.
 *  \param  *time : Pointer to time struct.
 *  \return Converted time struct.
 */
uint16_t time_get_bcd(time_t *time);


#endif
