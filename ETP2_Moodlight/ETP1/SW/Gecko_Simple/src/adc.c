/** ***************************************************************************
 * @file
 * @brief ADC
 *
 * All the functions needed for the ADC.
 *
 * Prefix: ADC
 *
 * Board:  Starter Kit EFM32-G8XX-STK
 * Device: EFM32G890F128 (Gecko)
 *
 * @author Lars Mueggler (muegglar@students.zhaw.ch)
 * @date 21.10.2019
 * @version 1.0.0
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

// System Files
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "em_chip.h"
#include "em_device.h"
#include "em_system.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_adc.h"

// Own written files
#include "adc.h"

/******************************************************************************
 * Functions
 *****************************************************************************/

/** ***************************************************************************
 * @brief Read the converted value from ADC0.
 *
 * This function reads a single value from the ADC0.
 *
 * @return ADC reading
 *****************************************************************************/
uint32_t ADC0_read_one(void) {
	return ADC0->SINGLEDATA;				// read converted ADC value
}

/** ***************************************************************************
 * @brief Start ADC0 for one single conversion.
 * This function starts one single conversion from the ADC.
 *****************************************************************************/
void ADC0_start_one(void){
	ADC0->CMD = ADC_CMD_SINGLESTART;		// start ADC conversion
}

/** ***************************************************************************
 * @brief Initialization of the ADC.
 *
 * This function initializes the ADC0.
 *****************************************************************************/
void ADC0_init(void) {
	uint32_t ADC_clock_frequency = 1000000;								// 13MHz is the upper limit, ADC_clock_frequency > (bit_resolution + 1) * sampling_frequency

	CMU_ClockEnable(cmuClock_ADC0, true);								// enable ADC clock

	/* load default values for general ADC configuration */
	ADC_Init_TypeDef ADCinit = ADC_INIT_DEFAULT;
	ADCinit.warmUpMode = adcWarmupKeepADCWarm;							// mode for ADC reference
	ADCinit.lpfMode = adcLPFilterDeCap;									// best SNR with this filter
	ADCinit.prescale = ADC_PrescaleCalc(ADC_clock_frequency, 0);		// Frequency prescaler value
	ADC_Init(ADC0, &ADCinit);											// write configuration registers

	/* load default values for ADC channel configuration */
	ADC_InitSingle_TypeDef ADCsingleInit = ADC_INITSINGLE_DEFAULT;
	ADCsingleInit.reference = adcRef2V5;								// use internal 2.5V reference
	ADCsingleInit.input = adcSingleInpCh0;								// ADC channel 0
	ADCsingleInit.resolution = adcRes12Bit;								// 12 bit resolution
	ADC_InitSingle(ADC0, &ADCsingleInit);								// write input configuration
	ADC0->CTRL = _ADC_CTRL_WARMUPMODE_NORMAL;
}
