#include "CppUTest/TestHarness.h"

#include "stdio.h"
#include "hal_gpio.h"
#include "event_handler.h"
#include "timer.h"

TEST_GROUP(event_handler)
{
    void setup()
    {
        printf("\n==================================================\n");
    }

    void teardown()
    {
        printf("\n==================================================\n \n \n");
    }
};


/* Test name: test_no_timeout_event
   Test sequence:
        - Start the timer with a duartion of 10
        - Call eh_get_event() once
        - Check that the event TIME_OUT is not generated                */

/// STUDENTS: To be programmed




/// END: To be programmed

/* Test name: test_timeout_event
   Test sequence:
        - Start the timer with a duartion X so that the event 
          TIME_OUT can be generated
        - Call eh_get_event() once
        - Check that the event TIME_OUT is generated                    */

/// STUDENTS: To be programmed




/// END: To be programmed

/* Test name: test_edge_detection
   Test sequence:
        - Initialize the input register (IDR) of GPIOB to 0
        - Call eh_get_event()
        - Check that no event is generated
        - Manipulate the input register (IDR) of GPIOB to generate an event
          EV_CAR_W
        - Call eh_get_event()
        - Check that the event EV_CAR_W is generated
        - Reset the input register (IDR) of GPIOB
        - Call eh_get_event()
        - Check that no event is generated                              */

/// STUDENTS: To be programmed




/// END: To be programmed
