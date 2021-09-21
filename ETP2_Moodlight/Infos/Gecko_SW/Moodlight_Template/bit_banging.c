/** ***************************************************************************
 * @file
 * @brief bit-banging protocol for the WS2812 LED integrated light source
 *
 * The bit-banging protocol is implemented in standard C
 * and makes no use of assembly language.
 *
 * @note For the time-critical transmit function the
 * compiler optimization level has been defined to guarantee correct timing.
 * @note The timing also depends on the clock frequency.
 *
 * Board:  Starter Kit EFM32-G8XX-STK
 * Device: EFM32G890F128 (Gecko)
 *
 * @author Hanspeter Hochreutener (hhrt@zhaw.ch)
 * @date 21.11.2017
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"


/******************************************************************************
 * Defines
 *****************************************************************************/
#define DATA_PORT		gpioPortD	///< Port for serial data output
#define DATA_PIN		0			///< Pin for serial data output
#define TRIGGER_PORT	gpioPortD	///< Port for oscilloscope trigger signal
#define TRIGGER_PIN		1			///< Pin for oscilloscope trigger signal


/******************************************************************************
 * Variables
 *****************************************************************************/
#define DATA_BYTES		4			///< Number of data bytes to be transmitted
uint8_t data[DATA_BYTES] = { 0xA5, 0x00, 0xFF, 0xA5 };		///< the data


/******************************************************************************
 * Functions
 *****************************************************************************/


/** ***************************************************************************
 * @brief Start crystal oscillators and use these clock sources
 *
 * High frequency clock = 32MHhz (defined by crystal on the starter kit)
 * Low frequency clock = 32.768kHz (defined by crystal on the starter kit)
 *****************************************************************************/
void INIT_XOclocks() {
	// High frequency clock
	CMU_OscillatorEnable(cmuOsc_HFXO, true, true);
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
	CMU_ClockEnable(cmuClock_HFPER, true);
	// Low frequency clock
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
	CMU_ClockSelectSet(cmuClock_CORELE, cmuSelect_LFXO);
	CMU_ClockEnable(cmuClock_CORELE, true);
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
	CMU_ClockEnable(cmuClock_LFA, true);
	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
	CMU_ClockEnable(cmuClock_LFB, true);
}


/** ***************************************************************************
 * @brief Configure the GPIOs used for data and trigger
 *****************************************************************************/
void GPIO_Init() {
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(DATA_PORT, DATA_PIN, gpioModePushPull, 0);
	GPIO_PinModeSet(TRIGGER_PORT, TRIGGER_PIN, gpioModePushPull, 0);

}


#pragma GCC optimize ("O2")
/** ***************************************************************************
 * @brief Output the data using bit-banging technique
 * @param [in] data to be sent
 * @param [in] length of the data
 * @note #pragma before and after the function is used to enable optimization
 * @n bus-access takes three clock cycles => shortest pulse = 3*1/32MHz = 94ns
 * @n The correct timing is done by just repeating a command.
 * @note See https://cdn-shop.adafruit.com/datasheets/WS2812.pdf for the timing
 *****************************************************************************/
void DATA_out(uint8_t data[], uint32_t length) {
	/* Disable the interrupts
	 * to make sure that the sending process is not interrupted	 */
	// NVIC_DisableIRQ(...);
	for (int32_t data_byte = 0; data_byte < length; data_byte++) {
		for (int32_t data_bit = 0; data_bit < 8; data_bit++) {
			GPIO->P[DATA_PORT].DOUTSET = 1 << DATA_PIN;
			if (data[data_byte] & 1<<data_bit) {
				GPIO->P[DATA_PORT].DOUTSET = 1 << DATA_PIN;
				GPIO->P[DATA_PORT].DOUTSET = 1 << DATA_PIN;	// repeat command = waiting
				GPIO->P[DATA_PORT].DOUTSET = 1 << DATA_PIN;	// repeat command = waiting
			} else {
				GPIO->P[DATA_PORT].DOUTCLR = 1 << DATA_PIN;
				GPIO->P[DATA_PORT].DOUTCLR = 1 << DATA_PIN;	// repeat command = waiting
			}
			GPIO->P[DATA_PORT].DOUTCLR = 1 << DATA_PIN;
			GPIO->P[DATA_PORT].DOUTCLR = 1 << DATA_PIN;	// repeat command = waiting
			GPIO->P[DATA_PORT].DOUTCLR = 1 << DATA_PIN;	// repeat command = waiting
			GPIO->P[DATA_PORT].DOUTCLR = 1 << DATA_PIN;	// repeat command = waiting
			GPIO->P[DATA_PORT].DOUTCLR = 1 << DATA_PIN;	// repeat command = waiting
			GPIO->P[DATA_PORT].DOUTCLR = 1 << DATA_PIN;	// repeat command = waiting
		}
	}
	for (int32_t wait_cycle = 0; wait_cycle < 250; wait_cycle++) {	// wait 50us
		GPIO->P[DATA_PORT].DOUTCLR = 1 << DATA_PIN;
	}
	/* Reenable the interrupts that are used */
	// NVIC_EnableIRQ(...);
}
#pragma GCC reset_options


/***************************************************************************//**
 * @brief  Main function
 ******************************************************************************/
int main(void) {
	CHIP_Init();
	INIT_XOclocks();
	GPIO_Init();

	while (1) {
		/* TRIGGER output without compiler optimization */
		GPIO->P[TRIGGER_PORT].DOUTSET = 1 << TRIGGER_PIN;
		GPIO->P[TRIGGER_PORT].DOUTCLR = 1 << TRIGGER_PIN;
		/* DATA output with compiler optimization */
		DATA_out(data, DATA_BYTES);
	}
}
