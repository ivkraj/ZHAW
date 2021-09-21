/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module state_machine.
 * --
 * -- $Id: state_machine.c 2690 2015-11-18 15:37:30Z fert $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <reg_stm32f4xx.h>

/* user includes */
#include "state_machine.h"
#include "action_handler.h"
#include "timer.h"


/* -- Macros used by student code
 * ------------------------------------------------------------------------- */

#define TIMER_DURATION       200u

#define TEXT_INIT                     "INIT                  "

#define TEXT_CAR_E2_PED_W             "CAR_E2_PED_W          "
#define TEXT_CAR_E1_E2                "CAR_E1_E2             "
#define TEXT_PED_E_W                  "PED_E_W               "
#define TEXT_CAR_E1_E2_CAR_E2_PED_W   "CAR_E1_E2_CAR_E2_PED_W"
#define TEXT_CAR_E2_PED_W_CAR_E1_E2   "CAR_E2_PED_W_CAR_E1_E2"
#define TEXT_CAR_E1_E2_PED_E_W        "CAR_E1_E2_PED_E_W     "
#define TEXT_PED_E_W_CAR_E1_E2        "PED_E_W_CAR_E1_E2     "
#define TEXT_PED_E_W_CAR_E2_PED_W     "PED_E_W_CAR_E2_PED_W  "
#define TEXT_CAR_E2_PED_W_PED_E_W     "CAR_E2_PED_W_PED_E_W  "


/* Local variables
 * ------------------------------------------------------------------------- */

static state_t state = INIT;


/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
state_t fsm_init(void)
{
    action_handler_init();

    /* Set initial signal color */
    ah_set_signal(SIGNAL_CAR_W, COLOR_NO);
    ah_set_signal(SIGNAL_CAR_S, COLOR_NO);
    ah_set_signal(SIGNAL_CAR_E1, COLOR_YELLOW);
    ah_set_signal(SIGNAL_CAR_E2, COLOR_YELLOW);
    ah_set_signal(SIGNAL_PED_W, COLOR_YELLOW);
    ah_set_signal(SIGNAL_PED_E, COLOR_YELLOW);

    timer_start(TIMER_DURATION);
    ah_lcd_write(TEXT_INIT);

    state = INIT;
    
    return state;
}

/*
 * See header file
 */
void fsm_set_state( state_t new_state )
{
    state = new_state;
}

/*
 * See header file
 */
