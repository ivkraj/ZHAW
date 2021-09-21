/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Project     : CT2 Lab Matrix-Keypad
 * -- Description : Modul for using the mode switches (DIP_SWITCHES) and
 * --               the mode-leds
 * --
 * -- $Id: inout.c 1443 2018-08-23 06:28:34Z ruan $
 * ------------------------------------------------------------------------- */

#include "inout.h"
#include "hal_gpio.h"

#define DIP_SW_PORT    GPIOB
#define LED_PORT       GPIOB
#define MODE_KEY_MASK  0x0300
#define MODE_KEY_SHIFT 8
#define LED_MASK       0x0C00
#define LED_SHIFT      10

/*
 * See header file
 */
void inout_init_io(void)
{
    hal_gpio_input_t mode_keys = { MODE_KEY_MASK,
                                   HAL_GPIO_PUPD_NOPULL };
    hal_gpio_output_t leds = { LED_MASK,
                               HAL_GPIO_PUPD_NOPULL,
                               HAL_GPIO_OUT_SPEED_2MHZ,
                               HAL_GPIO_OUT_TYPE_PP };
    hal_gpio_init_input(DIP_SW_PORT, mode_keys);
    hal_gpio_init_output(LED_PORT, leds);
}
/*
 * See header file
 */
uint8_t inout_switch_read(void)
{
    uint16_t input = 0;
    input = ~hal_gpio_input_read(DIP_SW_PORT); // switch-positions inverted
    input &= MODE_KEY_MASK;
    input = input >> MODE_KEY_SHIFT;
    return (uint8_t)input;
}

/*
 * See header file
 */
void inout_led_write(uint8_t mode_nr)
{
    uint16_t outbuf = 0;
    outbuf = hal_gpio_output_read(LED_PORT);    // read current outputs
    outbuf &= ~LED_MASK;                         // clear led-outputs
    outbuf |= (((uint16_t)mode_nr << LED_SHIFT) & LED_MASK);
    // set desired value for the leds
    hal_gpio_output_write(LED_PORT, outbuf);
}
