/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Interface of module event_handler.
 * --
 * -- Generates events based on rising or falling edges on inport.
 * --
 * -- $Id: event_handler.h 2690 2015-11-18 15:37:30Z fert $
 * ------------------------------------------------------------------------- */

/* re-definition guard */
#ifndef _EVENT_HANDLER_H
#define _EVENT_HANDLER_H

/* standard includes */
#include <stdint.h>


/* -- Type definitions
 * ------------------------------------------------------------------------- */

typedef enum {
    NO_EVENT,
    TIME_OUT,
    EV_CAR_E1,
    EV_CAR_E2,
    EV_CAR_S,
    EV_CAR_W,
    EV_PED_E,
    EV_PED_W
} event_t;

/* -- Macros to mask input ports
 * ------------------------------------------------------------------------- */

#define MASK_CAR_W  (0x01)
#define MASK_CAR_S  (0x02)
#define MASK_CAR_E1 (0x04)
#define MASK_CAR_E2 (0x08)
#define MASK_PED_W  (0x10)
#define MASK_PED_E  (0x20)

/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/*
 * Initialize event handler (GPIO etc.)
 */
void ed_init(void);


/*
 * Check for and return the appropriate events
 *   (a) rising edges on the buttons, i.e. the value on the respective inport 
 *       has changed from a zero (during the last call to ed_get_event()) to a
 *       ONE during this call.
 *   (b) time out on the timer, i.e. the reading of the timer has changed from
 *       ONE (during the last call to ed_get_event()) to ZERO during this call
 *
 * Each call to ed_get_event() decrements the timer by 1
 */
event_t ed_get_event(void);
#endif
