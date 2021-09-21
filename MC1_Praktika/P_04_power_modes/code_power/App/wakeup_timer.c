/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module wakeup timer.
 *
 *  $Id: wakeup_timer.c 1804 2019-09-13 15:52:32Z kjaz $
 * ------------------------------------------------------------------------- */


/* Standard includes */
#include <reg_stm32f4xx.h>

/* User includes */
#include "wakeup_timer.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define TIME_OUT 0x1000
#define LSION    (0x01 << 0u)
#define LSIRDY   (0x01 << 1u)
#define WUTWF    (0x01 << 2u)


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void wakeup_init(void)
{
    uint32_t reg = 0;
    uint32_t count = 0;

    /* In LP mode the clocks might be disabled.
     * Enable LSI and wait till its ready */
    RCC->CSR |= LSION;
    do {
        reg = RCC->CSR & LSIRDY;
        count++;
    } while ((reg == 0) && (count != TIME_OUT));

    /*
     * There is no wakeup timer hal, please read/write
     * directly to the registers.
     */

    /// STUDENTS: To be programmed
    PWR->CR |= (0x01 << 8);     //Disable backup domain write protection
    
    // Disable write protection
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    
    // Enable RTC clock and select RTC source
    //RCC->CSR |= 0x01;           // Enable internal low speed oscillator    
    RCC->BDCR |= (0x02 << 8);   // Select LSI as RTC source
    RCC->BDCR |= (0x01 << 15);  // Enable RTC clock

    // Setup wakeup timer
    RTC->CR &= ~(0x01 << 10);   // Disable wakeup timer
    
    while((RTC->ISR & (0x01 << 2)) == 0){};       // Wait until wakeup timer config is allowed
    
    RTC->CR &= ~(0x07);         // Select 16 as PSC -> f = 2kHz
    RTC->WUTR = 200;            // wakeup timer = 1/2kHz * 200
    RTC->CR |= (0x01 << 14);    // Enable wakeup timer interrupt
    
    // Setup wakeup interrupt
    RTC->ISR &= ~(0x01 << 10);  // clear wakeup timer flag
    
    EXTI->RTSR |= (0x1 << 22u); // Enable Interrupt on rising edge
    EXTI->IMR |= (0x1 << 22u);  // Unmask interrupt line
    NVIC->ISER0 |= (0x01 << 3); // Setup NVIC for RTC Wakeup
    
    RTC->CR |= (0x01 << 10);    // Enable wakeup timer
    
    RTC->WPR = 0xFF;            // Reenable RTC write protection
    
    /// END: To be programmed
}

/* -- Interrupt function definitions
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed

void RTC_WKUP_IRQHandler(void) {
    RTC->ISR &= ~(0x01 << 10);  // clear wakeup timer flag
    EXTI->PR |= (0x01 << 22);   // clear pending bit of wakeup timer 
}



/// END: To be programmed
