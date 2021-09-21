/** ***************************************************************************
 * @file
 * @brief User interface
 *
 * This User interface contains 5 states. The user can switch between Design 0, 1 and 2
 * and reset these with pressing the gecko. In the idle state the software does nothing
 * with the output ports.
 *
 * The user interface is implemented as a Finite State Machine.
 *
 * The <b>states</b> are:
 * @n RED, GREEN, BLUE, WHITE, TEMPERATURE, LUMINOSITY, IDLE
 *
 * TODO Anpassen!
 * The <b>events</b> and <b>transitions</b> are:<dl>
 * <dt>Touchgecko pressed</dt>
 * <dd>Go to state START, initialize/reinitialize the system and switch to state IDLE.</dd>
 * <dt>Touchslider touched (only in DESIGN0, 1, 2)</dt>
 * <dd>Adjust value of the currently active state. Only in Design 0,1 and 2 available</dd>
 * <dt>Pushbutton 0 pressed</dt>
 * <dd>Go one state to the right, wrap around from IDLE to DESIGN2.</dd>
 * <dt>Pushbutton 1 pressed</dt>
 * <dd>Go one state to the left, wrap around from DESIGN0 to IDLE.</dd>
 * </dl>
 * Any changes in state or value are reflected on the <b>display</b>.
 *
 * Prefix: UI
 *
 * @author Hanspeter Hochreutener (hhrt@zhaw.ch)
 * @author Lars Mueggler (muegglar@students.zhaw.ch)
 * @date 13.12.2019
 * @version 1.0.0
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

// System Files
#include <stdlib.h>
#include <string.h>

#include "em_emu.h"
#include "segmentlcd.h"

// Own written Files
#include "globals.h"
#include "userinterface.h"
#include "pushbuttons.h"
#include "touchslider.h"
#include "powerLEDs.h"
#include "pwm.h"
#include "communication.h"
#include "i2c_com.h"
#include "gpioInterrupt.h"
#include "externSensors.h"

/******************************************************************************
 * Defines
 *****************************************************************************/

#define UI_STATE_COUNT		14								///< number of FSM states

#define UI_TEXT_COMPARE_LENGTH	3							///< compare first x chars of string
// Must be long enough to distinguish all the texts
// and not longer than the shortest text.

#define UI_DELAY 			10000							///< user interface delay (in TIMER0 ticks)

char *UI_text[UI_STATE_COUNT] = {
		"RED", "GREEN", "BLUE", "WHITE", "RED_ONLY", "GREEN_ONLY", "BLUE_ONLY", "WHITE_ONLY", "TEMPERATURE", "RGBW", "RGBFADE", "RGBWSTROBO", "IDLE", "START"
};															///< text for LCD

char const MODE_CHAR = 'M';
char const ILLUMINATION_CHAR = 'L';
char const TEMPLIMIT_CHAR = 'T';
char const DELIMITER_CHAR[] = {'\n'};

/******************************************************************************
 * Variables
 *****************************************************************************/

UI_state_t UI_state_current = START;				///< current state of the FSM
UI_state_t UI_state_next = START;						///< next state of the FSM
bool UI_state_changed = true;							///< state transition of the FSM
static int32_t UI_value_current = 0;					///< current value (if applicable)
int32_t UI_value_next = 0;								///< next value (if applicable)
bool UI_value_changed = true;							///< value changed

/*******************************************************************************
 * Functions
 *****************************************************************************/

/** **************************************************************************
 * @brief Part of the user interface finite state machine: Touch events
 * Reads for all Designs the slider value
 * @note Because the HW of all solutions can provide more than 350mA, the slider value
 * is limited!
 *****************************************************************************/
void UI_FSM_event_Touch(void) {

	CAPSENSE_Sense();

	switch (UI_state_next) {
	case RED:
	case GREEN:
	case BLUE:
	case WHITE:
	case RED_ONLY:
	case GREEN_ONLY:
	case BLUE_ONLY:
	case WHITE_ONLY:
		UI_value_next = CAPSENSE_getSliderValue(0, MAX_VALUE);	// read value & set of touchsliderFlag

		if (touchsliderFlag) {										// check if touchslider is touched
			UI_value_changed = true;
			touchsliderFlag = false;								// reset the flag
		}
		break;
	default:														// no value to change in Init and Start states
		break;
	}
	if (CAPSENSE_getPressed(BUTTON_CHANNEL)) {						// read status of the touchgecko
		UI_state_next = START;										// initialize system
		UI_state_changed = true;									// set the flag
	}
}

