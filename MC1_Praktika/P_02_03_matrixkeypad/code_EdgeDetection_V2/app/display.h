/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Project     : CT2 Lab Matrix-Keypad
 * -- Description : Modul handles the 8-digit 7-segment-display
 * --				Controls the buffer (data for each digit)
 * --				Updates the display with the stored values
 * --
 * -- $Id: display.h 1443 2018-08-23 06:28:34Z ruan $
 * ------------------------------------------------------------------------- */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "stdint.h"

/**
 *  \brief  Stores a new value into the display buffer and shifts the
 *          previously stored values left by one digit.
 *			The stored values will be displayed by calling disp_update().
 *  \param  value : New value to be stored
 */
void disp_reg_new_value(uint8_t value);

/**
 *  \brief  Updates the display
 *			The values which are stored in the buffer will be displayed.
 */
void disp_update(void);

/**
 *  \brief  Initialises the display-module including GPIOs, and Buffers
 *			and clears the display
 */
void disp_init(void);

/**
 *  \brief  Clears the values which are stored in the buffer,
 *			the display will be blank after next call of disp_update().
 */
void disp_clear_buffer(void);

#endif
