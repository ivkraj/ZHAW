/** ***************************************************************************
 * @file
 * @brief Global variables, defines and functions
 *
 * Prefix: G
 *
 * @author Hanspeter Hochreutener (hhrt@zhaw.ch)
 * @author Lars Mueggler (muegglar@students.zhaw.ch)
 * @date 12.12.2019
 * @version 1.0.0
 *****************************************************************************/ 

/******************************************************************************
 * Includes
 *****************************************************************************/

// System files
#include "em_cmu.h"

// Own written files
#include "globals.h"
#include "powerLEDs.h"


/******************************************************************************
 * Constants
 *****************************************************************************/
uint32_t const FREQ_LETIMER_400 = 80;										 ///< Count for Frequency 400 Hz	32768/400 = 81..
uint32_t const FREQ_TIMER1_400 = 40000;										 ///< Count for Frequency 400 Hz	8000000/400=80000
uint32_t const FREQ_TIMER2_1 = 1562;
uint32_t const MAX_VALUE = 255;

/******************************************************************************
 * Variables
 *****************************************************************************/
uint32_t illumination_settings[3] = {0,0,1};										///< Settings for Illumination Default NotInverted[0], OFF[1]
int32_t temperature = 0;
uint32_t illumination = 0;
int32_t temp_values[] = {0,0,0,0};
char temperatureString[] = {'T','0','0','0','0','0','0','0','0','0','0','0','0','0','0','\0'};
char overTempONString[] = {'O','0','0','0','0','0','0','0','0','0','0','0','0','0','1','\0'};
char overTempOFFString[] = {'O','0','0','0','0','0','0','0','0','0','0','0','0','0','0','\0'};
bool newIllumFlag = false, newTempFlag = false, measTempFlag = false, overTempFlag;
uint32_t pre_timer2_counter = 0, timer2_interrupt_counter = 0;
/******************************************************************************
 * Functions
 *****************************************************************************/

/** ***************************************************************************
 * @brief Start crystal oscillators and use these clock sources
 *
 * High frequency clock = 32MHz (defined by crystal on the starter kit)
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

/**************************************************************************//**
 * @brief  Convert an integer to a string (in base 10 representation)
 * @param [in] l = integer to convert
 * @param [out] string = string representation of number l
 *
 * ltostr (long to string) is the inverse function to strtol (string to long)
 * @n strtol is a standard function in stdlib.h
 * @n ltostr unfortunately is not. So it is provided here for convenience.
 * @n Make sure the string is at least [12] long to hold the maximum integer.
 * @note strtol and ltostr need less memory than sscanf and sprintf
 ******************************************************************************/
void ltostr(int32_t l, char *string) {
	const char digit[] = "0123456789";	// digit characters
	if (0 > l) {						// negative integer
		l *= -1;						// change sign
		*string++ = '-';				// output a '-' and move one digit to the right
	}
	int tmp = l;
	do {								// calculate number of digits in output string
		tmp = tmp / 10;					// next digit
		string++;						// move to the end of the output string
	} while (0 < tmp);					// do while handles also the special case l == 0
	*string-- = '\0';					// output end of string char and move to last digit
	do {								// begin with rightmost digit in output string
		*string-- = digit[l % 10];		// last digit and move to next digit left
		l = l / 10;						// remaining integer portion without last digits
	} while (0 < l);					// do while handles also the special case l == 0
}

/** **************************************************************************
 * @brief User interface finite state machine: Handled the events
 *
 * Switch to the next state and/or adjust the value.
 *****************************************************************************/

void convertTemperatureToString(int32_t temp){
	temperatureString[14] =(char)(temp&0xF)+65;
	temperatureString[13] =(char)((temp>>4)&0xF)+65;
	temperatureString[12] =(char)((temp>>8)&0xF)+65;
	temperatureString[11] =(char)((temp>>12)&0xF)+65;

	if(temperatureString[14] == '\0'){
		temperatureString[14] = '0';
	}
	if(temperatureString[13] == '\0'){
		temperatureString[13] = '0';
	}
	if(temperatureString[12] == '\0'){
			temperatureString[12] = '0';
	}
	if(temperatureString[11] == '\0'){
			temperatureString[11] = '0';
	}
}
