/** ***************************************************************************
 * @file
 * @brief See adc.c
 *****************************************************************************/

#ifndef _PWM_h_
#define _PWM_h_

/******************************************************************************
 * Functions
 *****************************************************************************/

void PWM_init();
void PWM_LETIMER0_change(uint32_t);
void PWM_LETIMER0_start();
void PWM_LETIMER0_stop();
void PWM_TIMER0_change(uint32_t);
void PWM_TIMER02_start();
void PWM_TIMER02_stop();

#endif
