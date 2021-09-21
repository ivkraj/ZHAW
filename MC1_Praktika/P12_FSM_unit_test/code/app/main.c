/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur             -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences)           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Project     : CT2 lab - FSM ampel
 * -- Description : Main program.
 * --
 * --               Task 1: - Implement green/red phase.
 * --               Task 2: - Implement transition with yellow phase.
 * --               Task 3: - Create own crossroad situation.
 * --
 * --
 * -- $Id: main.c 2690 2015-11-18 15:37:30Z fert $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <reg_stm32f4xx.h>

/* user includes */
#include "event_detector.h"
#include "state_machine.h"
#include "timer.h"


/* -- M A I N
 * ------------------------------------------------------------------------- */

int main(void)
{
    event_t event;

    ed_init();
    fsm_init();
    timer_init();

    while (1) {
        event = ed_get_event();
        if (event != NO_EVENT) {
            fsm_handle_event(event);
        }
        timer_wait_for_tick();
    }
}

