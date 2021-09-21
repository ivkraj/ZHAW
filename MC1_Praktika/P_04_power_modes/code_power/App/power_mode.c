/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module power_mode.
 *
 *  $Id: power_mode.c 1603 2018-10-10 06:31:28Z kjaz $
 * ------------------------------------------------------------------------- */


/* Standard includes */
#include <reg_stm32f4xx.h>
#include <hal_rcc.h>
#include <hal_pwr.h>
#include <hal_gpio.h>

/* User includes */
#include "power_mode.h"
#include "user_button.h"


/* -- Macros
 * ------------------------------------------------------------------------- */



/* -- Local function declarations
 * ------------------------------------------------------------------------- */

static void fill_init_structs(hal_rcc_pll_init_t *pll_init,
                              hal_rcc_clk_init_t *clk_init,
                              const power_clk_t clock);


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 *  See header file
 */
void power_init(void)
{
    /* Enable debugging while in sleep and stop mode */
    DBG->CR |= (0x1 << 0u);
    DBG->CR |= (0x1 << 1u);
}


/*
 *  See header file
 */
void power_enter_sleep(void)
{
    /*
     * There are no hal functions to enter sleep,
     * please read/write directly to the registers.
     */

    /// STUDENTS: To be programmed
    
    SCB->SCR &= ~(0x01 << 2u);  // disable Deep Sleep
    __asm volatile ("wfi");     // enter Sleep Mode

    /// END: To be programmed
}


/*
 *  See header file
 */
void power_enter_stop(void)
{
    /*
     * There are no hal functions to enter deep sleep,
     * please read/write directly to the registers.
     */

    /// STUDENTS: To be programmed
        
    PWR->CR &= ~(0x1 << 1u);    // Clear PDDS -> Enter STOP when DEEPSLEEP

    PWR->CR &= ~(0x1 << 0u);    // Clear LPDS -> Main voltage regulator when STOP
 
    SCB->SCR |= (0x1 << 2u);    // Set SLEEPDEEP bit to enter DEEPSLEEP
 
    __asm volatile ("wfi");     // Enter low power mode
 
    SCB->SCR &= ~(0x1 << 2u);   // Reset SLEEPDEEP bit after wakeup
    
    //power_set_clock(CLOCK_120MHZ);  // Enable Clock

    /// END: To be programmed
}


/*
 *  See header file
 */
void power_set_clock(const power_clk_t clock)
{
    hal_rcc_pll_init_t pll_init;
    hal_rcc_clk_init_t clk_init;

    /* Fill init structs based on speed */
    fill_init_structs(&pll_init, &clk_init, clock);

    /* Reset */
    RCC->CR |= 0x00000001;      // Set HSION bit first -> keep cpu running
    RCC->CFGR &= 0x1111111c;    // Set system clock to HSI
    RCC->CR &= 0xeaf6ffff;      // Reset HSEON, CSSON, PLLON, PLLI2S,
                                //   PLLSAI bits (STM32F42xx/43xx)
    RCC->CR &= 0xfffbffff;      // Reset HSEBYP bit
    RCC->CFGR = 0u;             // Reset RCC->CFGR to default values
    RCC->PLLCFGR = 0x24003010;  // Reset RCC->PLLxCFGR to default values

    /* Enable HSE oscillator and proceed if ok */
    if (hal_rcc_set_osc(HAL_RCC_OSC_HSE, ENABLE)) {
        /* Select regulator voltage output Scale 1 mode */
        PWR->CR |= (0x3 << 14u);

        /* Configure PLL */
        hal_rcc_setup_pll(HAL_RCC_OSC_PLL, pll_init);

        /* Enable PLL */
        hal_rcc_set_osc(HAL_RCC_OSC_PLL, ENABLE);

        /* Enable overdrive to allow system clock >= 168 MHz */
        hal_pwr_set_overdrive(ENABLE);

        /* Setup system clock */
        hal_rcc_setup_clock(clk_init);
    } else {
        /* If HSE fails to start-up, the application will have wrong clock con-
           figuration. User can add here some code to deal with this error */
    }

    /* Configure Flash prefetch, Instruction cache, Data cache
     * and wait state */
    FLASH->ACR = 0x00000705;
}


/* -- Local function definitions
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Fills the supplied init structs (for PLL and system clocks).
 *  \param  pll_init : Pointer to PLL init struct.
 *  \param  clk_init : Pointer to system clock init struct.
 *  \param  clock : Select desired system clock.
 */
