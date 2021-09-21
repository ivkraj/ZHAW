/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Interface of module mode_control.
 *
 *  $Id: mode_control.h 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _MODE_CONTROL_H
#define _MODE_CONTROL_H


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Initializes mode_control. To be called before the mode_control
 *          FSM can be used
 */
void mode_control_init(void);

/**
 *  \brief  Implements the FSM of mode_control
 *          Needs to be called periodically. Will poll the queue for
 *          events and process them.
 */
void mode_control_handle_event(void);


#endif
