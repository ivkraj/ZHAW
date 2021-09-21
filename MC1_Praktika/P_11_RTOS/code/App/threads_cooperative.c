/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module threads_cooperative.
 * 
 *  $Id: threads_cooperative.c 1319 2017-12-08 10:20:00Z kjaz $
 * ------------------------------------------------------------------------- */


/* Standard includes */ 
#include <cmsis_os.h>
#include <hal_gpio.h>
#include <stdio.h>

/* User includes */
#include "threads.h"
#include "uart.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define LED_GREEN           (0x1 << 13u)
#define LED_RED             (0x1 << 14u)
#define HALF_SECOND         (0x7fffff)


/* -- Variables with global scope
 * ------------------------------------------------------------------------- */


/* -- Local function declarations
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed
void toggle_green (void const *argument);
void toggle_red (void const *argument);


/// END: To be programmed
static void wait_blocking(uint32_t value);


/* -- Thread definition
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed

osThreadDef(toggle_green, osPriorityAboveNormal, 1, 0);
osThreadDef(toggle_red, osPriorityNormal, 1, 0);

/// END: To be programmed


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void threads_init(void)
{
    hal_gpio_output_t gpio;
    osThreadId id_green, id_red;

    uart_init();
  
    GPIOG_ENABLE();
    
    gpio.pins = LED_GREEN | LED_RED;
    gpio.pupd = HAL_GPIO_PUPD_UP;
    gpio.out_speed = HAL_GPIO_OUT_SPEED_100MHZ;
    gpio.out_type = HAL_GPIO_OUT_TYPE_PP;
    
    hal_gpio_init_output(GPIOG, gpio);
    
    /// STUDENTS: To be programmed      

    osKernelInitialize();
    id_green = osThreadCreate(osThread(toggle_green), NULL);
    id_red = osThreadCreate(osThread(toggle_red), NULL);
    if (id_green == NULL) {
        printf("\n\rCreation of Thread green_toggle was not successful");
    } else {
        printf("\n\rCreation of Thread green_toggle was successful");
    }

    if (id_red == NULL) {
        printf("\n\rCreation of Thread red_toggle was not successful");
    } else {
        printf("\n\rCreation of Thread red_toggle was successful");
    }  
    
    osKernelStart();


    /// END: To be programmed
}


/* -- Local function definitions
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed

void toggle_green (void const *argument) {
    while (1) {
        hal_gpio_bit_set(GPIOG, LED_GREEN);
        wait_blocking(HALF_SECOND);
        hal_gpio_bit_reset(GPIOG, LED_GREEN);
        wait_blocking(HALF_SECOND);
        osThreadYield();
    }
}

void toggle_red (void const *argument) {
    while (1) {
        hal_gpio_bit_set(GPIOG, LED_RED);
        wait_blocking(HALF_SECOND);
        hal_gpio_bit_reset(GPIOG, LED_RED);
        wait_blocking(HALF_SECOND);
        osThreadYield();
    }
}



/// END: To be programmed

/**
 *  \brief  Wait loop.
 *  \param  value : Iterations to wait. Blocks execution of other threads.
 */
static void wait_blocking(uint32_t value)
{
    for (; value > 0u; value--);
}
