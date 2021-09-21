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
#include "externSensors.h"
#include "userinterface.h"
#include "stdlib.h"
#include "time.h"

/******************************************************************************
 * Defines and variables
 *****************************************************************************/
uint32_t COLOR_MAX[] = {29,29,29,31};
/******************************************************************************
 * Functions
 *****************************************************************************/

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
	TIMER_Init_TypeDef timer2_init = TIMER_INIT_DEFAULT;
	/* CPU Clock 32MHz*/
	CMU_ClockEnable(cmuClock_TIMER0, true);											// enable TIMER0 clock
	CMU_ClockEnable(cmuClock_TIMER2, true);											// enable TIMER2 clock
	CMU_ClockEnable(cmuClock_GPIO, true);											// enable GPIO clock
	CMU_ClockEnable(cmuClock_LETIMER0, true);										// enable LETIMER0 clock
	/* load default values for general TIMER configuration */

	// Set GPIO Mode
	GPIO_PinModeSet(gpioPortD, 1, gpioModePushPull, 0);								// output mode: gpioModePushPull
	GPIO_PinModeSet(gpioPortD, 2, gpioModePushPull, 0);								// output mode: gpioModePushPull
	GPIO_PinModeSet(gpioPortD, 3, gpioModePushPull, 0);								// output mode: gpioModePushPull

	/* After prescaling with factor 4 it has frequency of 8kHz*/
	TIMER0->CTRL |= timerPrescale4;
	//timerInit.prescale = timerPrescale1024;

	TIMER_TopSet(TIMER0, FREQ_TIMER1_400);											// TOP defines PWM period 50kHz
	TIMER_CompareSet(TIMER0, 0, FREQ_TIMER1_400/2);									// CC value defines PWM active time
	TIMER_CompareSet(TIMER0, 1, FREQ_TIMER1_400/2);									// CC value defines PWM active time
	TIMER_CompareSet(TIMER0, 2, FREQ_TIMER1_400/2);									// CC value defines PWM active time

	TIMER0->CC[0].CTRL = TIMER_CC_CTRL_CMOA_CLEAR | TIMER_CC_CTRL_CUFOA_SET | TIMER_CC_CTRL_MODE_PWM;// Clear output on compare match| Set output on counter underflow|Pulse-Width Modulation
	TIMER0->CC[1].CTRL = TIMER_CC_CTRL_CMOA_CLEAR | TIMER_CC_CTRL_CUFOA_SET | TIMER_CC_CTRL_MODE_PWM;// Clear output on compare match| Set output on counter underflow|Pulse-Width Modulation
	TIMER0->CC[2].CTRL = TIMER_CC_CTRL_CMOA_CLEAR | TIMER_CC_CTRL_CUFOA_SET | TIMER_CC_CTRL_MODE_PWM;// Clear output on compare match| Set output on counter underflow|Pulse-Width Modulation
	TIMER0->ROUTE |= (TIMER_ROUTE_LOCATION_LOC3 | TIMER_ROUTE_CC0PEN | TIMER_ROUTE_CC1PEN | TIMER_ROUTE_CC2PEN); // route output to location #3 and enable output CC0, CC1, CC2

	// Timer2 used for Temp & Illum-function

	timer2_init.prescale = timerPrescale1024;
	//TIMER2->CTRL |= timerPrescale1024;
	TIMER_Init(TIMER2, &timer2_init);
	TIMER_TopSet(TIMER2, FREQ_TIMER2_1);		// TOP defines Interrupt 20 Hz for Temp and Illum
	TIMER_IntEnable(TIMER2, TIMER_IFC_OF);		// enable Interrupt for TIMER2
	NVIC_EnableIRQ(TIMER2_IRQn);
	TIMER2->CMD = TIMER_CMD_START;

	// Settings LETIMER0, PWM signal for solution 0
	LETIMER_TopSet(LETIMER0, FREQ_LETIMER_400);										// enable LETIMER0
	GPIO_PinModeSet(gpioPortC, 4, gpioModePushPull, 0);								// output mode: gpioModePushPull

	LETIMER0->COMP0 = FREQ_LETIMER_400;												// define PWM period
	LETIMER0->COMP1 = FREQ_LETIMER_400/2;											// define PWM active time
	LETIMER0->REP0 = 1;																// must be nonzero
	LETIMER0->REP1 = 1;																// must be nonzero
	LETIMER0->ROUTE = LETIMER_ROUTE_OUT0PEN | LETIMER_ROUTE_LOCATION_LOC3;			// enable output 0 and route it to location 3

	// Set Low Energy Timer to PWM Mode
	LETIMER0->CTRL = LETIMER_CTRL_COMP0TOP | LETIMER_CTRL_UFOA0_PWM; 				// enable PWM Modus|LETn_O0 is set idle on CNT underflow, and active on compare match with COMP1
	srand(time(NULL));
}


