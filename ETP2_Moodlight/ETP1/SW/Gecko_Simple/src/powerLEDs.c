
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
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_adc.h"
#include "em_dac.h"
#include "em_timer.h"

// Own written files
#include "pwm.h"
#include "powerLEDs.h"
#include "signalLEDs.h"
#include "dac.h"
#include "globals.h"

/******************************************************************************
 * Defines and variables
 *****************************************************************************/

int32_t PWR_value[PWR_SOLUTION_COUNT] = { 0, 0, 0 }; ///< Actual set points for values

/** ports and pins for LED drivers (with number of solution) */
#define PWR_0_PWM_PORT		gpioPortC		///< Port of PWM for solution 0
#define PWR_0_PWM_PIN			4			///< Pin of PWM for solution 0
#define PWR_1_DAC_PORT		gpioPortB		///< Port of DAC for solution 1
#define PWR_1_DAC_PIN			12			///< Pin of DAC for solution 1
#define PWR_2_ADC_PORT		gpioPortD		///< Port of ADC for solution 2
#define PWR_2_ADC_PIN			0			///< Pin of ADC for solution 2
#define PWR_2_FET_PORT		gpioPortD		///< Port of FET for solution 2
#define PWR_2_FET_PIN			1			///< Pin of FET for solution 2

/** Avoid rounding issues in intermediate calculations
 * by left-shifting input values first,
 * then do the calculations
 * and finally right-shifting the output values.
 * @n PWR_conversion_shift 12 is equivalent to fixed point calculation in Q19.12
 * @warning Avoid underflow and overflow of intermediate results!
 * @n Range of int32_t = -2'147'483'648 ... 2'147'483'647 */
#define PWR_conversion_shift		12

/** Conversion factor from user-interface-value to microcontroller-output.
 * Use integer data types only and right-shifts instead of divisions
 * for faster code execution (divisions are very time consuming).
 * @n output_value = (PWR_value * PWR_conversion_output) >> PWR_conversion_shift */
const int32_t PWR_conversion_output[PWR_SOLUTION_COUNT] = { 0, 0, 0};

/** Conversion factor from microcontroller-input user-interface-value.
 * Use integer data types only and right-shifts instead of divisions
 * for faster code execution (divisions are very time consuming).
 * @n PWR_value = (input_value * PWR_conversion_input) >> PWR_conversion_shift */
const int32_t PWR_conversion_input[PWR_SOLUTION_COUNT] = { 0, 0, 0};

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
		if (value > PWR_MAXVALUE[solution]) { value = PWR_MAXVALUE[solution]; }	// value should be under max Current
		PWR_value[solution] = value;
		switch (solution) {										// Refresh value
		case 0:													// Solution 0 PWM with f=400Hz
			PWM_LETIMER0_change(value);
			break;
		case 1: 												// Solution 1 DAC
			DAC0_write(value);
			break;
		case 2:													// Solution 2 PWM with 50kHz and ADC
			break;												// Empty because of PI Controller
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
