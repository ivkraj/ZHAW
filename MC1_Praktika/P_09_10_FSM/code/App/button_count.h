/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Interface of module button_count.
 *
 *  $Id: button_count.h 1302 2017-11-22 10:50:03Z kjaz $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _BUTTON_COUNT_H
#define _BUTTON_COUNT_H


/* Standard includes */
#include <stdint.h>


/* User includes */
#include "seg7_display.h"
#include "buttons.h"


/* -- Public type definitions
 * ------------------------------------------------------------------------- */

/* enumerate the events */
typedef enum {
    BCC_NO_EVENT,
    BCC_BUTTON_T0_EVENT = T0_PRESSED,   // Has to be Tx_PRESSED so the module
    BCC_BUTTON_T1_EVENT = T1_PRESSED,   // can handle both parameter
    BCC_BUTTON_T2_EVENT = T2_PRESSED,
    BCC_BUTTON_T3_EVENT = T3_PRESSED,
    BCC_DISPLAY_UPDATE_EVENT
} button_count_events_t;


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Initialize module button_count_ctrl.
 */
void button_count_init(void);

/**
 *  \brief  Put new event in this FSMs queue.
 *  \param  event : Event for the FSM to process.
 */
void button_count_put_queue(button_count_events_t event);

/**
 *  \brief  Handle events in queue of this FSM.
 */
void button_count_handle_event(void);

/**
 *  \brief  Returns 7 segment output data.
 *  \param  output : Pointer to struct to store output data.
 */
void button_count_get_output(seg7_output_t *output);

/**
 *  \brief  Update content for display for this FSM.
 */
void button_count_update_display(void);

#endif
