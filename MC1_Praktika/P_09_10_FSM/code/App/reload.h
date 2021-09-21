/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Interface of module reload.
 * 
 *  $Id: reload.h 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _RELOAD_H
#define _RELOAD_H


/* Standard includes */
#include <stdint.h>

/* User includes */
#include "time.h"
#include "seg7_display.h"


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Increment seconds on reload value.
 */
void reload_inc_seconds(void);

/**
 *  \brief  Increment minutes on reload value.
 */
void reload_inc_minutes(void);

/**
 *  \brief  Decrement seconds on reload value.
 */
void reload_dec_seconds(void);

/**
 *  \brief  Decrement minutes on reload value.
 */
void reload_dec_minutes(void);

/**
 *  \brief  Returns 7 segment output data.
 *  \param  output : Pointer to struct to store output data.
 */
void reload_get_output(seg7_output_t *output);

/**
 *  \brief  Returns reload value.
 *  \return	Struct with reload value.
 */
time_t reload_get_time(void);

#endif
