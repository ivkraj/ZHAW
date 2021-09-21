/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/** 
 *  \brief  Interface of module reload_ctrl.
 * 
 *  $Id: reload_ctrl.h 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _RELOAD_CTRL_H
#define _RELOAD_CTRL_H


/* Standard includes */
#include <stdint.h>


/* -- Public type definitions
 * ------------------------------------------------------------------------- */

/* enumerate the events */
typedef enum {
    RLC_NO_EVENT,
    RLC_DISPLAY_UPDATE_EVENT,   // just update the display
    RLC_BUTTON_SELECT_EVENT,    // change between seconds and minutes
    RLC_BUTTON_UP_EVENT,        // increment the selected reload value
    RLC_BUTTON_DOWN_EVENT       // decrement the selected reload value
} reload_ctrl_events_t;


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Put new event in this FSMs queue.
 *  \param  event : Event for the FSM to process.
 */
void rl_ctrl_put_queue(reload_ctrl_events_t event);

/**
 *  \brief  Handle events in queue of this FSM.
 */
void rl_ctrl_handle_event(void);

/**
 *  \brief  Update content for display for this FSM.
 */
void rl_ctrl_update_display(void);

#endif
