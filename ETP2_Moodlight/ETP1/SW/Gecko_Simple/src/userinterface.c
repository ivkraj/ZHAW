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
 * @n DESIGN0, DESIGN1, DESIGN2, IDLE, START
 *
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
#include "dac.h"

/******************************************************************************
 * Defines
 *****************************************************************************/

#define UI_STATE_COUNT		5								///< number of FSM states

#define UI_TEXT_COMPARE_LENGTH	3							///< compare first x chars of string
// Must be long enough to distinguish all the texts
// and not longer than the shortest text.

#define UI_DELAY 			10000							///< user interface delay (in TIMER0 ticks)

char *UI_text[UI_STATE_COUNT] = {
		"DESIGN0", "DESIGN1", "DESIGN2", "IDLE", "START"
};															///< text for LCD

/******************************************************************************
 * Variables
 *****************************************************************************/

/**
 * Enum with the FSM states
 */
typedef enum {
	DESIGN_0 = 0, DESIGN_1, DESIGN_2, IDLE, START		// states, count must be = UI_STATE_COUNT
} UI_state_t;

static UI_state_t UI_state_current = START;				///< current state of the FSM
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
	case DESIGN_0:
	case DESIGN_1:
	case DESIGN_2:
		UI_value_next = CAPSENSE_getSliderValue(0, PWR_MAXVALUE[UI_state_next]);	// read value & set of touchsliderFlag

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
		case DESIGN_0:					// Stop all outputs
			PWM_TIMER02_stop();
			PWM_LETIMER0_stop();
			DAC0_stop();
			UI_state_next = IDLE;
			break;
		case DESIGN_1:					// Prepare outputs for Design_0
			PWM_TIMER02_stop();
			PWM_LETIMER0_start();
			DAC0_stop();
			UI_state_next--;			// next state -> Design_0
			break;
		case DESIGN_2:					// Prepare outputs for Design_1
			PWM_TIMER02_stop();
			PWM_LETIMER0_stop();
			DAC0_start(PWR_get_value(1));
			UI_state_next--;			// next state -> Design_1
			break;
		case IDLE:						// Prepare outputs for Design_2
			PWM_TIMER02_start();
			PWM_LETIMER0_stop();
			DAC0_stop();
			UI_state_next = DESIGN_2;
			break;
		default:
			;
		}
		UI_state_changed = true;		// set the flag
		PB0_IRQflag = false;			// reset interrupt flag
	}
	if (PB1_IRQflag) {					// Check if Pushbutton 1 is pressed -> right rotation
		switch (UI_state_current) {
		case DESIGN_0:					// Prepare outputs for Design_1
			PWM_TIMER02_stop();
			PWM_LETIMER0_stop();
			DAC0_start(PWR_get_value(1));
			UI_state_next++;			// next_state -> Design_1
			break;
		case DESIGN_1:					// Prepare outputs for Design_2
			PWM_TIMER02_start();
			PWM_LETIMER0_stop();
			DAC0_stop();
			UI_state_next++;			// next_state -> Design_2
			break;
		case DESIGN_2:					// Stop all outputs
			PWM_TIMER02_stop();
			PWM_LETIMER0_stop();
			DAC0_stop();
			UI_state_next = IDLE;
			break;
		case IDLE:						// Prepare outputs for Design_0
			PWM_TIMER02_stop();
			PWM_LETIMER0_start();
			DAC0_stop();
			UI_state_next = DESIGN_0;
			break;
		default:
			;
		}
		UI_state_changed = true;		// set the flag
		PB1_IRQflag = false;			// reset interrupt flag
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
}

/** **************************************************************************
 * @brief User interface finite state machine: Handled the events
 *
 * Switch to the next state and/or adjust the value.
 *****************************************************************************/
void UI_FSM_state_value(void) {
	if (UI_value_changed) {										// store new value for power LEDs
		switch (UI_state_next) {
		case DESIGN_0:
		case DESIGN_1:
		case DESIGN_2:
			PWR_set_value(UI_state_next, UI_value_next);
			break;
		default:
			;
		}
	}
	if (UI_state_changed || UI_value_changed) {					// display new state and value
		switch (UI_state_next) {
		case DESIGN_0:
		case DESIGN_1:
		case DESIGN_2:
			UI_value_next = PWR_get_value(UI_state_next);		// get the actual value
			/* display state and value */
			SegmentLCD_Write(UI_text[UI_state_next]);
			SegmentLCD_Number(UI_value_next);
			break;
		case IDLE:
		case START:
			/* display state, blank display for value*/
			SegmentLCD_Write(UI_text[UI_state_next]);
			SegmentLCD_NumberOff();
			PWM_TIMER02_stop();
			PWM_LETIMER0_stop();
			DAC0_stop();
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
			PWR_set_value(solution, PWR_MAXVALUE[solution]/4);				// set start value of each solution to 25% of MAXVALUE
		}
		UI_state_next = IDLE;
		UI_state_changed = true;
	}
}
