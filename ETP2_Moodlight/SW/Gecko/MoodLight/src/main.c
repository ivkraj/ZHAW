/** ***************************************************************************
 * @mainpage Moodlight Simple
 *
 * \tableofcontents
 *
 * @section selc1 Abstract
 *
 * This software (SW) implements three controllable buck converters for an LED Moodlight.
 * These solutions were developed in advance and are used as LED drivers.
 * The current can be regulated linear between 0mA and 350mA. 'Solution 0'
 * and 'Solution 1' use the IC
 * <a href="https://www.diodes.com/assets/Datasheets/ZXLD1350.pdf">ZXLD1350</a>
 * to control the current. For 'Solution 2'
 * the current regulation is done by SW with a PI controller. This solution needs a
 * buck converter and the IC
 * <a href="https://www.diodes.com/assets/Datasheets/ZXCT1107_10.pdf">ZXCT1110</a>
 * for current measuring.
 * All solutions are working without any problems. The regulation of 'Solution 2' has
 * a current ripple of about 3 mA, which leads to flickering of the LEDs at the lower current range.
 *
 * @section selc Overview
 *
 * The LEDs are driven by our own developed HW and a microcontroller SW, written in C.
 * @image html LEDdriverHW.jpg
 *
 * For the user interface we received a microcontroller evaluation board
 * <a href="https://www.silabs.com/products/development-tools/mcu/32-bit/efm32-gecko-starter-kit">EFM32G890F128</a>
 * (Gecko).
 * This evaluation board contains pushbuttons to choose between the solutions, a touchslider to change the current
 * and a touchgecko to make a restart.
 * @image html gecko.jpg
 *
 * The FSM is set up as follow:
 * @image html bubblediagramm_etp1.png
 * 
 * @section selc2 Specification of the project purpose
 *
 * The purpose of this project is to develop the hardware and software for
 * a buck converter which can be used for a Moodlight.
 * Three different prototypes were designed, built and evaluated.
 * This documentation contains the SW for these prototypes. The
 * goal of this project is to develop a buck converter that is easy to use, reliable and can regulate almost linear
 * the current through the LEDs from 0mA to 350 mA.
 *
 * @section selc3 Description of the files and modules
 *
 * The \a main.c file only contains the main function with its initializations of the whole system. The user interface is programmed as a finite state
 * machine (FSM) and reads the actual value of the touchslider. It can be found in the
 * \a userinterface.c file. This file also controls the I/O's of the individual solutions.
 * The initialization of the touchslider and the functions to read the touchslider value are in the \a touchslider.c file.
 * Pushbutton functions are implemented in the \a pushbuttons.c file.
 * The functions to control the LEDs of the Gecko-Starterkit are in the
 * \a signalLEDs.c file. All these files and templates were taken from the
 * "MoodLight_Template_SDK581.zip" and customized. 'Solution 0' and 'Solution 1' are implemented in the \a powerLEDs.c file.
 * The functions of these two solutions are in the files \a dac.c and \a pwm.c. The PI controller in 'Solution 2' and its functions are implemented in the \a pwm.c file.
 * The control loop of PI controller uses the functions of the \a adc.c file.
 * In the \a globals.c file some constants were implemented.
 *
 * @section selc4 Code Guideline
 *
 * - Define constants and use them to make the code easier to read and to understand
 * - Use modular design to make the code clearly arranged
 * - Capitalization style: use Prefix at every function, the variables and
 * function should be in the style like var_1, and PREFIX_function_2
 * - Bodies of functions, loops, if-else statements, etc.
 * should be indented and statements within the same body-level should be
 * indented the same amount
 * - Comment the code: every file should have a short description what the
 * purpose of the file is, who's the author and the date. Every function should have
 * a comment with the parameters and purpose.
 * - No floating point operation and division realised with shift operation
 *
 * @section selc5 'Solution 0/1'
 *
 * @subsection selc6 Description
 * 'Solution 0' uses an PWM signal of 400Hz to adjust the current of the LEDs. There is no control loop.
 * The current is set directly to the value of the touchslider.
 * 'Solution 1' uses a DC level to adjust the current of the LEDs.
 * The principle is the same as for 'Solution 0'. There is no control loop and
 * the current is set directly to the value of the touchslider.
 *
 * @subsection selc7 Approach
 * Because there is no control loop in both solutions,
 * the implementations is quite simple. The only difficulty
 * was to convert the slider value correctly and to initialize the Low Energy Timer.
 *
 * @subsection selc8 Tests
 * The current through the LEDs can be regulated smoothly
 * and the current can be varied between 0mA and 350mA.
 * Because there is no control loop, it's not necessary to do
 * more tests. Only the set value has to correspond to the actual value.
 *
 * @subsection selc9 Conclusion
 * These two solution works as expected. The current can be
 * regulated smoothly from 0mA to 350mA. Both solutions are implemented on the same input pin.
 * @n To select one of these solutions, it's necessary to move the jumper J2.
 *
 * @section selc10 'Solution 2'
 *
 * @subsection selc11 Description
 * 'Solution 2' uses the PI controller to control the current. The ADC0
 * measures a voltage that is proportional to the LED current. The ADC value is
 * compared to the slider value and the PWM is continuously regulated to get the target
 * current through the LEDs. The PI controller compares and adjusts every
 * 0.1 ms the current through the LEDs when solution 2 is selected.
 * The function call of the PI controller is realised with a TIMER2-interrupt.
 *
 * @image html PIController.png "PI controller block diagram"
 * Description:
 * The PI controller is realised in the function \a PI_Controller_Adjust_Current().
 * First of all the PI controller calculates the difference between the actual current and the set value.
 * This difference gets multiplied with a P- and an I-component. Due to the P-value,
 * the step response of the control loop is very high, but there is still a deviation to the set value.
 * The function of the I-component is to regulate the system deviation slowly but precisely to zero.
 * The connection of the P-component and the I-components builds a functional control loop to adjust the current.
 *
 * @subsection selc12 Approach
 * To get a good P- and I-value, the I-value of the control loop was set to zero firstly. The P-value was adapted
 * until the output signal of the control loop started to oscillate. The received P-value was then multiplied with
 * the factor 0.45 (Ziegler-Nichols) to get the final P-value. For a proper I-value, the I-value was adapted until the step response of the
 * control loop was acceptable. Means the reaction of the PI controller should be fast and there should not be
 * any overshooting.
 *
 * @subsection selc13 Tests
 * The current through the LEDs can be regulated smoothly. Flickering is just visible
 * at the lower current range (approx. 20mA).The controller has no
 * overshooting at any point of time. The current can be regulated between 0mA and
 * 350mA. To control the functionality of the PI controller, it is necessary
 * to check the step response of the control loop and the remaining deviation.
 * These functionalities were checked at the ADC output of the IC ZXCT1110.
 *
 * @image html PI_regulator_up.jpg "PI controller step response up"
 *
 * The controller takes less than 2ms to respond to a fast change of the set current and there is no
 * overshoot visible. This fast reaction time is due to the P-component.
 * The deviation that remains is maximum 0.28V.
 *
 * @image html PI_regulator_down.jpg "PI controller step response down"
 * The controller takes about 20ms from the adjusted current down to almost 0mA
 * and has no overshoot. This slow step response is due to the I-component of the control loop.
 *
 *
 * @subsection selc14 Conclusion
 * The PI controller works as expected at the higher current range.
 * At the lower current range the LEDs are flickering.
 * Because the ADC needs approx. 40us to measure a new value,
 * the control loop can't regulate with a higher frequency than
 * the ADC. This leads to remaining current ripple of 3mA.
 * Because the control frequency is set to 10kHz, there is an
 * audible coil whine.
 *
 * @section selc15 Results
 *
 * 'Solution 0' and 'Solution 1' are working properly. The current through the LEDs can be regulated linear. The implementation of the
 * these two solutions was not very difficult compared to 'Solution 2'. Only the conversion for the input to the output had to be done.
 * @n However the implementation of 'Solution 2' was more complex.
 * The PI controller works as wished, but there is an audible coil whine. The calculation effort is low and takes almost no time because
 * the SW only operates with integer values. The most time-consuming operation is, to get a new ADC value. The solution uses an timer interrupt, which
 * leads to an accurate regulation of the current. In the lower current range, the LEDs are flickering, because the current ripple
 * makes up a larger percentage of the flowing current and therefore the effect of the PI controller is visible.
 *
 * <hr>
 * @n
 *
 * Board:  Starter Kit EFM32-G8XX-STK
 * Device: EFM32G890F128 (Gecko)
 *
 * @author Hanspeter Hochreutener (hhrt@zhaw.ch)
 * @date 15.7.2015
 *
 * Edited by
 * @author Lars Mueggler (muegglar@students.zhaw.ch)
 * @author Ivan Krajinovic (krajiiva@students.zhaw.ch)
 * @date 11.12.2019
 * @version 1.0.0
 *****************************************************************************/

