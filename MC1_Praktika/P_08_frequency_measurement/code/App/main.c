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
 * -- Description :
 * --   #Reciprocal Counting 4.1 (Input GPIO Port A Pin 8):
 * --    This program counts with an internal fast clock using
 * --    timer 1. Timer 1 is setup in input capture mode.
 * --    The external fequency on PA 8 triggers on rising edge a
 * --    capture. Timer 1 fires a capture interrupt.
 * --    Now the capture value can be read and the frequency can
 * --    be calculated and displayed on lcd.
 * --   #Frequency multiplier 4.2 (Output GPIO Port A Pin 5):
 * --    With an interrupt the capture
 * --    value of timer 1 is loaded into timer 8 autoreload register.
 * --    By multiplying (dividing) the prescaler from timer 1 with the
 * --    hexswitch value and setting them as prescaler of timer 8, the
 * --    timer 8 now generates a multiplied frequency.
 * --    Timer 8 is set in output mode channel 1N, so the multiplied
 * --    frequency can be shown on the oscilloscope at gpio PA5.
 * --
 * -- $Id: main.c 945 2016-10-17 13:14:05Z feur $
 * --------------------------------------------------------------- */

/* user includes */
#include <stdint.h>
#include "reg_ctboard.h"
#include "display_freq.h"
#include "hal_timer.h"
#include "timers_freq.h"
#include "adc.h"

/* user defines */
#define HCLK                   ((uint32_t)84000000)
#define PRESCALER              140u           // prescaler timer 1
#define COUNTER_CLOCK_TIM1     (uint32_t)(HCLK / PRESCALER)
#define COUNTER_CLOCK_TIM3     (uint32_t) (HCLK / TIM3_PRESCALER)
#define TIMER_WAIT_FOR_LCD     0x9ffff
#define RED                    0xffff
#define GREEN                  0xffff
#define BLUE                   0xffff

/* ------------------------------------------------------------------
 * Main
 * --------------------------------------------------------------- */

int main(void)
{
    /* local vars */
    /// STUDENTS: To be programmed
    uint32_t freq_tim1;


    /// END: To be programmed
  
    uint32_t wait = 0;
    uint32_t freq_tim3;
    uint32_t reload_val_tim3;

    /* init timer 3, adc, timer 1, timer 8 and LCD */  
    init_timer_3();
    adc_init();
    init_measure_timer(PRESCALER);
    init_frequency_multiplier();
    init_lcd_background_color(RED,GREEN,BLUE);
  
    /* start counter of timer 3, timer 1 and timer 8 */
    hal_timer_start(TIM3);
    hal_timer_start(TIM1);
    hal_timer_start(TIM8);

    while (1) {
      
        /* main loop */
      
        /* read POT1 value and write it to ARR of timer 3.
         * calculate and then display the frequency on LCD */
        reload_val_tim3 = adc_get_value(ADC_RES_6BIT);
        hal_timer_reload_write(TIM3, reload_val_tim3+1);
        freq_tim3 = (COUNTER_CLOCK_TIM3 / (reload_val_tim3+1))/2;
        write_tim3_freq_to_lcd(freq_tim3);
        
      
        /* Task 4.1
         *  -> get the newest capture value of timer 1
         *  -> calculate the frequency from the capture value
         *  -> call function to update frequency on lcd
         *
         * Task 4.2
         *  -> before lcd update: call function to set  
         *     prescaler of timer 8 depending on hex switch
         */
      
        /// STUDENTS: To be programmed
        freq_tim1 = COUNTER_CLOCK_TIM1/get_capture_value();
        write_tim1_freq_to_lcd(freq_tim1);
				
        set_prescaler_freq_mul();

        write_tim1_freq_to_lcd(freq_tim1);


        /// END: To be programmed
      
        /*wait a little due to lcd */
        for (wait = 0; wait < TIMER_WAIT_FOR_LCD; wait++) ;
    }
}


