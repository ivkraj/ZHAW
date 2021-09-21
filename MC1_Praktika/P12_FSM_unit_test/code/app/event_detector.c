/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module event_handler.
 * --
 * -- $Id: event_handler.c 2690 2015-11-18 15:37:30Z fert $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <reg_stm32f4xx.h>

/* user includes */
#include "event_detector.h"
#include "timer.h"
#include "hal_gpio.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define PINS_PORT_B (0x3f)

/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void ed_init(void)
{
    hal_gpio_input_t gpio_init;

    /* Enable peripheral */
    GPIOB_ENABLE();

    /* Configure input port (GPIO B) */
    //   gpio_init.pins = PINS_PORT_B;
    gpio_init.pupd = HAL_GPIO_PUPD_DOWN;

    hal_gpio_init_input(GPIOB, gpio_init);
}

/*
 * See header file
 */
event_t ed_get_event(void)
{
    event_t event = NO_EVENT;

    static uint8_t port_value_old = 0xff;
    uint8_t port_value;
    uint8_t edge_positive;

    static uint16_t timer_old = 0u;
    uint16_t timer;
   
    /* update old timer before decrementing! */
    /* bugfix from when it was done afterwards */
    timer_old = timer_read();
    
    /* Execute a timer tick at each execution of get_event */
    timer_decrement();

    /* Read the input port */
    port_value = (uint8_t)hal_gpio_input_read(GPIOB);

    /* Detect edges: Set the corresponding bit for each detected edge */
    edge_positive = ~port_value_old & port_value;

    /* Get the current timer value */
    timer = timer_read();

    /* Detect timer event */
    if (timer == 0u && timer_old == 1u) {
        event = TIME_OUT;
    }
    /* Detect other events */
    else if (edge_positive & MASK_CAR_W) {
        event = EV_CAR_W;
    } else if (edge_positive & MASK_CAR_S) {
        event = EV_CAR_S;
    } else if (edge_positive & MASK_CAR_E1) {
        event = EV_CAR_E1;
    } else if (edge_positive & MASK_CAR_E2) {
        event = EV_CAR_E2;
    } else if (edge_positive & MASK_PED_W) {
        event = EV_PED_W;
    } else if (edge_positive & MASK_PED_E) {
        event = EV_PED_E;
    }

    /* Update port_state_old and timer_old */
    port_value_old = port_value;

    return event;
}
