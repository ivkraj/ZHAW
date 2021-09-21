
 /** ***************************************************************************
 * @file
 * @brief Power LEDs
 *
 * All the functions needed for the current control of the power LEDs
 * for solution 0 and solution 1.
 *
 * Prefix: PWR
 *
 * Board:  Starter Kit EFM32-G8XX-STK
 * Device: EFM32G890F128 (Gecko)
 *
 * @author Hanspeter Hochreutener (hhrt@zhaw.ch)
 * @author Lars Mueggler (muegglar@students.zhaw.ch)
 * @date 11.12.2019
 * @version 1.0.0
 *****************************************************************************/ 

/******************************************************************************
 * Includes
 *****************************************************************************/

// System files
#include <i2c_com.h>
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_adc.h"
#include "em_dac.h"
#include "em_timer.h"

// Own written files
#include "pwm.h"
#include "powerLEDs.h"
#include "signalLEDs.h"
#include "globals.h"
#include "userinterface.h"
#include "i2c_com.h"
#include "externSensors.h"

/******************************************************************************
 * Defines and variables
 *****************************************************************************/

int32_t PWR_value[PWR_SOLUTION_COUNT] = { 0, 0, 0, 0, 0, 0, 0}; ///< Actual set points for values

/** ports and pins for LED drivers (with number of solution) */
#define PWR_0_PWM_PORT		gpioPortC		///< Port of PWM for solution 0
#define PWR_0_PWM_PIN			4			///< Pin of PWM for solution 0
#define PWR_1_DAC_PORT		gpioPortB		///< Port of DAC for solution 1
#define PWR_1_DAC_PIN			12			///< Pin of DAC for solution 1
#define PWR_2_ADC_PORT		gpioPortD		///< Port of ADC for solution 2
#define PWR_2_ADC_PIN			0			///< Pin of ADC for solution 2
#define PWR_2_FET_PORT		gpioPortD		///< Port of FET for solution 2
#define PWR_2_FET_PIN			1			///< Pin of FET for solution 2


/******************************************************************************
 * Functions
 *****************************************************************************/

/** ***************************************************************************
 * @brief Set the set point of the selected power LED driver.
 * @param [solution] solution number
 * @param [value] value of set point
 *
 * The LED driver current is also adjusted accordingly.
 *****************************************************************************/
void PWR_set_value(uint32_t solution, int32_t value) {
	if (solution < PWR_SOLUTION_COUNT) {						// check if solution number in valid range
		if (value < 0) { value = 0; }							// value should positive
		//if (value > PWR_MAXVALUE[solution]) { value = PWR_MAXVALUE[solution]; }	// value should be under max Current
		if (value > MAX_VALUE) { value = MAX_VALUE; }
		PWR_value[solution] = value;
		switch (solution) {										// Refresh value
		case RED:													// RED only mode
			PWM_TIMER0_RED_change(value);
			break;
		case GREEN: 												// Green only mode
			PWM_TIMER0_GREEN_change(value);
			break;
		case BLUE:													// Blue only mode
			PWM_TIMER0_BLUE_change(value);
			break;
		case WHITE:
			PWM_LETIMER0_WHITE_change(value);					// White only mode
			break;
		case TEMPERATURE:													// Temperature mode
			ES_TempToColor();
			break;
		case RED_ONLY:
			PWM_TIMER0_RED_change(value);
			PWM_TIMER0_GREEN_change(0);
			PWM_TIMER0_BLUE_change(0);
			PWM_LETIMER0_WHITE_change(0);
			break;
		case GREEN_ONLY:
			PWM_TIMER0_RED_change(0);
			PWM_TIMER0_GREEN_change(value);
			PWM_TIMER0_BLUE_change(0);
			PWM_LETIMER0_WHITE_change(0);
			break;
		case BLUE_ONLY:
			PWM_TIMER0_RED_change(0);
			PWM_TIMER0_GREEN_change(0);
			PWM_TIMER0_BLUE_change(value);
			PWM_LETIMER0_WHITE_change(0);
			break;
		case WHITE_ONLY:
			PWM_TIMER0_RED_change(0);
			PWM_TIMER0_GREEN_change(0);
			PWM_TIMER0_BLUE_change(0);
			PWM_LETIMER0_WHITE_change(value);
			break;
		case RGBW:
			PWM_TIMER0_RED_change(RGBW_Color[0]);
			PWM_TIMER0_GREEN_change(RGBW_Color[1]);
			PWM_TIMER0_BLUE_change(RGBW_Color[2]);
			PWM_LETIMER0_WHITE_change(RGBW_Color[3]);
			break;
		case RGBFADE:
			PWM_FadeRGBLight();
			break;
		case RGBWSTROBO:
			PWM_StroboRGBWLight();
			break;
		}

	}
}

/** ***************************************************************************
 * @brief Get the set point of the selected power LED driver.
 * @param [solution] solution number
 * @return value of set point
 *****************************************************************************/
uint32_t PWR_get_value(uint32_t solution) {
	uint32_t value = 0;
	if (solution < PWR_SOLUTION_COUNT) {						// Check if solution number is in valid range
		value = PWR_value[solution];
	}
	return value;
}

/** ***************************************************************************
 * @brief Start all the power LED drivers.
 *
 * All the GPIOs are initialized and started.
 *****************************************************************************/
void PWR_init(void) {
	CMU_ClockEnable(cmuClock_GPIO, true);						// enable GPIO clock
}
