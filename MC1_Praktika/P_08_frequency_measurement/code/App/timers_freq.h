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
 * -- Description : Interface timers_freq.c for module frequency
 * --               measure and multipier timers.
 * --
 * --
 * -- $Revision: 374 $
 * -- $Id: timers_freq.h 373 2018-10-25 08:16:43Z gruj $
 * --------------------------------------------------------------- */

#ifndef _TIMERS_FREQ
#define _TIMERS_FREQ

#include <stdint.h>

#define TIM3_PRESCALER 50u

/**
 *  \brief  Initialize timer 3 in output mode mapped to GPIO PA6.
 *          The timer runs in toggle mode.
 *          After inititialization the autoreload value can be set 
 *          by the potentiometer. A value between 1 and 65536. Like this
 *          the frequency can by changed depending on the potentiometer.
 */
void init_timer_3(void);


/**
 *  \brief  Init a timer in input capture mode.
            Setup timer 1 as 16 bit up counter with external trigger.
 *          Input GPIO PA8 is used to trigger a capture from counter
 *          into capture compare register 1.
 *          Configure timer in reset mode, so the counter register is
 *          reseted (cleared) after a capture.
 *  \param  prescaler : prescaler for the clock source (84 MHz).
 */
void init_measure_timer(uint16_t prescaler);


/**
 *  \brief  Initialize timer 8 in output mode mapped to GPIO PA5.
 *          The timer runs in toggle mode.
 *          After inititialization the prescaler can be divided by the
 *          hex switch value, by calling set_prescaler_freq_mul(). Like this
 *          the frequency can multiplied by the value on the hex switch.
 */
void init_frequency_multiplier(void);


/**
 *  \brief  Returns the newest capture value of timer 1.
 *  \return Current capture value of timer 1.
 */
uint32_t get_capture_value(void);


/**
 * \brief   Read out hex switch and divide the value by standard
 *          prescaler of timer (init prescaler value).
 */
void set_prescaler_freq_mul(void);



/* end header ----------------------------------------------------- */
#endif