static void fill_init_structs(hal_rcc_pll_init_t *pll_init,
                              hal_rcc_clk_init_t *clk_init,
                              const power_clk_t clock)
{
    switch (clock) {
        //default:
        case CLOCK_168MHZ:
            /* PLL config */
            pll_init->source = HAL_RCC_OSC_HSE; // Use HSE (8 MHz) as PLL source
            pll_init->m_divider = 4u;   // VCOin: HSE / m_divider -> 2 MHz
            pll_init->n_factor = 168u;  // VCOout: VCOin * n_factor -> 336 MHz
            pll_init->p_divider = 2u;   // PLL: VCOout / p_divider -> 168 MHz
            pll_init->q_divider = 7u;   // 48 MHz out: VCOout / q_div -> 48 MHz
            /* Clock config */
            clk_init->osc = HAL_RCC_OSC_PLL; // Use PLL as SYSCLK source
            clk_init->hpre = HAL_RCC_HPRE_2; // HCLK: SYSCLK / hpre -> 84 MHz
            clk_init->ppre1 = HAL_RCC_PPRE_2; // PCLK1: HCLK / ppre1 -> 48 MHz
            clk_init->ppre2 = HAL_RCC_PPRE_2; // PCLK2: HCLK / ppre2 -> 48 MHz
            break;

        // STUDENTS: To be programmed
        case CLOCK_120MHZ:
            /* PLL config */
            pll_init->source = HAL_RCC_OSC_HSE; // Use HSE (8 MHz) as PLL source
            pll_init->m_divider = 4u;   // VCOin: HSE / m_divider -> 2 MHz
            pll_init->n_factor = 240u;  // VCOout: VCOin * n_factor -> 480 MHz
            pll_init->p_divider = 4u;   // PLL: VCOout / p_divider -> 120 MHz
            pll_init->q_divider = 10u;   // 48 MHz out: VCOout / q_div -> 48 MHz
            /* Clock config */
            clk_init->osc = HAL_RCC_OSC_PLL; // Use PLL as SYSCLK source
            clk_init->hpre = HAL_RCC_HPRE_2; // HCLK: SYSCLK / hpre -> 84 MHz
            clk_init->ppre1 = HAL_RCC_PPRE_2; // PCLK1: HCLK / ppre1 -> 48 MHz
            clk_init->ppre2 = HAL_RCC_PPRE_2; // PCLK2: HCLK / ppre2 -> 48 MHz
            break;
        case CLOCK_84MHZ:
            /* PLL config */
            pll_init->source = HAL_RCC_OSC_HSE; // Use HSE (8 MHz) as PLL source
            pll_init->m_divider = 4u;   // VCOin: HSE / m_divider -> 2 MHz
            pll_init->n_factor = 168u;  // VCOout: VCOin * n_factor -> 336 MHz
            pll_init->p_divider = 4u;   // PLL: VCOout / p_divider -> 84 MHz
            pll_init->q_divider = 7u;   // 48 MHz out: VCOout / q_div -> 48 MHz
            /* Clock config */
            clk_init->osc = HAL_RCC_OSC_PLL; // Use PLL as SYSCLK source
            clk_init->hpre = HAL_RCC_HPRE_2; // HCLK: SYSCLK / hpre -> 84 MHz
            clk_init->ppre1 = HAL_RCC_PPRE_2; // PCLK1: HCLK / ppre1 -> 48 MHz
            clk_init->ppre2 = HAL_RCC_PPRE_2; // PCLK2: HCLK / ppre2 -> 48 MHz
            break;
        case CLOCK_60MHZ:
            /* PLL config */
            pll_init->source = HAL_RCC_OSC_HSE; // Use HSE (8 MHz) as PLL source
            pll_init->m_divider = 4u;   // VCOin: HSE / m_divider -> 2 MHz
            pll_init->n_factor = 120u;  // VCOout: VCOin * n_factor -> 240 MHz
            pll_init->p_divider = 4u;   // PLL: VCOout / p_divider -> 60 MHz
            pll_init->q_divider = 5u;   // 48 MHz out: VCOout / q_div -> 48 MHz
            /* Clock config */
            clk_init->osc = HAL_RCC_OSC_PLL; // Use PLL as SYSCLK source
            clk_init->hpre = HAL_RCC_HPRE_2; // HCLK: SYSCLK / hpre -> 84 MHz
            clk_init->ppre1 = HAL_RCC_PPRE_2; // PCLK1: HCLK / ppre1 -> 48 MHz
            clk_init->ppre2 = HAL_RCC_PPRE_2; // PCLK2: HCLK / ppre2 -> 48 MHz
            break;

        /// END: To be programmed
    }
}

