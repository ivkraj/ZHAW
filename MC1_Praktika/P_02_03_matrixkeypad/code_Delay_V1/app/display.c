/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Project     : CT2 Lab Matrix-Keypad
 * -- Description : Modul handles the 8-digit 7-segment-display
 * --				Controls the buffer (data for each digit)
 * --				Updates the display with the stored values
 * --
 * -- $Id: display.c 1443 2018-08-23 06:28:34Z ruan $
 * ------------------------------------------------------------------------- */

#include "display.h"
#include "hal_gpio.h"

/* -- macros and function prototypes
 * --------------------------------------------------------------------------*/
#define NUM_OF_DIGITS  8            /**< Number of digits                    */
#define DIGIT_BLANK    0xFF         /**< value to be written for blank digit */
#define DISPLAY_PORT   GPIOA
/// STUDENTS: To be programmed
#define ENABLEMASK 0x1<<11



/// END: To be programmed

static const uint8_t int2hex [16] = { 0xC0,        //0
                                      0xf9,        //1
                                      0xa4,        //2
                                      0xb0,        //3
                                      0x99,        //4
                                      0x92,        //5
                                      0x82,        //6
                                      0xf8,        //7
                                      0x80,        //8
                                      0x90,        //9
                                      0x88,        //A
                                      0x83,        //b
                                      0xC6,        //C
                                      0xA1,        //D
                                      0x86,        //E
                                      0x8E };      //F

static uint8_t disp_buf[NUM_OF_DIGITS];

/*
 * See header file
 */
void disp_reg_new_value(uint8_t value)
{
    /// STUDENTS: To be programmed
    
    uint8_t index;
    
    for(index = (NUM_OF_DIGITS-1); index > 0; index--) {
        disp_buf[index] = disp_buf[index-1];
    }
    
    disp_buf[0] = value;
    
    /// END: To be programmed
}

/*
 * See header file
 */
void disp_update(void)
{
    /// STUDENTS: To be programmed    
    uint16_t data;
    for(uint16_t i = 0; i < NUM_OF_DIGITS; i++) {
        
        data = int2hex[disp_buf[i]] |( i << 8);
        hal_gpio_output_write(DISPLAY_PORT,data | ENABLEMASK);
        
        // activate enable pin
        hal_gpio_bit_reset(DISPLAY_PORT, ENABLEMASK);
        
        // disable enable pin
        hal_gpio_bit_set(DISPLAY_PORT, ENABLEMASK);
    }
    /// END: To be programmed
}
/*
 * See header file
 */
void disp_init(void)
{
    hal_gpio_output_t display_outputs = { 0x0FFF,
                                          HAL_GPIO_PUPD_NOPULL,
                                          HAL_GPIO_OUT_SPEED_2MHZ,
                                          HAL_GPIO_OUT_TYPE_PP };
    hal_gpio_init_output(DISPLAY_PORT, display_outputs);
    disp_clear_buffer();
    disp_update();
}

/*
 * See header file
 */
void disp_clear_buffer(void)
{
    uint8_t i;
    for (i = 0; i < NUM_OF_DIGITS; i++) {
        disp_buf[i] = DIGIT_BLANK;
    }
}

/* internal functions definitions ------------------------------------------ */

/// STUDENTS: To be programmed




/// END: To be programmed