/** ***************************************************************************
 * @file
 * @brief main file for the Moodlight
 *
 * Sets up uC, clocks, peripherals and user interface.
 * Starts the powerLEDs, etc.
 *
 * Loops in the user interface
 *
 * @author Lars Mueggler (muegglar@students.zhaw.ch)
 * @date 11.12.2019
 * @version 1.0.0
 *****************************************************************************/ 

/******************************************************************************
 * Includes
 *****************************************************************************/

// System files
#include "em_chip.h"
#include "em_device.h"
#include "em_system.h"
#include "em_cmu.h"
#include "em_emu.h"

// Own written files
#include "segmentlcd.h"
#include "globals.h"
#include "pushbuttons.h"
#include "powerLEDs.h"
#include "touchslider.h"
#include "userinterface.h"
#include "signalleds.h"
#include "adc.h"
#include "pwm.h"
#include "gpioInterrupt.h"
#include "communication.h"
#include "i2c_com.h"
#include "externSensors.h"

/******************************************************************************
 * Functions
 *****************************************************************************/

/** ****************************************************************************
 * @brief main function
 *
 * Sets up uC, clocks, peripherals and user interface.
 * Waits in low energy mode for an interrupt.
 *
 * @return nothing, as it runs in an endless loop
 *****************************************************************************/
int main(void) {
  CHIP_Init();                  		// Chip revision alignment and errata fixes
  INIT_XOclocks();						// Start crystal oscillators

  PWR_init();							// Initialize the power LEDs
  ADC0_init();							// Initialize ADC0 input
  PWM_init();	 						// Initialize PWM outputs
  COM_Init();
  GPIOINT_init();
  I2C_init();
  ES_init();

  SL_Init();							// Initialize the signal LEDs

  PB_Init();							// Initialize the pushbuttons
  PB_EnableIRQ();						// Enable pushbuttons interrupts

  CAPSENSE_Init();						// Initialize touchslider and touchgecko

  SegmentLCD_Init(false);				// Initialize the LCD
  SegmentLCD_Write("PowerUp");			// Display "PowerUp"
  SegmentLCD_Number(0);					// Display "0"

  while(1) {							// loop forever
	  SL_Toggle(SL_3_PORT, SL_3_PIN);	// can be used for oscilloscope synch.
	  UI_FSM_event();					// check for events
	  UI_FSM_state_value();				// handles the events
	  ES_Get_Sensors_Values();

  }
}