void TIMER2_IRQHandler(void) {
	if ((pre_timer2_counter == 5)||(pre_timer2_counter == 10)||(pre_timer2_counter == 15)|| (pre_timer2_counter == 20)){
		measTempFlag = true;
		temp_index = (pre_timer2_counter/5)-1;
	}
	if (pre_timer2_counter == 20){
		newIllumFlag = true;
		newTempFlag = true;
		pre_timer2_counter = 0;
		if (UI_state_current == RGBWSTROBO){
			UI_value_changed = true;
		}
	}
	else {
		pre_timer2_counter++;
	}

	if (UI_state_current == RGBFADE) {
		if (timer2_interrupt_counter > 6*255) {
			timer2_interrupt_counter = 0;
		}else {
			timer2_interrupt_counter++;
		}
		UI_value_changed = true;
	}
	TIMER_IntClear(TIMER2, TIMER_IFC_OF);		// Clear Timer Interrupt flag
}

void PWM_StroboRGBWLight(void){
	uint32_t red_value = rand()%256;
	uint32_t green_value = rand()%256;
	uint32_t blue_value = rand()%256;

	PWM_TIMER0_RED_change(red_value);
	PWM_TIMER0_GREEN_change(green_value);
	PWM_TIMER0_BLUE_change(blue_value);
}

void PWM_FadeRGBLight(void) {
	uint32_t red_value = 0;
	uint32_t green_value = 0;
	uint32_t blue_value = 0;

	if (timer2_interrupt_counter <= 255) {
		red_value = 255;
		green_value = timer2_interrupt_counter;
	}
	else if (timer2_interrupt_counter <= 2*255) {
		green_value = 255;
		red_value = 2*255 - timer2_interrupt_counter;
	}
	else if (timer2_interrupt_counter <= 3*255) {
		green_value = 255;
		blue_value = timer2_interrupt_counter - 2*255;
	}
	else if (timer2_interrupt_counter <= 4*255) {
		blue_value = 255;
		green_value = 4*255 - timer2_interrupt_counter;
	}
	else if (timer2_interrupt_counter <= 5*255) {
		blue_value = 255;
		red_value = timer2_interrupt_counter - 4*255;
	}
	else if (timer2_interrupt_counter <= 6*255) {
		red_value = 255;
		blue_value = 6*255 - timer2_interrupt_counter;
	}
	else {
		red_value = 255;
	}

	PWM_TIMER0_RED_change(red_value);
	PWM_TIMER0_GREEN_change(green_value);
	PWM_TIMER0_BLUE_change(blue_value);

}

uint32_t calculateDutycycle(uint32_t value, uint32_t timer, uint32_t color){
	uint32_t val = (value*((timer*4096)/MAX_VALUE)) >> 12;
    uint32_t val2 =(val*((color*4096)/MAX_VALUE)) >> 12;
    uint32_t il = ES_IllumToBright();
    uint32_t val3 = (val2*((il*4096)/255)) >> 12;
    return val3;
}

/** ***************************************************************************
 * @brief Change duty cycle of LETIMER0 in PWM mode.
 * @param [value_compare] value_compare new PWM active time
 *****************************************************************************/
void PWM_LETIMER0_WHITE_change(uint32_t compare) {
	uint32_t dut = calculateDutycycle(compare, FREQ_LETIMER_400, COLOR_MAX[3]);
	LETIMER0->COMP1 = dut;			// Set new value

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
 * @brief Change the dutycycle of PWM Blue Channel(TIMER0.0)
 * @param [compare] on time of PWM
 *****************************************************************************/
void PWM_TIMER0_RED_change(uint32_t compare){
	uint32_t dut = calculateDutycycle(compare, FREQ_TIMER1_400, COLOR_MAX[0]);
	TIMER0->CC[0].CCVB = dut;
}

/** ***************************************************************************
 * @brief Change the dutycycle of PWM Green Channel(TIMER0.1)
 * @param [compare] on time of PWM
 *****************************************************************************/
void PWM_TIMER0_GREEN_change(uint32_t compare){
	uint32_t dut = calculateDutycycle(compare, FREQ_TIMER1_400, COLOR_MAX[1]);
	TIMER0->CC[1].CCVB = dut;
}

/** ***************************************************************************
 * @brief Change the dutycycle of PWM Blue Channel(TIMER0.2)
 * @param [compare] on time of PWM
 *****************************************************************************/
void PWM_TIMER0_BLUE_change(uint32_t compare){
	uint32_t dut = calculateDutycycle(compare, FREQ_TIMER1_400, COLOR_MAX[2]);
	TIMER0->CC[2].CCVB = dut;
}

/** ***************************************************************************
 * @brief Start TIMER0 and TIMER2 which are needed for solution 2.
 *****************************************************************************/
void PWM_TIMER0_start(){
	TIMER0->CMD = TIMER_CMD_START;

}

/** ***************************************************************************
 * @brief Stop TIMER0 and TIMER2 which are needed for solution 2.
 *****************************************************************************/
void PWM_TIMER0_stop(){
	TIMER0->CMD = TIMER_CMD_STOP;
	//TIMER2->CMD = TIMER_CMD_STOP;
	GPIO_clear(gpioPortD,1);
	GPIO_clear(gpioPortD,2);
	GPIO_clear(gpioPortD,3);
}



