/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module output (target: discovery).
 *
 *  $Id: output.c 1597 2018-10-04 09:07:09Z ruan $
 * ------------------------------------------------------------------------- */


/* Standard includes */
#include <reg_stm32f4xx.h>
#include <hal_gpio.h>

/* User includes */
#include "output.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define LED_GREEN (0x1 << 13u)
#define LED_RED   (0x1 << 14u)


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 *  See header file
 */
void output_init(void)
{
    hal_gpio_output_t gpio_init;

    /* Initialize gpio pins */
    gpio_init.pins = LED_GREEN | LED_RED;
    gpio_init.pupd = HAL_GPIO_PUPD_NOPULL;
    gpio_init.out_speed = HAL_GPIO_OUT_SPEED_10MHZ;
    gpio_init.out_type = HAL_GPIO_OUT_TYPE_PP;

    hal_gpio_init_output(GPIOG, gpio_init);

    /* Default state for LEDs */
    hal_gpio_bit_reset(GPIOG, LED_GREEN);
    hal_gpio_bit_reset(GPIOG, LED_RED);
}


/*
 *  See header file
 */
void output_set_green(hal_bool_t status)
{
    if (status == ENABLE) {
        hal_gpio_bit_set(GPIOG, LED_GREEN);
    } else {
        hal_gpio_bit_reset(GPIOG, LED_GREEN);
    }
}


/*
 *  See header file
 */
void output_set_red(hal_bool_t status)
{
    if (status == ENABLE) {
        hal_gpio_bit_set(GPIOG, LED_RED);
    } else {
        hal_gpio_bit_reset(GPIOG, LED_RED);
    }
}

