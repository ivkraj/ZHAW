/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Interface of module state_machine.
 * --
 * -- Reacts on events and triggers actions.
 * --
 * -- $Id: state_machine.h 2690 2015-11-18 15:37:30Z fert $
 * ------------------------------------------------------------------------- */

/* re-definition guard */
#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

/* standard includes */
#include <stdint.h>

/* user includes */
#include "event_detector.h"


/* -- Type definitions
 * ------------------------------------------------------------------------- */

typedef enum {
    INIT,
    CAR_E2_PED_W,
    PED_E_W,
    CAR_E1_E2,
    CAR_E2_PED_W_PED_E_W,
    PED_E_W_CAR_E2_PED_W,
    PED_E_W_CAR_E1_E2,
    CAR_E1_E2_PED_E_W,
    CAR_E1_E2_CAR_E2_PED_W,
    CAR_E2_PED_W_CAR_E1_E2,
  
} state_t;



/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/*
 * Initialize the state machine
 */
state_t fsm_init(void);

/*
 * set new state in fsm for debugging purposes!!
 */
void fsm_set_state( state_t new_state );

/*
 * Process the given event, based on actual state.
 */
state_t fsm_handle_event(event_t event);
#endif
