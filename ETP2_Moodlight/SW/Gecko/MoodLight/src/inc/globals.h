/** ***************************************************************************
 * @file
 * @brief See globals.c
 *****************************************************************************/ 

#ifndef GLOBALS_H_
#define GLOBALS_H_



/******************************************************************************
 * Defines
 *****************************************************************************/
#include "powerLEDs.h"

/******************************************************************************
 * Constants
 *****************************************************************************/
uint32_t const FREQ_LETIMER_400;										///< Count for Frequency 400 Hz	32768/400 = 81..
uint32_t const FREQ_TIMER1_400;										///< Count for Frequency 25 kHz	32000000/400=80000
uint32_t const FREQ_TIMER2_1;										///< Count for Frequency 25 kHz	32000000/400=80000
uint32_t const MAX_VALUE;

/******************************************************************************
 * Variables
 *****************************************************************************/
uint32_t illumination_settings[3];											///< Settings for Illumination Default NotInverted[0], OFF[1]
int32_t temperature;
uint32_t illumination;
int32_t temp_values[4];
char temperatureString[16], overTempONString[16], overTempOFFString[16];
bool newIllumFlag, newTempFlag, measTempFlag, overTempFlag;
uint32_t pre_timer2_counter, timer2_interrupt_counter, pre_temp_counter;

/******************************************************************************
 * Functions
 *****************************************************************************/

void INIT_XOclocks();
void ltostr(int32_t l, char *string);
void convertTemperatureToString(int32_t temp);

#endif
