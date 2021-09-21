/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module action_handler.
 * --
 * -- $Id: action_handler.c 2746 2015-12-01 10:42:23Z fert $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include "reg_stm32f4xx.h"

/* user includes */
#include "action_handler.h"
#include "hal_gpio.h"
#include "hal_ct_lcd.h"

/* -- Macros
 * ------------------------------------------------------------------------- */

#define PINS_PORT_A (0xfff)

/* Public function definitions
 * ------------------------------------------------------------------------- */


/*
 * See header file
 */
void action_handler_init(void)
{
    hal_gpio_output_t gpio_init;

    /* Enable peripheral */
    GPIOA_ENABLE();
  
    /* Configure output port (GPIO A) */
    gpio_init.pins = PINS_PORT_A;
    gpio_init.pupd = HAL_GPIO_PUPD_DOWN;
    gpio_init.out_speed = HAL_GPIO_OUT_SPEED_2MHZ;
    gpio_init.out_type = HAL_GPIO_OUT_TYPE_PP;

    hal_gpio_init_output(GPIOA, gpio_init);
}


/*
 * See header file
 */
void ah_set_signal(signal_t signal, color_t color)
{
    uint16_t data = hal_gpio_output_read(GPIOA);
    
/* Emulate a coding error in case of CPPUTEST 
   This coding error has to be detected with the unit tests */
#ifdef CPPUTEST
    data &= ~(0x3 << signal / 2);       // reset bits
    data |= (color << signal / 2);      // set bits
#else
    data &= ~(0x3 << signal);           // reset bits
    data |= (color << signal);          // set bits    
#endif
    
    hal_gpio_output_write(GPIOA, data);
}


/*
 * See header file
 */
void ah_lcd_write(char text[])
{
#ifndef CPPUTEST
    hal_ct_lcd_clear();
    hal_ct_lcd_write(0, text);
  
#endif
}
