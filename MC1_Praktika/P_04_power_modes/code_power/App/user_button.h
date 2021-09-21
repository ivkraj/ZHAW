/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Interface of module user_button.
 *
 *  $Id: user_button.h 1597 2018-10-04 09:07:09Z ruan $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _USER_BUTTON_H
#define _USER_BUTTON_H


/* Standard includes */
#include <stdint.h>
#include <hal_common.h>


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Initializes the button module
 */
void button_init(void);

/**
 *  \brief  Determines if user button got pressed
 *  \return True if pressed, false if not pressed
 */
hal_bool_t did_button_get_pressed(void);
#endif
