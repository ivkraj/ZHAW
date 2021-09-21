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
uint32_t const FREQ_400;										///< Count for Frequency 400 Hz	32768/400 = 81..
uint32_t const FREQ_25k;										///< Count for Frequency 25 kHz	32000000/25000=1280
uint32_t const FREQ_50k;										///< Count for Frequency 50 kHz	32000000/50000=640
uint32_t const FREQ_10k;										///< Count for Frequency 10 kHz	32000000/10000=3200
uint32_t const PWR_MAXVALUE[PWR_SOLUTION_COUNT]; ///< Max values for each solution

/******************************************************************************
 * Functions
 *****************************************************************************/

void INIT_XOclocks();
void ltostr(int32_t l, char *string);

#endif