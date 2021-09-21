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
 * -- Description : Modul for using the mode switches (DIP_SWITCHES) and
 * --               the mode-leds
 * --
 * -- $Id: inout.h 1443 2018-08-23 06:28:34Z ruan $
 * ------------------------------------------------------------------------- */

#ifndef _INOUT_H
#define _INOUT_H

#include "stdint.h"

/**
 *  \brief  Initialises the GPIOs used for the two DIP-Switches and LEDs
 *			must be called before using the DIP-Switches and LEDs
 */
void inout_init_io(void);

/**
 *  \brief  Reads the values of the dip-switches (called mode-keys)
 *  \return Value of the two switches (0..3)
 */
uint8_t inout_switch_read(void);

/**
 *  \brief  Writes current mode to the leds.
 *  \param  mode_nr: Number of the current mode (0..3).
 */
void inout_led_write(uint8_t mode_nr);
#endif
