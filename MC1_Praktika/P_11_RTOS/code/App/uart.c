/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module uart.
 * 
 *  $Id: uart.c 1124 2016-12-06 07:29:06Z feur $
 * ------------------------------------------------------------------------- */


/* Standard includes */ 
#include <reg_stm32f4xx.h>
#include <hal_gpio.h>

/* User includes */
#include "uart.h"


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void uart_init(void)
{
    hal_gpio_output_t gpio;
    
    GPIOA_ENABLE();
    USART1_ENABLE();
    
    gpio.pins = (0x1 << 9u);
    gpio.pupd = HAL_GPIO_PUPD_NOPULL;
    gpio.out_speed = HAL_GPIO_OUT_SPEED_100MHZ;
    gpio.out_type = HAL_GPIO_OUT_TYPE_PP;
    
    hal_gpio_init_alternate(GPIOA, HAL_GPIO_AF_USART1, gpio);    
     
    USART1->BRR |= (42000000 / 115200);   /* Set baudrate to 115'200 */
    USART1->CR1 |= (0x1 << 13u);          /* Enable UART */

    USART1->CR1 |= (0x1 << 3u);           /* Enable transmission */  
}

/*
 * See header file
 */
int stdout_putchar(int ch) {
    while(!(USART1->SR & (0x1 << 7u)));
    USART1->DR = (char) ch;
    
    return ch;
}
