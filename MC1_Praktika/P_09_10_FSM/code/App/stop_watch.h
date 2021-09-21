/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Interface of module stop_watch.
 * 
 *  $Id: stop_watch.h 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _STOP_WATCH_H
#define _STOP_WATCH_H


/* Standard includes */
#include <stdint.h>

/* User includes */
#include "seg7_display.h"


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Start stop watch.
 */
void stop_watch_start(void);

/**
 *  \brief  Stop stop watch.
 */
void stop_watch_stop(void);

/**
 *  \brief  Reset stop watch.
 */
void stop_watch_reset(void);

/**
 *  \brief  Returns 7 segment output data.
 *  \param  output : Pointer to struct to store output data.
 */
void stop_watch_get_output(seg7_output_t *output);


#endif
