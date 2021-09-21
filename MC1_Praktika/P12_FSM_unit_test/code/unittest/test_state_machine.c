#include "CppUTest/TestHarness.h"

#include "stdio.h"
#include "action_handler.h"
#include "event_detector.h"
#include "hal_gpio.h"
#include "state_machine.h"
#include "timer.h"

/* A group of tests to test the entire FSM system consisting of the modules 
   state_machine, event_detector, action_handler and timer                */
TEST_GROUP(FSM_system)
{
     /* This function is executed at the START of each test that belongs to
        this test group */
    void setup()
    {
        printf("\n==================================================\n");

        // always reset all input events to zero
        GPIOB->IDR = 0x00000000;
    }

     /* This function is executed at the END of each test that belongs to
        this test group */
    void teardown()
    {
        printf("\n==================================================\n \n \n");
    }
};


/* Test name: car_e2_ped_w_ev_car_e1
   Purpose: Verify that in state CAR_E2_PED_W, the FSM transitions to
            state CAR_E2_PED_W_CAR_E1_E2 when a rising edge is applied on 
            button CAR_E1
   Test sequence:
        - Set the state to CAR_E2_PED_W
        - Manipulate the input register (IDR) of GPIOB to generate a rising edge 
          on button CAR_E1
        - Handle the event by calling fsm_handle_event() using the return 
          value of eh_get_event() as an argument
                                  --> fsm_handle_event(eh_get_event())
        - Check that the state has been set to CAR_E2_PED_W_CAR_E1_E2       */

/// STUDENTS: To be programmed
TEST(FSM_system, car_e2_ped_w_ev_car_e1)
{
		event_t event_generated;
		state_t fsm_state;
	
		fsm_set_state(CAR_E2_PED_W);
	
		GPIOB->IDR = 0x00000000;
	
		event_generated = ed_get_event();
	
		GPIOB->IDR |= MASK_CAR_E1;
	
		event_generated = ed_get_event();
	
    printf("Event generated: %04X\n", event_generated);
	
		fsm_state = fsm_handle_event(event_generated);
	
	  CHECK_TEXT(fsm_state == CAR_E2_PED_W_CAR_E1_E2, "CAR_E2_PED_W_CAR_E1_E2 is not generated");

    printf("\n");
}

/// END: To be programmed


/* Test name:  car_e2_ped_w_ev_ped_e
   Purpose: Verify that in state CAR_E2_PED_W, the FSM transitions to
            state CAR_E2_PED_W_PED_E_W when a rising edge is applied on 
            button PED_E
   Test sequence:
        - Set the state to CAR_E2_PED_W
        - Manipulate the input register (IDR) of GPIOB to generate a rising edge 
          on button PED_E
        - Handle the event by calling fsm_handle_event() using the return
          value of eh_get_event() as an argument.
                                  --> fsm_handle_event(eh_get_event())
        - Check that state has been set to CAR_E2_PED_W_PED_E_W         */

/// STUDENTS: To be programmed
TEST(FSM_system, car_e2_ped_w_ev_ped_e)
{
		event_t event_generated;
		state_t fsm_state;
	
		fsm_set_state(CAR_E2_PED_W);
	
		GPIOB->IDR = 0x00000000;
	
		event_generated = ed_get_event();
	
		GPIOB->IDR |= MASK_PED_E;
	
		event_generated = ed_get_event();
	
    printf("Event generated: %04X\n", event_generated);
	
		fsm_state = fsm_handle_event(event_generated);
	
	  CHECK_TEXT(fsm_state == CAR_E2_PED_W_PED_E_W, "CAR_E2_PED_W_PED_E_W is not generated");

    printf("\n");
}

/// END: To be programmed


/* Test name:  car_e2_ped_w_to_invalid
   Purpose: Verify that in state CAR_E2_PED_W, the FSM does not transition to 
            another state if there are events other than rising edges on 
            the buttons CAR_E1 or PED_E1
   Test sequence:
        - Set the state to CAR_E2_PED_W
        - Manipulate the input register (IDR) of GPIOB to generate simultaneouly 
          rising edges on all the buttons except CAR_E1 and PED_E
        - Start the timer with time 1
        - Handle the event by calling fsm_handle_event() using the return
          value of eh_get_event() as an argument.
                                 --> fsm_handle_event(eh_get_event())
        - Check that no state transition has occured                    */ 

/// STUDENTS: To be programmed
TEST(FSM_system, car_e2_ped_w_to_invalid)
{
		event_t event_generated;
		state_t fsm_state;
	
		fsm_set_state(CAR_E2_PED_W);
	
		GPIOB->IDR = 0x00000000;
	
		event_generated = ed_get_event();
	
		GPIOB->IDR |= (MASK_CAR_W | MASK_CAR_S | MASK_CAR_E2 | MASK_PED_W);
	
		timer_start(1);
	
		event_generated = ed_get_event();
	
    printf("Event generated: %04X\n", event_generated);
	
		fsm_state = fsm_handle_event(event_generated);
	
	  CHECK_TEXT(fsm_state == CAR_E2_PED_W, "State transition has occured");

    printf("\n");
}

/// END: To be programmed
