/** ***************************************************************************
 * @file
 * @brief See powerLEDs.c
 *****************************************************************************/ 

#ifndef PWRLEDS_H_
#define PWRLEDS_H_

/******************************************************************************
 * Defines
 *****************************************************************************/

#define PWR_SOLUTION_COUNT		12			///< number of power LED drivers
#define RGBW_COLOR_VALUES		4			///< number of Color values

uint32_t RGBW_Color[RGBW_COLOR_VALUES];		///< Array for RGB Values

/******************************************************************************
 * Functions
 *****************************************************************************/

void PWR_set_value(uint32_t channel, int32_t value);
uint32_t PWR_get_value(uint32_t channel);
void PWR_init(void);

#endif
