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
void PWM_LETIMER0_WHITE_change(uint32_t);
void PWM_LETIMER0_start();
void PWM_LETIMER0_stop();
void PWM_TIMER0_RED_change(uint32_t);
void PWM_TIMER0_GREEN_change(uint32_t);
void PWM_TIMER0_BLUE_change(uint32_t);
void PWM_TIMER0_start();
void PWM_TIMER0_stop();
void PWM_FadeRGBLight();
void PWM_StroboRGBWLight();

#endif
