/** ***************************************************************************
 * @file
 * @brief PWM
 *
 * Initializing the timers and output pins, functions to change the duty cycles
 * and start/stop the PWM signals
 *
 * Prefix: PWM
 *
 *
 * Board:  Starter Kit EFM32-G8XX-STK
 * Device: EFM32G890F128 (Gecko)
 *
 * @authors Lars Mueggler (muegglar@students.zhaw.ch)
 * @date 27.12.2019
 * @version 1.0.0
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

// System Files
#include "em_chip.h"
#include "em_device.h"
#include "em_system.h"
#include "em_gpio.h"
#include "em_timer.h"
#include "em_letimer.h"
#include "em_cmu.h"

// Own written Files
#include "signalLEDs.h"
#include "globals.h"
#include "pwm.h"
#include "adc.h"
#include "powerLEDs.h"

/******************************************************************************
 * Defines and variables
 *****************************************************************************/

#define P 1 	///< P Value --> 2.2*0.45 = 0.99 (Ziegler-Nichols)
#define I 300 	///< I Value

/******************************************************************************
 * Functions
 *****************************************************************************/

/** ***************************************************************************
* @brief PI controller for adjusting the output current
* @n static variable are used because the controller needs the previous values
*****************************************************************************/
void PI_controller_adjust_current(){
	static uint32_t set_val;
	static uint32_t act_val;
	static int32_t diff;
	static int32_t i_val = 0;
	static int32_t p_val;
	static int32_t result;

	set_val = PWR_get_value(2);					// get Slider value of Solution 2
	act_val = ADC0_read_one();					// get actual value

	set_val = (set_val*65777)>>12;				// conversion: //4095/255=16.06 --> *2^12=65777


	diff = (int32_t) set_val - (int32_t) act_val; // calculate control deviation

	p_val = P * diff;							// calculate new P value
	i_val += (I * diff);						// calculate new I value with previous value

	if (i_val > 4095) {							// limitation to +-4095
		i_val = 4095;
	}else if (i_val < -4095) {
		i_val = -4095;
	}

	result = i_val + p_val;						// generate new error compensation value
	result = ( result * 640) >> 12; 			// conversion: 32MHz/50kHz = 640 --> 640/4095=0.156 --> * 2^12 = 640

	if (result > 640) {							// limitation to 0...640
		result = 640;
		i_val = 0;
	}else if (result < 0) {
		result = 0;
	}

	PWM_TIMER0_change(result);					// set error compensation value
}

/** ***************************************************************************
 * @brief TIMER2 interrupt handler. This interrupt is for the <b>PI controller</b>.
 *
 * The PI controller contains ADC0 measurements, the control path and adjusting the
 * PWM active times.
 * @note This interrupt handler is time critical. Because of ADC0 not faster than 25kHz!
 *****************************************************************************/
void TIMER2_IRQHandler(void) {
	SL_On(SL_0_PORT, SL_0_PIN);					// to see if the PI controller is active

	PI_controller_adjust_current();				// Adjust output current with a PI controller

	SL_Off(SL_0_PORT, SL_0_PIN);				// stop PI controller active LED
	TIMER_IntClear(TIMER2, TIMER_IFC_OF);		// Clear Timer Interrupt flag
	ADC0_start_one();							// start measuring new ADC value
}

/** ***************************************************************************
* @brief Clear a GPIO
* @param [port] port of GPIO
* @param [pin] pin of GPIO
*****************************************************************************/
void GPIO_clear(uint32_t port, uint32_t pin) {
      GPIO_PinOutClear(port, pin);
}

/** ***************************************************************************
 * @brief Initialize TIMER0, TIMER2, LETIMER0
 *
 * All the peripherals are initialized.
 *
 * @note TIMER0 is used to generate PWM signal with a frequency of 50kHz for solution 2.
 * @note TIMER2 is used to generate interrupts for PI controller with a frequency of 10kHz for solution 2.
 * @note LETIMER0 is used to generate PWM signal with a frequency of 400Hz for solution 0
 *****************************************************************************/
