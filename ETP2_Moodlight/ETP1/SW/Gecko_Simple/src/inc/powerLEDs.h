/** ***************************************************************************
 * @file
 * @brief See powerLEDs.c
 *****************************************************************************/ 

#ifndef PWRLEDS_H_
#define PWRLEDS_H_

/******************************************************************************
 * Defines
 *****************************************************************************/

#define PWR_SOLUTION_COUNT		3			///< number of power LED drivers

/******************************************************************************
 * Functions
 *****************************************************************************/

void PWR_set_value(uint32_t channel, int32_t value);
uint32_t PWR_get_value(uint32_t channel);
void PWR_init(void);

#endif
