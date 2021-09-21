/** ***************************************************************************
 * @file
 * @brief GPIO Interrupt
 *
 * Initializing the timers and output pins, functions to change the duty cycles
 * and start/stop the PWM signals
 *
 * Prefix: GPIOINT
 *
 *
 * Board:  Starter Kit EFM32-G8XX-STK
 * Device: EFM32G890F128 (Gecko)
 *
 * @authors Lars Mueggler (muegglar@students.zhaw.ch)
 * @date 04.03.2020
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
#include "em_i2c.h"

// Own written Files
#include "gpioInterrupt.h"

/******************************************************************************
 * Defines and variables
 *****************************************************************************/

/******************************************************************************
 * Functions
 *****************************************************************************/
/** ***************************************************************************
 * @brief LumINT
 *
 *****************************************************************************/
void GPIOINT_LumINT(){
// TODO

}

/** ***************************************************************************
 * @brief TempOS
 *
 *****************************************************************************/
void GPIOINT_TempOS(){

// TODO
}

bool GPIOgetBTState(){

	if(GPIO_PinInGet(PE1_PORT,PE1_PIN) == 1){
		return true;
	}
	else{
		return false;
	}
}

/** ***************************************************************************
 * @brief Initialize GPIOs for Interrupts of OPT3001 and LM75
 *
 *****************************************************************************/
void GPIOINT_init(){

	// enable GPIO clock
	CMU_ClockEnable(cmuClock_GPIO, true);

	// Set GPIO Mode
	GPIO_PinModeSet(PD0_PORT, PD0_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(PC5_PORT, PC5_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(PE1_PORT, PE1_PIN, gpioModeInput, 0);

	// Set falling edge interrupt
	GPIO_IntConfig(PD0_PORT, PD0_PIN, false, true, true);
	GPIO_IntConfig(PC5_PORT, PC5_PIN, false, true, true);
	GPIO_IntConfig(PE1_PORT, PE1_PIN, true, true, true);

	// GPIO Interrupt Init
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
	NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}

