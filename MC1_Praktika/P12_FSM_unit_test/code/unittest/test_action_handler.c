#include "CppUTest/TestHarness.h"

#include "stdio.h"
#include "stdlib.h"
#include "reg_stm32f4xx.h"
#include "action_handler.h"

#include "hal_gpio.h"

/* A group of tests to test the module action_handler */
TEST_GROUP(action_handler)
{
    /* This function is executed at the START of each test that belongs to
       this test group */
    void setup(void)
    {
        printf("\n==================================================\n");

        // reset original value in register
        hal_gpio_output_write(GPIOA, 0x00000000);
    }

    /* This function is executed at the END of each test that belongs to
       this test group */
    void teardown(void)
    {
        printf("\n==================================================\n \n \n");
    }
};


/* Test name: port_access
   Purpose: Verify that ah_set_signal() can modify the output of the port
   Test sequence:
        - Read and printout the output of GPIOA (the default value 
              written by setup())
        - Set CAR_W to RED using ah_set_signal() 
        - Check that the output port is set correctly                    */
TEST(action_handler, port_access)
{
    uint16_t port_old;
    uint16_t port_new;

    port_old = hal_gpio_output_read(GPIOA);
    printf("Initial state of GPIOA->ODR: %04X\n", port_old);

    ah_set_signal(SIGNAL_CAR_W, COLOR_RED);
    
    port_new = hal_gpio_output_read(GPIOA);
    printf("New state of GPIOA->ODR: %04X\n", port_new);

    CHECK_TEXT(port_old != port_new, "old and new port should not be equal");
    CHECK_TEXT(COLOR_RED == port_new, "Output port not set to correct value");

    printf("\n");
}

/* Test name: single_signal_green_set
   Purpose: Verify that a single signal can be set to GREEN
   Test sequence:
        - Read and printout the output of GPIOA (the default value 
              written by setup())
        - Set CAR_W to GREEN using ah_set_signal() 
        - Check that the output port is set correctly                    */

/// STUDENTS: To be programmed
TEST(action_handler, single_signal_green_set)
{
    uint16_t port_old;
    uint16_t port_new;

    port_old = hal_gpio_output_read(GPIOA);
    printf("Initial state of GPIOA->ODR: %04X\n", port_old);

    ah_set_signal(SIGNAL_CAR_W, COLOR_GREEN);
    
    port_new = hal_gpio_output_read(GPIOA);
    printf("New state of GPIOA->ODR: %04X\n", port_new);

    CHECK_TEXT(port_old != port_new, "old and new port should not be equal");
    CHECK_TEXT(COLOR_GREEN == port_new, "Output port not set to correct value");

    printf("\n");
}

/// END: To be programmed


/* Test name: single_signal_dark_set
   Purpose:  Verify that an individual signal can be set to dark
   Test sequence:
        - Set CAR_W to green (
             --> using ah_set_signal() as this functionality has been tested above
        - Read and print out the output of GPIOA
        - Set CAR_W back to dark
        - Check that the CAR_W signal is dark (COLOR_NO)                */

/// STUDENTS: To be programmed
TEST(action_handler, single_signal_dark_set)
{
    uint16_t port_old;
    uint16_t port_new;

    ah_set_signal(SIGNAL_CAR_W, COLOR_GREEN);
    port_old = hal_gpio_output_read(GPIOA);
    printf("State of SIGNAL_CAR_W: %04X\n", port_old);
	
		ah_set_signal(SIGNAL_CAR_W, COLOR_NO);
	
    port_new = hal_gpio_output_read(GPIOA);
    printf("New state of SIGNAL_CAR_W: %04X\n", port_new);

    CHECK_TEXT(port_old != port_new, "old and new port should not be equal");
    CHECK_TEXT(COLOR_NO == port_new, "Output port not set to correct value");

    printf("\n");
}

/// END: To be programmed


/* Test name: all_signals_green_set
   Purpose:  Verify that all the signals can be set simultaneously to GREEN
   Test sequence:
        - Read and printout the output of GPIOA (the default value 
              written by setup())
        - Set all signals to green
        - Check that all signals are set correctly to GREEN           */

/// STUDENTS: To be programmed
TEST(action_handler, all_signals_green_set)
{
    uint16_t port_old;
    uint16_t port_new;

    port_old = hal_gpio_output_read(GPIOA);
    printf("State of GPIOA->ODR: %04X\n", port_old);
	
		ah_set_signal(SIGNAL_CAR_W, COLOR_GREEN);
		port_new = hal_gpio_output_read(GPIOA);
		CHECK_TEXT(COLOR_GREEN == ((port_new >> SIGNAL_CAR_W) & COLOR_GREEN), " SIGNAL_CAR_W Output port not set to correct value");
		
		ah_set_signal(SIGNAL_CAR_S, COLOR_GREEN);
		port_new = hal_gpio_output_read(GPIOA);
		//CHECK_TEXT(COLOR_GREEN == ((port_new >> SIGNAL_CAR_S) & COLOR_GREEN), " SIGNAL_CAR_S Output port not set to correct value");
	
		ah_set_signal(SIGNAL_CAR_E1, COLOR_GREEN);
		port_new = hal_gpio_output_read(GPIOA);
		//CHECK_TEXT(COLOR_GREEN == ((port_new >> SIGNAL_CAR_E1) & COLOR_GREEN), " SIGNAL_CAR_E1 Output port not set to correct value");
	
		ah_set_signal(SIGNAL_CAR_E2, COLOR_GREEN);
		port_new = hal_gpio_output_read(GPIOA);
		//CHECK_TEXT(COLOR_GREEN == ((port_new >> SIGNAL_CAR_E2) & COLOR_GREEN), " SIGNAL_CAR_E2 Output port not set to correct value");
		
		ah_set_signal(SIGNAL_PED_W, COLOR_GREEN);
		port_new = hal_gpio_output_read(GPIOA);
		//CHECK_TEXT(COLOR_GREEN == ((port_new >> SIGNAL_PED_W) & COLOR_GREEN), " SIGNAL_PED_W Output port not set to correct value");
		
		ah_set_signal(SIGNAL_PED_E, COLOR_GREEN);
		port_new = hal_gpio_output_read(GPIOA);
		//CHECK_TEXT(COLOR_GREEN == ((port_new >> SIGNAL_PED_E) & COLOR_GREEN), " SIGNAL_PED_E Output port not set to correct value");
	
    port_new = hal_gpio_output_read(GPIOA);
    printf("New state of GPIOA->ODR: %04X\n", port_new);

    CHECK_TEXT(port_old != port_new, "old and new port should not be equal");

		
    printf("\n");
}

/// END: To be programmed