void PWM_init(){

	/* CPU Clock 32MHz*/
	CMU_ClockEnable(cmuClock_TIMER0, true);											// enable TIMER0 clock
	CMU_ClockEnable(cmuClock_TIMER2, true);											// enable TIMER2 clock
	CMU_ClockEnable(cmuClock_GPIO, true);											// enable GPIO clock
	CMU_ClockEnable(cmuClock_LETIMER0, true);										// enable LETIMER0 clock
	/* load default values for general TIMER configuration */

	// Set GPIO Mode (Channel 1, 2 already implemented for ETP2)
	GPIO_PinModeSet(gpioPortD, 1, gpioModePushPull, 0);								// output mode: gpioModePushPull
	GPIO_PinModeSet(gpioPortD, 2, gpioModePushPull, 0);								// output mode: gpioModePushPull
	GPIO_PinModeSet(gpioPortD, 3, gpioModePushPull, 0);								// output mode: gpioModePushPull

	// Settings TIMER0, PWM signal for solution 2 (Channel 1, 2 already implemented for ETP2)
	TIMER_TopSet(TIMER0, FREQ_50k);													// TOP defines PWM period 50kHz
	TIMER_CompareSet(TIMER0, 0, 316);												// CC value defines PWM active time
	TIMER_CompareSet(TIMER0, 1, 316);												// CC value defines PWM active time
	TIMER_CompareSet(TIMER0, 2, 316);												// CC value defines PWM active time

	// Set Timer to PWM Mode (Channel 1, 2 already implemented for ETP2)
	TIMER0->CC[0].CTRL = TIMER_CC_CTRL_CMOA_CLEAR | TIMER_CC_CTRL_CUFOA_SET | TIMER_CC_CTRL_MODE_PWM;// Clear output on compare match| Set output on counter underflow|Pulse-Width Modulation
	TIMER0->CC[1].CTRL = TIMER_CC_CTRL_CMOA_CLEAR | TIMER_CC_CTRL_CUFOA_SET | TIMER_CC_CTRL_MODE_PWM;// Clear output on compare match| Set output on counter underflow|Pulse-Width Modulation
	TIMER0->CC[2].CTRL = TIMER_CC_CTRL_CMOA_CLEAR | TIMER_CC_CTRL_CUFOA_SET | TIMER_CC_CTRL_MODE_PWM;// Clear output on compare match| Set output on counter underflow|Pulse-Width Modulation
	TIMER0->ROUTE |= (TIMER_ROUTE_LOCATION_LOC3 | TIMER_ROUTE_CC0PEN | TIMER_ROUTE_CC1PEN | TIMER_ROUTE_CC2PEN); // route output to location #3 and enable output CC0, CC1, CC2

	// Settings TIMER2, interrupts PI controller for solution 2
	TIMER_TopSet(TIMER2, FREQ_10k);													// TOP defines PWM period 10kHz
	TIMER_IntEnable(TIMER2, TIMER_IFC_OF);											// enable Interrupt for TIMER2
	NVIC_EnableIRQ(TIMER2_IRQn);													// enable Interrupt in NVIC

	// Settings LETIMER0, PWM signal for solution 0
	LETIMER_TopSet(LETIMER0, FREQ_400);												// enable LETIMER0
	GPIO_PinModeSet(gpioPortC, 4, gpioModePushPull, 0);								// output mode: gpioModePushPull

	LETIMER0->COMP0 = FREQ_400;														// define PWM period
	LETIMER0->COMP1 = 20;															// define PWM active time
	LETIMER0->REP0 = 1;																// must be nonzero
	LETIMER0->REP1 = 1;																// must be nonzero
	LETIMER0->ROUTE = LETIMER_ROUTE_OUT0PEN | LETIMER_ROUTE_LOCATION_LOC3;			// enable output 0 and route it to location 3

	// Set Low Energy Timer to PWM Mode
	LETIMER0->CTRL = LETIMER_CTRL_COMP0TOP | LETIMER_CTRL_UFOA0_PWM; 				// enable PWM Modus|LETn_O0 is set idle on CNT underflow, and active on compare match with COMP1
}

/** ***************************************************************************
 * @brief Change duty cycle of LETIMER0 in PWM mode.
 * @param [value_compare] value_compare new PWM active time
 *****************************************************************************/
void PWM_LETIMER0_change(uint32_t value_compare) {
	uint32_t val = (value_compare*1285) >>12;				// conversion: 80/255= 0.313 --> *2^12 = 1285
	LETIMER0->COMP1 = val;									// Set new value
}

/** ***************************************************************************
 * @brief Start LETIMER0.
 *****************************************************************************/
void PWM_LETIMER0_start(){
	LETIMER0->CMD = LETIMER_CMD_START;
}

/** ***************************************************************************
 * @brief Stop LETIMER0.
 *****************************************************************************/
void PWM_LETIMER0_stop(){
	LETIMER0->CMD = LETIMER_CMD_STOP;
	GPIO_PinOutClear(gpioPortC, 4);
}

/** ***************************************************************************
 * @brief Change the dutycycle of PWM (TIMER0)
 * @param [compare] on time of PWM
 *****************************************************************************/
void PWM_TIMER0_change(uint32_t compare){
	TIMER0->CC[0].CCVB = compare;
}

/** ***************************************************************************
 * @brief Start TIMER0 and TIMER2 which are needed for solution 2.
 *****************************************************************************/
void PWM_TIMER02_start(){
	TIMER0->CMD = TIMER_CMD_START;
	TIMER2->CMD = TIMER_CMD_START;
}

/** ***************************************************************************
 * @brief Stop TIMER0 and TIMER2 which are needed for solution 2.
 *****************************************************************************/
void PWM_TIMER02_stop(){
	TIMER0->CMD = TIMER_CMD_STOP;
	TIMER2->CMD = TIMER_CMD_STOP;
	GPIO_clear(gpioPortD,1);
	GPIO_clear(gpioPortD,2);
	GPIO_clear(gpioPortD,3);
}
