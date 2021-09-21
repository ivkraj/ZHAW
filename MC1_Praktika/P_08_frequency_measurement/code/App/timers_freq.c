/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 lab - Frequenzmessung
 * -- Description : Implementation of module timers frequency.
 * --               Init timer 1 with an external capture channel on
 * --               GPIO PA8. At initialization the timer sets a prescaler
 * --               for the internal clock source (84 MHz).
 * --               Init timer 8 in output compare mode.
 * --               Outputpin is GPIO PA5.
 * --               At run time the prescaler of timer 8 can be set
 * --               depending on the hex switch. This allows setting
 * --               the timer speed. The multiplier factor (hex switch value)
 * --               is displayed on 7seg.
 * --               
 * --
 * -- $Revision: 946 $
 * -- $Id: timers_freq.c 945 2018-10-25 08:42:05Z gruj $
 * --------------------------------------------------------------- */

#include <stdint.h>
#include "reg_ctboard.h"
#include "timers_freq.h"
#include "hal_timer.h"
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "hal_ct_seg7.h"

#define LED_GREEN (0x1 << 13u)


/* module-wide global variables */
static volatile uint8_t multiplier;
static volatile uint16_t prescaler_tim1;
/* memory for capture value */
static volatile uint32_t capture_value = 0;

/* prototypes for internal functions */
static uint8_t read_display_hex_switch(void);


/*
 * See header file
 */
void init_timer_3(void){
  
    hal_gpio_output_t gpio;
    hal_timer_base_init_t timer_base;
    hal_timer_output_init_t timer_output;
  
    /* enable peripheral clock for timer and gpio */  
    GPIOA_ENABLE();
    TIM3_ENABLE(); 
  
    /* select alternate function mode to use gpio for timer purpose.
     * gpio PA6 is directly mapped to timer 3 CH1 
     * init PA6 as an output pin for timer 3 (output compare mode)
     */
    gpio.pins = (0x1 << 6u);
    gpio.pupd = HAL_GPIO_PUPD_DOWN;
    gpio.out_speed = HAL_GPIO_OUT_SPEED_100MHZ;
    gpio.out_type = HAL_GPIO_OUT_TYPE_PP;
    hal_gpio_init_alternate(GPIOA, HAL_GPIO_AF_TIM3, gpio);
  
  
    /* Init timer 3 as 16 bit downcounter
       - set autoreload value
       - set the prescaler for timer 3
       - set timer 3 in continous mode  
         => at update event counter restarts
       - set timer 3 as a downcouter to avoid counter overrun
       - set to master reset mode
         (not important, since timer 3 in not in master mode)
       - init basic timer functionality
     */
    timer_base.count = 0xFFFF;
    timer_base.prescaler = TIM3_PRESCALER;
    timer_base.run_mode = HAL_TIMER_RUN_CONTINOUS;
    timer_base.mode = HAL_TIMER_MODE_DOWN;
    timer_base.master_mode = HAL_TIMER_MASTER_RESET;
    hal_timer_init_base(TIM3, timer_base);
    
    
    /* Configure timer 3 in output compare mode 
       - set the value in compare reg (CCR1) to 0 
       - set the output reference signal OC1REF in toggle mode
         each time when CNT = CCR1 => OC1REF toggles
       - set output polarity of OC1 to active high
       - enable compare mode on specified channel
       - init timer 3 in output mode
     */
    timer_output.ccr_value = 0u;
    timer_output.mode = HAL_TIMER_OCMODE_TOGGLE;
    timer_output.polarity = HAL_TIMER_POLARITY_HIGH;
    timer_output.output_state = ENABLE;
    hal_timer_init_output(TIM3, HAL_TIMER_CH1, timer_output);
    
}


/*
 * See header file
 */
void init_measure_timer(uint16_t prescaler)
{
    hal_gpio_output_t gpio;
    hal_timer_base_init_t timer_base;
    hal_timer_input_init_t timer_input;
    hal_timer_clock_init_t timer_clock;
    
    /* store parameter to module-wide global variable */
    prescaler_tim1 = prescaler;

    /* enable peripheral clock for timer 1 and gpio */
    TIM1_ENABLE();
    GPIOA_ENABLE();

  
    /* select alternate function mode to use gpio port for timer purpose.
     * gpio PA8 is directly mapped to timer 1 CH1.
     * althought PA8 is initialized as output pin, timer 1 can use 
     * PA8 either on CH1 as an output pin (for output compare mode)
     * or as an input pin (for input capture mode).
     */
    gpio.pins = (0x1 << 8u);
    gpio.pupd = HAL_GPIO_PUPD_DOWN;
    gpio.out_speed = HAL_GPIO_OUT_SPEED_100MHZ;
    gpio.out_type = HAL_GPIO_OUT_TYPE_PP;
    hal_gpio_init_alternate(GPIOA, HAL_GPIO_AF_TIM1, gpio);
  
  
    /* enable counter reset at capture
       - select internal clock source (CK_INT = 84MHz) as counter source
       - select the filtered timer input 1 (TI1FP1) as the 
         trigger input source for the trigger input (TRGI)
       - trigger input (TRGI) resets the counter of timer 1 -> slave mode
       - init timer 1 as reset at capture
     */
    timer_clock.source = HAL_TIMER_CLKSRC_INT;
    timer_clock.trg_source = HAL_TIMER_TRGSRC_TI1FP;
    timer_clock.slave_mode = HAL_TIMER_SLAVE_RESET;
    hal_timer_init_clock(TIM1, timer_clock);
    
    
    /* configure and enable the input capture channel 1 (IC1)
       - select the direct pin mapping, so CH1 is mapped to 
         timer input 1 (TI1)
       - no sampling filter ist needed on TI1
       - select TI1FP1 as input source for caption,
         such that the rising edge of TI1 triggers a caption
       - no prescaler is needed
       - enable capture mode on specified channel
       - init timer 1 as input capture     
     */
    timer_input.input = HAL_TIMER_IC_SEL_DIRECT;
    timer_input.filter.mode = HAL_TIMER_FILTER_OFF;
    timer_input.filter.polarity = HAL_TIMER_POLARITY_HIGH;
    timer_input.filter.prescaler = HAL_TIMER_PSC_OFF;
    timer_input.input_state = ENABLE;
    hal_timer_init_input(TIM1, HAL_TIMER_CH1, timer_input);
    

    /* init timer 1 as 16 bit upcounter 
     *    -> use hal_timer_init_base()
     * enable interrupt
     *    -> use hal_timer_irq_set()
     */

    /// STUDENTS: To be programmed
		
    timer_base.count = 0xFFFF;
    timer_base.prescaler = prescaler_tim1;
    timer_base.run_mode = HAL_TIMER_RUN_CONTINOUS;
    timer_base.mode = HAL_TIMER_MODE_UP;
    timer_base.master_mode = HAL_TIMER_MASTER_RESET;

    hal_timer_init_base(TIM1, timer_base);
    hal_timer_irq_set(TIM1, HAL_TIMER_IRQ_CC1, ENABLE);

    /// END: To be programmed
}


