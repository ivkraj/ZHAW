/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module adc.
 * --
 * --
 * -- $Id: adc.c 4734 2019-03-11 12:55:21Z ruan $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <reg_stm32f4xx.h>

/* user includes */
#include "adc.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define PERIPH_GPIOF_ENABLE (0x00000020)
#define PERIPH_ADC3_ENABLE  (0x00000400)

/* Configuring pin for ADC: PF.6 */
#define GPIOF_MODER_ANALOG (0x3 << 12)


/* ADC configuration
 * ------------------------------------------------------------------------- */

#define ADC_CCR_NORMALCLK   (0x0 << 16u)    // APB2 / 2 -> 21 MHz
#define ADC_CCR_NODMA       (0x0 << 14u)    // DMA disabled
#define ADC_CCR_DELAY       (0x0 << 8u)     // Delay 5 cycles
#define ADC_CCR_INDEPENDENT (0x0 << 0u)     // ADC independent mode

#define ADC_MASK_12BIT     (0x0fff)
#define ADC_MASK_10BIT     (0x03ff)
#define ADC_MASK_8BIT      (0x00ff)
#define ADC_MASK_6BIT      (0x003f)

#define ADC_CR1_NOSCAN     (0x0 << 8u)      // No scan -> only one channel

#define ADC_CR2_NOEXTTRG   (0x0 << 28u)     // No external trigger
#define ADC_CR2_ALIGNRIGHT (0x0 << 11u)     // Align data on right
#define ADC_CR2_SINGLE     (0x0 << 1u)      // Single conversion mode

#define ADC_SMPR2_3CYCLES  (0x0 << 12u)     // Conversion cycles
#define ADC_SQR1_SINGLE    (0x0 << 20u)     // 1 conversion
#define ADC_SQR3_FIRST     (0x4 << 0u)      // ADC ch. 4, first in sequence

#define ADC_ADON           (0x1 << 0u)
#define ADC_END_OF_CONV    (0x1 << 1u)
#define ADC_RES_MASK       (0x3 << 24u)     // Mask resolution bits
#define ADC_START          (0x1 << 30u)

#define FILTER_SIZE        16u

#define NORMALIZED_FULLSCALE (1000u)

/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 *  See header file
 */
void adc_init(void)
{
    /* Enable peripheral clocks */
    RCC->AHB1ENR |= PERIPH_GPIOF_ENABLE;
    RCC->APB2ENR |= PERIPH_ADC3_ENABLE;

    /* Configure PF.6 as input */
    GPIOF->MODER |= GPIOF_MODER_ANALOG;


    /* ADC common init */
    ADCCOM->CCR = ADC_CCR_NORMALCLK |
                  ADC_CCR_NODMA |
                  ADC_CCR_DELAY |
                  ADC_CCR_INDEPENDENT;

    /* Configure ADC3 scan conversion mode and default resolution */
    ADC3->CR1 = ADC_RES_6BIT |
                ADC_CR1_NOSCAN;

    /* Configure ADC3 other settings */
    ADC3->CR2 = ADC_CR2_NOEXTTRG |
                ADC_CR2_ALIGNRIGHT |
                ADC_CR2_SINGLE;

    /* Configure ADC3 channel 4 */
    ADC3->SMPR2 = ADC_SMPR2_3CYCLES;
    ADC3->SQR1 = ADC_SQR1_SINGLE;
    ADC3->SQR3 = ADC_SQR3_FIRST;

    /* Enable ADC3 */
    ADC3->CR2 |= ADC_ADON;

}


/*
 *  See header file
 */
uint32_t adc_get_value(adc_resolution_t resolution)
{
    uint16_t adc_value;
    uint32_t normalized;
  
    /* Set resolution */
    ADC3->CR1 &= ~ADC_RES_MASK;
    ADC3->CR1 |= resolution;

    /* Start conversion */
    ADC3->CR2 |= ADC_START;

    /* Wait for conversion to finish */
    while (!(ADC3->SR & ADC_END_OF_CONV));

    /* Read converted value */
    adc_value = ADC3->DR;
    
    adc_value &= ADC_MASK_6BIT;
  
    normalized = adc_filter_value(adc_value);

    normalized = ((normalized * NORMALIZED_FULLSCALE)/0x3f);
 

    return normalized;
}


/*
 *  See header file
 */
uint16_t adc_filter_value(uint16_t adc_value)
{
    uint16_t filtered_value = 0;

    static uint16_t adc_array[FILTER_SIZE];
    uint8_t i;

    /* Shift values in array */
    for (i = 1; i < FILTER_SIZE; i++) {
        adc_array[i - 1] = adc_array[i];
        filtered_value += adc_array[i];
    }

    /* Add newest value to array */
    adc_array[FILTER_SIZE - 1] = adc_value;
    filtered_value += adc_value;

    filtered_value /= FILTER_SIZE;

    return filtered_value;
}
