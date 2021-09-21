/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Interface of module output.
 *
 *  $Id: output.h 1597 2018-10-04 09:07:09Z ruan $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _OUTPUT_H
#define _OUTPUT_H


/* Standard includes */
#include <hal_common.h>


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Initializes the output module
 */
void output_init(void);

/**
 *  \brief  Enables / disables the green output
 *  \param  status : If set to ENABLE turn green output on, otherwise off
 */
void output_set_green(hal_bool_t status);

/**
 *  \brief  Enables / disables the red output
 *  \param  status : If set to ENABLE turn red output on, otherwise off
 */
void output_set_red(hal_bool_t status);
#endif