state_t fsm_handle_event(event_t event)
{
    switch (state) {
        /* --------------------------------------------- CAR_S ------------- */
        case INIT:
            switch (event) {
                case TIME_OUT:
                    ah_set_signal(SIGNAL_CAR_E1, COLOR_RED);
                    ah_set_signal(SIGNAL_CAR_E2, COLOR_GREEN);
                    ah_set_signal(SIGNAL_PED_W, COLOR_GREEN);
                    ah_set_signal(SIGNAL_PED_E, COLOR_RED);

                    ah_lcd_write(TEXT_CAR_E2_PED_W);
                    state = CAR_E2_PED_W;
                    break;

                default:
                    break;
            }
            break;
        /* --------------------------------------------- CAR_S ------------- */
        case CAR_E2_PED_W:
            switch (event) {
                case EV_CAR_E1:
                    timer_start(TIMER_DURATION);

                    ah_set_signal(SIGNAL_CAR_E1, COLOR_RED);
                    ah_set_signal(SIGNAL_CAR_E2, COLOR_GREEN);
                    ah_set_signal(SIGNAL_PED_W, COLOR_YELLOW);
                    ah_set_signal(SIGNAL_PED_E, COLOR_RED);

                    ah_lcd_write(TEXT_CAR_E1_E2_CAR_E2_PED_W);
                    state = CAR_E2_PED_W_CAR_E1_E2;
                    break;

                case EV_PED_E:
                    timer_start(TIMER_DURATION);

                    ah_set_signal(SIGNAL_CAR_E1, COLOR_RED);
                    ah_set_signal(SIGNAL_CAR_E2, COLOR_YELLOW);
                    ah_set_signal(SIGNAL_PED_W, COLOR_GREEN);
                    ah_set_signal(SIGNAL_PED_E, COLOR_RED);

                    ah_lcd_write(TEXT_CAR_E2_PED_W_PED_E_W);
                    state = CAR_E2_PED_W_PED_E_W;
                    break;
                
                case TIME_OUT:    // error introduced for unit tests
                    state = INIT;
                    break;

                default:
                    break;
            }
            break;
        /* --------------------------------------------- CAR_S_W ----------- */
        case CAR_E1_E2:
            switch (event) {
                case EV_PED_E:
                    timer_start(TIMER_DURATION);

                    ah_set_signal(SIGNAL_CAR_E1, COLOR_YELLOW);
                    ah_set_signal(SIGNAL_CAR_E2, COLOR_YELLOW);
                    ah_set_signal(SIGNAL_PED_W, COLOR_RED);
                    ah_set_signal(SIGNAL_PED_E, COLOR_RED);

                    ah_lcd_write(TEXT_CAR_E1_E2_PED_E_W);
                    state = CAR_E1_E2_PED_E_W;
                    break;

                case EV_PED_W:
                    timer_start(TIMER_DURATION);

                    ah_set_signal(SIGNAL_CAR_E1, COLOR_YELLOW);
                    ah_set_signal(SIGNAL_CAR_E2, COLOR_GREEN);
                    ah_set_signal(SIGNAL_PED_W, COLOR_RED);
                    ah_set_signal(SIGNAL_PED_E, COLOR_RED);

                    ah_lcd_write(TEXT_CAR_E2_PED_W_CAR_E1_E2);
                    state = CAR_E1_E2_CAR_E2_PED_W;
                    break;

                default:
                    break;
            }
            break;
        /* --------------------------------------------- PED_E_W ----------- */
        case PED_E_W:
            switch (event) {
                case EV_CAR_E1:
                    timer_start(TIMER_DURATION);

                    ah_set_signal(SIGNAL_CAR_E1, COLOR_RED);
                    ah_set_signal(SIGNAL_CAR_E2, COLOR_RED);
                    ah_set_signal(SIGNAL_PED_W, COLOR_YELLOW);
                    ah_set_signal(SIGNAL_PED_E, COLOR_YELLOW);

                    ah_lcd_write(TEXT_PED_E_W_CAR_E1_E2);
                    state = PED_E_W_CAR_E1_E2;
                    break;

                case EV_CAR_E2:
                    timer_start(TIMER_DURATION);

                    ah_set_signal(SIGNAL_CAR_E1, COLOR_RED);
                    ah_set_signal(SIGNAL_CAR_E2, COLOR_RED);
                    ah_set_signal(SIGNAL_PED_W, COLOR_GREEN);
                    ah_set_signal(SIGNAL_PED_E, COLOR_YELLOW);

                    ah_lcd_write(TEXT_PED_E_W_CAR_E2_PED_W);
                    state = PED_E_W_CAR_E2_PED_W;
                    break;

                default:
                    break;
            }
            break;
        /* --------------------------------------------- CAR_S_CAR_S_W ----- */
        case CAR_E2_PED_W_CAR_E1_E2:
            switch (event) {
                case TIME_OUT:
                    ah_set_signal(SIGNAL_CAR_E1, COLOR_GREEN);
                    ah_set_signal(SIGNAL_CAR_E2, COLOR_GREEN);
                    ah_set_signal(SIGNAL_PED_W, COLOR_RED);
                    ah_set_signal(SIGNAL_PED_E, COLOR_RED);

                    ah_lcd_write(TEXT_CAR_E1_E2);
                    state = CAR_E1_E2;
                    break;

                default:
                    break;
            }
            break;
        /* --------------------------------------------- CAR_S_W_CAR_S ----- */
        case CAR_E1_E2_CAR_E2_PED_W:
            switch (event) {
                case TIME_OUT:
                    ah_set_signal(SIGNAL_CAR_E1, COLOR_RED);
                    ah_set_signal(SIGNAL_CAR_E2, COLOR_GREEN);
                    ah_set_signal(SIGNAL_PED_W, COLOR_GREEN);
                    ah_set_signal(SIGNAL_PED_E, COLOR_RED);

                    ah_lcd_write(TEXT_CAR_E2_PED_W);
                    state = CAR_E2_PED_W;
                    break;

                default:
                    break;
            }
            break;
        /* --------------------------------------------- CAR_S_W_PED_E_W --- */
        case CAR_E1_E2_PED_E_W:
            switch (event) {
                case TIME_OUT:
                    ah_set_signal(SIGNAL_CAR_E1, COLOR_RED);
                    ah_set_signal(SIGNAL_CAR_E2, COLOR_RED);
                    ah_set_signal(SIGNAL_PED_W, COLOR_GREEN);
                    ah_set_signal(SIGNAL_PED_E, COLOR_GREEN);

                    ah_lcd_write(TEXT_PED_E_W);
                    state = PED_E_W;
                    break;

                default:
                    break;
            }
            break;
        /* --------------------------------------------- PED_E_W_CAR_S_W --- */
        case PED_E_W_CAR_E1_E2:
            switch (event) {
                case TIME_OUT:
                    ah_set_signal(SIGNAL_CAR_E1, COLOR_GREEN);
                    ah_set_signal(SIGNAL_CAR_E2, COLOR_GREEN);
                    ah_set_signal(SIGNAL_PED_W, COLOR_RED);
                    ah_set_signal(SIGNAL_PED_E, COLOR_RED);

                    ah_lcd_write(TEXT_CAR_E1_E2);
                    state = CAR_E1_E2;
                    break;

                default:
                    break;
            }
            break;
        /* --------------------------------------------- PED_E_W_CAR_S ---- */
        case PED_E_W_CAR_E2_PED_W:
            switch (event) {
                case TIME_OUT:
                    ah_set_signal(SIGNAL_CAR_E1, COLOR_RED);
                    ah_set_signal(SIGNAL_CAR_E2, COLOR_GREEN);
                    ah_set_signal(SIGNAL_PED_W, COLOR_GREEN);
                    ah_set_signal(SIGNAL_PED_E, COLOR_RED);

                    ah_lcd_write(TEXT_CAR_E2_PED_W);
                    state = CAR_E2_PED_W;
                    break;

                default:
                    break;
            }
            break;
        /* --------------------------------------------- CAR_S_PED_E_W ----- */
        case CAR_E2_PED_W_PED_E_W:
            switch (event) {
                case TIME_OUT:
                    ah_set_signal(SIGNAL_CAR_E1, COLOR_RED);
                    ah_set_signal(SIGNAL_CAR_E2, COLOR_RED);
                    ah_set_signal(SIGNAL_PED_W, COLOR_GREEN);
                    ah_set_signal(SIGNAL_PED_E, COLOR_GREEN);

                    ah_lcd_write(TEXT_PED_E_W);
                    state = PED_E_W;
                    break;

                default:
                    break;
            }
            break;
        /* --------------------------------------------- DEFAULT ----------- */
        default:
            break;
    }

    return state;
}
