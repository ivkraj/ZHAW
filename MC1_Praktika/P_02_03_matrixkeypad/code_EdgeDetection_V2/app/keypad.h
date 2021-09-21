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
 * -- Description : Modul handles the reading of the keypad
 * --
 * -- $Id: keypad.h 1539 2018-09-20 06:17:18Z kjaz $
 * ------------------------------------------------------------------------- */

/* re-definition guard */
#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "stdint.h"

#define NOKEY_PRESSED 0xFF

/**
 *  \brief  Scans the matrix-keypad column-wise
 *  \param  none
 *  \return The value of the pressed key (0..15)
            0xFF if no key is pressed
 */
uint8_t scan_keypad_cbc(void);

/**
 *  \brief  Scans the matrix-keypad by switching inputs and outputs
 *  \param  none
 *  \return The value of the pressed key (0..15) on success
 *          0xFF if no key is pressed
 */
uint8_t scan_keypad_fast(void);

#endif
