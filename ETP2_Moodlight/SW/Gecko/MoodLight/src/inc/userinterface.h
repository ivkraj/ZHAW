/** ***************************************************************************
 * @file
 * @brief See userinterface.c
 *****************************************************************************/

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

/**
 * Enum with the FSM states
 */
typedef enum {
	RED = 0, GREEN, BLUE, WHITE, RED_ONLY, GREEN_ONLY, BLUE_ONLY, WHITE_ONLY, TEMPERATURE, RGBW, RGBFADE, RGBWSTROBO, IDLE, START		// states, count must be = UI_STATE_COUNT
} UI_state_t;

UI_state_t UI_state_current;

/******************************************************************************
 * Functions
 *****************************************************************************/

void UI_FSM_event(void);
void UI_FSM_state_value(void);
void changeStateTo(UI_state_t nextState);
bool UI_value_changed;

#endif
