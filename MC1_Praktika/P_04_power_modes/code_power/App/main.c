/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur             -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences)           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Main program
 *
 *  $Id: main.c 1804 2019-09-13 15:52:32Z kjaz $
 * ------------------------------------------------------------------------- */

/* Standard includes */
#include <reg_stm32f4xx.h>
#include <hal_gpio.h>

/* User includes */
#include "power_mode.h"
#include "output.h"
#include "work.h"
#include "user_button.h"
#include "wakeup_timer.h"

#define CLOCK (CLOCK_168MHZ)

/* Switch between task with FSM and task with wakeup_timer */
//const hal_bool_t use_wakeup_timer = FALSE;    // uncomment for FSM
const hal_bool_t use_wakeup_timer = TRUE;   // uncomment for wakeup_timer

/* States for Finite State Machine (FSM) */
typedef enum {
    ON_STATE    = 0x00,      // RED on , GREEN on
    RUN_STATE   = 0x01,      // RED off, GREEN on
    SLEEP_STATE = 0x02,      // RED on , GREEN off
    STOP_STATE  = 0x03,      // RED off, GREEN off
} state_type;


/* -- Local function declarations
 * ------------------------------------------------------------------------- */
static void enable_peripherals(void);
static void init_gpio(void);

/* -- M A I N
 * ------------------------------------------------------------------------- */
int main(void)
{
    /* Set initial state */
    state_type state = ON_STATE;
    
    /* Enable used peripherals: GPIOs and SYSCFG */
    enable_peripherals();
  
    /* Set unused Ports as output */
    init_gpio();    

    /* Setup system clock */
    power_set_clock(CLOCK);

    /* Initialize modules */
    power_init();
    button_init();
    output_init();
    
    /* ---------- Choose task ---------------------------------------------- */
    if (use_wakeup_timer == FALSE) {
        
        /* FSM to measure current consumption in different power modes */
        
        output_set_green(ENABLE);
        output_set_red(ENABLE);

        while (1) {
            if (did_button_get_pressed()) {
                switch (state) {
                    case ON_STATE:
                        output_set_green(ENABLE);
                        output_set_red(DISABLE);
                        state = RUN_STATE;
                        break;

                    case RUN_STATE:
                        output_set_green(DISABLE);
                        output_set_red(ENABLE);
                        power_enter_sleep();
                        state = SLEEP_STATE;
                        break;

                    case SLEEP_STATE:
                        output_set_green(DISABLE);
                        output_set_red(DISABLE);
                        power_enter_stop();
                        state = STOP_STATE;
                        break;

                    case STOP_STATE:
                        power_set_clock(CLOCK);
                        output_set_green(ENABLE);
                        output_set_red(ENABLE);
                        state = ON_STATE;
                        break;
                }
            }
        }
        
    } else {
        /* Code for task with wakeup timer goes here */

        wakeup_init();

        while (1) {
            /// STUDENTS: To be programmed 
            
            output_set_green(ENABLE);
            output_set_red(DISABLE);
            work_hard();
            output_set_green(DISABLE);
            power_enter_stop();
            power_set_clock(CLOCK);
            
            /// END: To be programmed
        }
    }
}


/* -- Local function definitions
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Enables all used peripherals.
 */
static void enable_peripherals(void)
{
    PWR_ENABLE();
    GPIOA_ENABLE();
    GPIOB_ENABLE();
    GPIOC_ENABLE();
    GPIOD_ENABLE();
    GPIOE_ENABLE();
    GPIOG_ENABLE();
    SYSCFG_ENABLE();
}

/**
 *  \brief  Set unused Pins as output.
 */
static void init_gpio(void)
{
    /* Initialize gpio pin as output */
    hal_gpio_output_t gpio_output;
  
    gpio_output.out_speed = HAL_GPIO_OUT_SPEED_100MHZ;
    gpio_output.out_type = HAL_GPIO_OUT_TYPE_PP;
    gpio_output.pupd = HAL_GPIO_PUPD_NOPULL;
  
    /* Init pins of GPIOA as output. Pins 0xF000 are used for debugging */
    gpio_output.pins = (0x0FFE << 0u);
    hal_gpio_init_output(GPIOA, gpio_output);
  
    /* Init pins of GPIOB as output */
    gpio_output.pins = (0xDFFF << 0u);
    hal_gpio_init_output(GPIOB, gpio_output);
    
    /* Init pins of GPIOC as output */
    gpio_output.pins = (0xFFDF << 0u);
    hal_gpio_init_output(GPIOC, gpio_output);
  
    /* Init pins of GPIOD as output */
    gpio_output.pins = (0xFFFF << 0u);
    hal_gpio_init_output(GPIOD, gpio_output);
    
    /* Init pins of GPIOE as output */
    gpio_output.pins = (0xFFFF << 0u);
    hal_gpio_init_output(GPIOE, gpio_output);
  
}

