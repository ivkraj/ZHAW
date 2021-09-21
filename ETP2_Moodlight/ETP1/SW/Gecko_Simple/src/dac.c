/** ***************************************************************************
 * @file
 * @brief DAC
 *
 * All the functions needed for the DAC
 *
 * Prefix: DAC
 *
 * Board:  Starter Kit EFM32-G8XX-STK
 * Device: EFM32G890F128 (Gecko)
 *
 * @author Lars Mueggler (muegglar@students.zhaw.ch)
 * @date 17.10.2019
 * @version 1.0.0
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

// System Files
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "em_emu.h"
#include "em_cmu.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_dac.h"
#include "em_timer.h"

// Own written Files
#include "segmentlcd.h"
#include "dac.h"

/******************************************************************************
 * Functions
 *****************************************************************************/

/** ***************************************************************************
 * @brief Initialize DAC0 channel 1
 *****************************************************************************/
void DAC0_init(void) {
	uint32_t ADC_clock_frequency = 1000000;								// 1MHz is the upper limit (DAC_clock_frequency < 1MHz)
	CMU_ClockEnable(cmuClock_DAC0, true);								// enable DAC clock

	DAC_Init_TypeDef DACinit = DAC_INIT_DEFAULT;						// load default values for general DAC configuration
	DACinit.prescale = DAC_PrescaleCalc(ADC_clock_frequency, 0);		// Frequency prescaler Value
	DACinit.reference = dacRef2V5;										// use internal 2.5V reference
	DAC_Init(DAC0, &DACinit);											// write configuration registers

	DAC_InitChannel_TypeDef DACinitChannel = DAC_INITCHANNEL_DEFAULT;	// load default values for DAC channel configuration
	DAC_InitChannel(DAC0, &DACinitChannel, 1);							// write channel 1 configuration
	DAC_Enable(DAC0, 1, true);											// enable channel 1
	DAC0->CH1DATA = 0;													// output value for channel 1
}

/** ***************************************************************************
 * @brief Output new value to DAC0 channel 1
 * @param [value] new output value
 *****************************************************************************/
void DAC0_write(uint32_t value) {
	DAC0->CH1DATA = (value*65777)>>12;									// conversion: 4095/255=16.0588 --> *2^12= 65777
}

/** ***************************************************************************
 * @brief Start DAC0.
 * @param [value] brightness of the led
 *****************************************************************************/
void DAC0_start(uint32_t value){
	DAC0_write(value);
}

/** ***************************************************************************
 * @brief Stop DAC0.
 *****************************************************************************/
void DAC0_stop(){
	DAC0->CH1DATA = 0;
}
