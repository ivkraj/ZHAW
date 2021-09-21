/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module buttons.
 * 
 *  $Id: buttons.c 1064 2016-11-18 06:12:26Z feur $
 * ------------------------------------------------------------------------- */

 
/* User includes */
#include "buttons.h"
#include "observable.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define REG_BUTTONS (*((volatile uint8_t *)(0x60000210)))

#define BUTTON_T0   0x01
#define BUTTON_T1   0x02
#define BUTTON_T2   0x04
#define BUTTON_T3   0x08


/* -- Variables with module-wide scope
 * ------------------------------------------------------------------------- */

static observable_t observable_buttons_actions;


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/**
 * See header file
 */
void buttons_init(void)
{
    observable_init(&observable_buttons_actions);
}


/**
 * See header file
 */
uint32_t buttons_register_observer(void (*func_ptr)(uint32_t data))
{
    return observer_register_observer(&observable_buttons_actions, func_ptr);
}

/**
 *  See header file
 */
void buttons_check_pressed(void)
{
    static uint8_t old_buttons_value;
    uint8_t buttons_value;
    uint8_t rising_edges;

    buttons_value = REG_BUTTONS;
    rising_edges = buttons_value & ~old_buttons_value;
    old_buttons_value = buttons_value;

    if (rising_edges & BUTTON_T0) {
        observable_notify(&observable_buttons_actions, T0_PRESSED);
    }
    if (rising_edges & BUTTON_T1) {
        observable_notify(&observable_buttons_actions, T1_PRESSED);
    }
    if (rising_edges & BUTTON_T2) {
        observable_notify(&observable_buttons_actions, T2_PRESSED);
    }
    if (rising_edges & BUTTON_T3) {
        observable_notify(&observable_buttons_actions, T3_PRESSED);
    }
}