/*
 * See header file
 */
void init_frequency_multiplier(void)
{
    hal_gpio_output_t gpio;
    hal_timer_base_init_t timer_base;
    hal_timer_output_init_t timer_output;

    /* enable peripheral clock for timer and gpio */
    GPIOA_ENABLE();
    TIM8_ENABLE();

  
    /* select alternate function mode to use gpio for timer purpose.
     * gpio PA5 is directly mapped to timer 8 CH1N 
     * (CH1N can be used only for output purpose)
     * init PA5 as an output pin for timer 8 (output compare mode)
     */
    gpio.pins = (0x1 << 5u);
    gpio.pupd = HAL_GPIO_PUPD_DOWN;
    gpio.out_speed = HAL_GPIO_OUT_SPEED_100MHZ;
    gpio.out_type = HAL_GPIO_OUT_TYPE_PP;
    hal_gpio_init_alternate(GPIOA, HAL_GPIO_AF_TIM8, gpio);

  
    /* Init timer 8 as 16 bit downcounter
     *    -> use init function hal_timer_init_base()
     *    -> init all elements of the defined struct timer_base
     */
     
    /// STUDENTS: To be programmed
		
    timer_base.count = 0xFFFF;
    timer_base.prescaler = prescaler_tim1;
    timer_base.run_mode = HAL_TIMER_RUN_CONTINOUS;
    timer_base.mode = HAL_TIMER_MODE_DOWN;
    timer_base.master_mode = HAL_TIMER_MASTER_RESET;
	
    hal_timer_init_base(TIM8, timer_base);

    /// END: To be programmed


    
    /* Configure timer 8 in output compare mode
     *    -> use init function hal_timer_init_output()
     *    -> init all elements of the defined struct timer_output
     *    -> toggle the output if counter value reaches ccr value
     */
     
    /// STUDENTS: To be programmed
		
    timer_output.ccr_value = 0u;
    timer_output.mode = HAL_TIMER_OCMODE_TOGGLE;
    timer_output.polarity = HAL_TIMER_POLARITY_HIGH;
    timer_output.output_state = ENABLE;
	
    hal_timer_init_output(TIM8, HAL_TIMER_CH1N, timer_output);

    /// END: To be programmed

    /* Disable master mode on timer 8 */
    TIM8->SMCR &= ~(0x7 << 0u);
}

/*
 * See header file
 */
uint32_t get_capture_value(void)
{
    return capture_value;
}


/*
 * See header file
 */
void set_prescaler_freq_mul(void)
{
    /// STUDENTS: To be programmed
	
		multiplier = read_display_hex_switch();
		hal_timer_prescaler_write(TIM8, prescaler_tim1 / (multiplier * 2u));
	
    /// END: To be programmed
}


/**
 *  \brief  Interrupt service routine for timer 1.
 *          Read out the new capture value and save.
 *          Load capture value into reload reg of timer 8.
 */
extern void TIM1_CC_IRQHandler(void)
{
    /// STUDENTS: To be programmed
	
    hal_timer_irq_clear(TIM1, HAL_TIMER_IRQ_CC1);
    capture_value = hal_timer_compare_read(TIM1, HAL_TIMER_CH1);
    hal_timer_reload_write(TIM8, capture_value);

    /// END: To be programmed
}


/* ------------------- internal functions ------------------- */

/**
 *  \brief  read and return the hex switch value and show
 *          the value on seven segment display.
 *  \return hex switch value from 1 to 16
 */
static uint8_t read_display_hex_switch(void)
{
    /// STUDENTS: To be programmed
		
    uint8_t hex_val = (CT_HEXSW & 0x0F)+1;
    uint8_t hex_sw = hex_val;
    uint16_t bcd;

    bcd = (hex_val%10);
    hex_val /= 10;
    bcd |= (hex_val%10) << 4;
        
    hal_ct_seg7_bin_write(bcd);

    return hex_sw;


    /// END: To be programmed
}

