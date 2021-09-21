/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Interface of module seg7_display.
 *
 *  $Id: seg7_display.h 1071 2016-11-18 12:38:50Z ruan $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _SEG7_DISPLAY_H
#define _SEG7_DISPLAY_H

/* Standard includes */
#include <stdint.h>


/* -- Type definitions
 * ------------------------------------------------------------------------- */

/**
 *  \struct seg7_output_t
 *  \brief  Struct for outputing data on 7 segment display.
 */
typedef struct {
    uint16_t value;     /**< Binary data to show on display. */
    uint8_t dots;       /**< Nr. of display the dot is shown on. */
} seg7_output_t;

/**
 *  \brief  Declaration of callback signature.
 */
typedef void (*seg7_callback_t)(seg7_output_t *output);


/* -- Public function declarations
 * ------------------------------------------------------------------------- */
 
/**
 *  \brief  Output data to 7 segment display.
 *
 *  If you call this function the value retrieved by the callback function
 *  gets displayed on the 7 segment display.
 */
void seg7_output_update(void);

/**
 *  \brief  Register callback for 7 segment display.
 *
 *  The callback function to register shall have the following signature:
 *      void function_name(seg7_output_t *output);
 *
 *  The output argument is a pointer to a seg7_output_t structure, which
 *  contains the binary value and the dots for outputting on the display.
 *  The callback function has to fill the struct with data.
 *
 *  \param  function : Callback function to register.
 */
void seg7_register_get_output_callback(seg7_callback_t function);

#endif
