/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module user_button.
 *
 *  $Id: user_button.c 1597 2018-10-04 09:07:09Z ruan $
 * ------------------------------------------------------------------------- */


/* Standard includes */
#include <reg_stm32f4xx.h>
#include <hal_gpio.h>

/* User includes */
#include "user_button.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define PIN_USER_BUTTON (0x1 << 0u)


/* -- Module-wide variable definitions
 * ------------------------------------------------------------------------- */

// Set to TRUE by interrupt handler
// Reset to FALSE through function did_button_get_pressed()
static volatile hal_bool_t button_pressed = FALSE;


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 *  See header file
 */
void button_init(void)
{
    /* Initialize gpio pin as input */
    hal_gpio_input_t gpio_init;

    /* Initialise gpio pin for interrupt */
    gpio_init.pins = PIN_USER_BUTTON;
    gpio_init.pupd = HAL_GPIO_PUPD_NOPULL;

    hal_gpio_init_input(GPIOA, gpio_init);
    hal_gpio_irq_set(GPIOA, PIN_USER_BUTTON, HAL_GPIO_TRG_POS, ENABLE);   
}


/*
 *  See header file
 */
hal_bool_t did_button_get_pressed(void)
{
    hal_bool_t return_val = FALSE;

    /// STUDENTS: To be programmed
    
    if (button_pressed == TRUE) {
        return_val = TRUE;
        button_pressed = FALSE;
    }

    /// END: To be programmed

    return return_val;
}


/* -- Interrupt function definitions
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed
void EXTI0_IRQHandler(void) {
    hal_gpio_irq_clear(PIN_USER_BUTTON);
    button_pressed = TRUE;
}



/// END: To be programmed