/** **************************************************************************
 * @brief Part of the user interface finite state machine: Handling pushbutton events
 * Pushbutton 0 performs a left rotation in the FSM, Pushbutton 1 however a right rotation
 *****************************************************************************/
void UI_FSM_event_Pushbutton(void) {
	if (PB0_IRQflag) {					// Check if pushbutton 0 is pressed -> left rotation
		switch (UI_state_current) {
		case RED:
			UI_state_next = IDLE;		// next state -> Design_0
			break;
		case GREEN:
			UI_state_next--;			// next state -> Design_0
			break;
		case BLUE:
			UI_state_next--;			// next state -> Design_1
			break;
		case WHITE:
			UI_state_next--;			// next state -> Design_0
			break;
		case RED_ONLY:
			UI_state_next--;			// next state -> Design_0
			break;
		case GREEN_ONLY:
			UI_state_next--;			// next state -> Design_0
			break;
		case BLUE_ONLY:
			UI_state_next--;			// next state -> Design_0
			break;
		case WHITE_ONLY:
			UI_state_next--;			// next state -> Design_0
			break;
		case IDLE:						// Prepare outputs for Design_2
			UI_state_next = WHITE_ONLY;
			break;
		default:
			;
		}
		UI_state_changed = true;		// set the flag
		PB0_IRQflag = false;			// reset interrupt flag
	}
	if (PB1_IRQflag) {					// Check if Pushbutton 1 is pressed -> right rotation
		switch (UI_state_current) {
		case RED:
			UI_state_next++;			// next_state -> Design_1
			break;
		case GREEN:
			UI_state_next++;			// next_state -> Design_2
			break;
		case BLUE:					    // Stop all outputs
			UI_state_next++;			// next_state -> Design_2
			break;
		case WHITE:
			UI_state_next++;			// next_state -> Design_2
			break;
		case RED_ONLY:
			UI_state_next++;			// next_state -> Design_2
			break;
		case GREEN_ONLY:
			UI_state_next++;			// next_state -> Design_2
			break;
		case BLUE_ONLY:
			UI_state_next++;			// next_state -> Design_2
			break;
		case WHITE_ONLY:
			UI_state_next = IDLE;			// next_state -> Design_2
			break;
		case IDLE:						// Prepare outputs for Design_0
			UI_state_next = RED;
			break;
		default:
			;
		}
		UI_state_changed = true;		// set the flag
		PB1_IRQflag = false;			// reset interrupt flag
	}
}

/** **************************************************************************
 * @brief User interface finite state machine: Handled the events
 *
 * Switch to the next state and/or adjust the value.
 *****************************************************************************/
void UI_FSM_state_value(void) {
	if (UI_value_changed) {										// store new value for power LEDs
		switch (UI_state_next) {
		case RED:
		case GREEN:
		case BLUE:
		case WHITE:
		case RED_ONLY:
		case GREEN_ONLY:
		case BLUE_ONLY:
		case WHITE_ONLY:
		case TEMPERATURE:
		case RGBW:
		case RGBFADE:
		case RGBWSTROBO:
			PWR_set_value(UI_state_next, UI_value_next);
			break;
		default:
			;
		}
	}
	if (UI_state_changed || UI_value_changed) {					// display new state and value
		switch (UI_state_next) {
		case RED:
		case WHITE_ONLY:
			PWM_TIMER0_start();
			PWM_LETIMER0_start();
			/* no break */
		case GREEN:
		case BLUE:
		case WHITE:
		case RED_ONLY:
		case GREEN_ONLY:
		case BLUE_ONLY:
			UI_value_next = PWR_get_value(UI_state_next);		// get the actual value
			PWR_set_value(UI_state_next,UI_value_next);
			/* display state and value */
			SegmentLCD_Write(UI_text[UI_state_next]);
			SegmentLCD_Number(UI_value_next);
			break;
		case TEMPERATURE:
		case RGBW:
		case RGBFADE:
		case RGBWSTROBO:
			PWM_TIMER0_start();
			PWM_LETIMER0_start();
			PWR_set_value(UI_state_next, UI_value_next);
			SegmentLCD_Write(UI_text[UI_state_next]);
			SegmentLCD_NumberOff();
			break;
		case IDLE:
		case START:
			/* display state, blank display for value*/
			SegmentLCD_Write(UI_text[UI_state_next]);
			SegmentLCD_NumberOff();
			PWM_TIMER0_stop();
			PWM_LETIMER0_stop();
			break;
		default:
			;
		}
	}
	// Update current state, value and flags
	UI_state_current = UI_state_next;
	UI_state_changed = false;
	UI_value_current = UI_value_next;
	UI_value_changed = false;

	// treat START specifically
	if (UI_state_current == START){
		for (uint32_t solution = 0; solution < PWR_SOLUTION_COUNT; solution++) {
			PWR_set_value(solution, MAX_VALUE/4);
		}
		UI_state_next = IDLE;
		UI_state_changed = true;
	}
}

/** **************************************************************************
 * @brief Part of the user interface finite state machine: Remote control events
 *
 * If a string is available from the serial interface read and analyze it.
 * @n The core is a parser which analyzes the received command string.
 * @n It extracts the state and the value (if any).
 * The value is separated by a ' ' from the state.
 * @n There are 3 possible outcomes:
 * @n - No valid state is recognized
 * => nothing happens
 * @n - A valid state is recognized, but no valid value
 * => switch to that state, don't change the value
 * @n - Valid state and value received
 * => switch to that state and change the value
 *****************************************************************************/
void UI_FSM_event_RemoteControl(void) {
		char command[COM_BUF_SIZE], *colorPointer, *statusPointer, *modePointer, *limitPointer;
		uint32_t i = 0;
		uint32_t mode = 0;
		if (COM_RX_Available() && (overTempFlag == false)) {				// check for a new command string

			SegmentLCD_Symbol(LCD_SYMBOL_ANT, 1);

			COM_RX_GetData(command,COM_BUF_SIZE);
			COM_Flush_Buffers();

			if(command[0] == MODE_CHAR){
				modePointer = strtok(command,DELIMITER_CHAR);
				i = 0;
				while(modePointer != NULL){
					if(i==1){
						mode = atoi(modePointer);
					}
					i++;
					modePointer=strtok(NULL,DELIMITER_CHAR);
				}
				switch(mode){
					case 0:
						UI_state_next = RGBW;
						break;
					case 1:
						UI_state_next = TEMPERATURE;
						break;
					case 2:
						UI_state_next = RGBFADE;
						break;
					case 3:
						UI_state_next = RGBWSTROBO;
						break;
					default:
						break;
				}
				UI_state_changed = true;
			}else if(command[0] == ILLUMINATION_CHAR){
				statusPointer = strtok(command,DELIMITER_CHAR);
				statusPointer = strtok(NULL,DELIMITER_CHAR);
				i = 0;
				while(statusPointer != NULL){
					illumination_settings[i] = atoi(statusPointer);
					i++;
					statusPointer = strtok(NULL,DELIMITER_CHAR);
				}
			}else if(command[0] == TEMPLIMIT_CHAR){
				limitPointer = strtok(command,DELIMITER_CHAR);
				limitPointer = strtok(NULL,DELIMITER_CHAR);
				i = 0;
				while(limitPointer != NULL){
					tempLimit[i] = atoi(limitPointer);
					i++;
					limitPointer = strtok(NULL,DELIMITER_CHAR);
				}
			} else{
				colorPointer = strtok(command,DELIMITER_CHAR);
				i = 0;
				while(colorPointer != NULL){
					RGBW_Color[i] = atoi(colorPointer);
					i++;
					colorPointer=strtok(NULL,DELIMITER_CHAR);
				}
				UI_value_changed = true;
			}

			SegmentLCD_Symbol(LCD_SYMBOL_ANT, 0);
		}
		if(PE1_IRQflag){
			if(GPIOgetBTState()){
				UI_state_next = RGBW;
				illumination_settings[0] = 0;
				illumination_settings[1] = 0;
			}
			else{
				UI_state_next = IDLE;
				illumination_settings[0] = 0;
				illumination_settings[1] = 0;
			}
			UI_state_changed = true;
			PE1_IRQflag = false;
		}
}

/** **************************************************************************
 * @brief User interface finite state machine: Checks for events
 *
 * Check if an event has occurred and define next state and value.
 *****************************************************************************/
void UI_FSM_event(void) {
	UI_FSM_event_Touch();
	UI_FSM_event_Pushbutton();
	UI_FSM_event_RemoteControl();
}

void changeStateTo(UI_state_t nextState){
	UI_state_next = nextState;
	UI_state_changed = true;
}
